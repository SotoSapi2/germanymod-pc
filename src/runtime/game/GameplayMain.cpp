#pragma once
#include "GameplayMain.hpp"
#include "Functions.hpp"
#include "../util/HookingUtil.hpp"
#include "Menu.hpp"
#include <json.hpp>

namespace GameplayMain
{
	using namespace Menu::Gameplay;

	IL2CPP::Object* gMyPlayerMoveC = nullptr;
	IL2CPP::List<IL2CPP::Object*>* gPlayerMoveCList = nullptr;

	void HandleAimbot()
	{
		float deltaTime = Time::DeltaTime();
		Vector3 screenCenter(Screen::GetWidth() / 2, Screen::GetHeight() / 2, 0);
		IL2CPP::Object* cam = Camera::GetMain();
		IL2CPP::Object* camTransform = Component::GetTransform(cam);
		Vector3 camPos = Transform::GetPosition(camTransform);

		IL2CPP::Object* myPlrTransform = PlayerMoveC::GetTransform(gMyPlayerMoveC);
		Vector3 myPos = Transform::GetPosition(myPlrTransform);

		IL2CPP::Object* targetTransform = nullptr;
		float minDistance = FLT_MAX;

		gPlayerMoveCList->ForEach([&](IL2CPP::Object* player)
		{
			if (player == nullptr) return;
			Vector3 pos = PlayerMoveC::GetPosition(player);
			Vector3 plrScreenPos = Camera::WorldToScreenPoint(cam, pos);
			IL2CPP::Object* bodyCollider = player->GetFieldRef<IL2CPP::Object*>("_bodyAimCollider");
			IL2CPP::Object* headCollider = player->GetFieldRef<IL2CPP::Object*>("_headAimCollider");

			float distance = Vector3::Distance(screenCenter, plrScreenPos);
			bool isVisible = false;

			Ray ray = {
				myPos,
				Vector3::Normalized(pos - myPos)
			};

			RaycastHit info;
			if (Physics::Raycast(ray, &info, 100))
			{
				isVisible = info.collider == GameObject::GetInstanceID(bodyCollider);
			}

			if (distance < minDistance && !PlayerMoveC::IsDead(player) && !PlayerMoveC::IsMine(player)
				&& PlayerMoveC::IsEnemyTo(gMyPlayerMoveC, player) && plrScreenPos.Z > 0 && isVisible)
			{
				minDistance = distance;
				targetTransform = PlayerMoveC::GetTransform(player);
			}
		});

		if (targetTransform != nullptr && minDistance < General::Aim::AimbotFOV.value)
		{
			Vector3 aimOffset;
			if (General::Aim::AimHead.value)
			{
				aimOffset = Vector3(0, 0.55, 0);
			}

			Vector3 targetPos = Transform::GetPosition(targetTransform);
			Vector3 camDirVec = Vector3::Normalized((targetPos + aimOffset) - camPos) * Time::DeltaTime();
			Vector3 myPlayerDirVec = Vector3::Normalized((targetPos + aimOffset) - myPos) * Time::DeltaTime();

			float smooth = 1.0f - (1.0f * General::Aim::AimbotSmoothing.value);

			Vector3 camForward = Vector3::Slerp(Transform::GetFoward(camTransform), camDirVec, smooth);
			Vector3 myPlrForward = Vector3::Slerp(Transform::GetFoward(myPlrTransform), myPlayerDirVec, smooth);

			Vector3 oldPlrRot = Quaternion::ToEuler(Transform::GetRotation(myPlrTransform));
			Transform::LookAtVec(myPlrTransform, myPos + myPlrForward);

			Vector3 newPlrRot = Quaternion::ToEuler(Transform::GetRotation(myPlrTransform));
			newPlrRot.X = oldPlrRot.X;
			newPlrRot.Z = oldPlrRot.Z;
			Transform::SetRotation(myPlrTransform, Quaternion::FromEuler(newPlrRot));

			Vector3 oldCamRot = Quaternion::ToEuler(Transform::GetRotation(camTransform));
			Transform::LookAtVec(camTransform, camPos + camForward);
			Vector3 newCamRot = Quaternion::ToEuler(Transform::GetRotation(camTransform));
			newCamRot.Y = oldCamRot.Y;
			Transform::SetRotation(camTransform, Quaternion::FromEuler(newCamRot));
		}
	}

	$Hook(void, WeaponManager, (IL2CPP::Object* _this))
	{
		if (gMyPlayerMoveC == nullptr)
			gMyPlayerMoveC = _this->GetFieldRef<IL2CPP::Object*>("myPlayerMoveC");

		if(gPlayerMoveCList == nullptr)
			gPlayerMoveCList = IL2CPP::ClassMapping::GetClass("PlayerListClass")->GetField(0x0)->GetValue<IL2CPP::List<IL2CPP::Object*>*>(nullptr);

		if (gMyPlayerMoveC != nullptr)
		{
			if (General::Aim::Aimbot.value && gPlayerMoveCList != nullptr)
			{
				HandleAimbot();
			}

			if (General::Player::InfAmmo.value)
			{
				WeaponMenager::GetPlayerWeapons(_this)->ForEach([&](IL2CPP::Object* wepW)
				{
					WeaponMenager::SetClipAmmo(wepW, 999);
				});
			}

			if (General::Visual::Xray.value)
			{
				gPlayerMoveCList->ForEach([&](IL2CPP::Object* player)
				{
					PlayerMoveC::SetXrayShader(player, true);
				});
			}

			if (General::Movement::AirJump.value)
			{
				IL2CPP::Object* firstPersonControl = PlayerMoveC::GetSkinName(gMyPlayerMoveC)->GetFieldRef<IL2CPP::Object*>("firstPersonControl");

				firstPersonControl->GetFieldRef<bool>("runJoystickAngleDeadZone", 1) = true;
				firstPersonControl->GetFieldRef<bool>("batleRoyaleAdditionalLandingGravity", 1) = true;
				firstPersonControl->GetFieldRef<bool>("_moveC", 38) = false;
			}
		}

		$CallOrig(WeaponManager, _this);
	}

	$Hook(void, AimCrosshairController, (IL2CPP::Object* _this))
	{
		const Color* crosshairColor = _this->GetFieldPtr<Color>(0x27);
		if (General::Aim::Triggerbot.value && crosshairColor->r >= 0.5f && crosshairColor->g == 0 && crosshairColor->b == 0)
		{
			PlayerMoveC::ShotPressed(gMyPlayerMoveC);
		}

		$CallOrig(AimCrosshairController, _this);
	}

	$Hook(void, GameConnect_OnRoomJoin, (IL2CPP::Object* _this))
	{
		using nlohmann::json;
		json matchSettings = json::array();

		auto currentRoom = PhotonNetwork::GetCurrentRoom();
		auto hashtable = RoomInfo::GetHashtable(currentRoom);
		
		if (General::Visual::TPS.value)
		{
			matchSettings.push_back({
				{"type", "EnableTPCamera"},
				{"bool", true}
			});
		}

		if (General::Player::NoFixedDelay.value)
		{
			matchSettings.push_back({
				{"type", "MovementScheme"},
				{"custom", "Oldschool"}
			});
		}

		if (General::Movement::GravityToggle.value)
		{
			matchSettings.push_back({
				{"type", "Gravity"},
				{"float", General::Movement::GravityPower.value}
			});
		}

		if (!matchSettings.empty())
		{
			PhotonHashtable::Set(
				hashtable,
				IL2CPP::String::Create("privateCustomParams"), 
				IL2CPP::String::Create(matchSettings.dump())
			);
		}

		$CallOrig(GameConnect_OnRoomJoin, _this);
	}

	$Hook(void, Rocket, (IL2CPP::Object* _this, IL2CPP::Object* rocketSettings))
	{
		if (General::Rocket::NuclearExplosion.value)
		{
			_this->GetFieldRef<IL2CPP::String*>(0x8) = IL2CPP::String::Create("a-bomb_mini");
			rocketSettings->GetFieldRef<IL2CPP::String*>("explosionName") = IL2CPP::String::Create("a-bomb_mini");
		}

		if (General::Rocket::LongLifetime.value)
		{
			rocketSettings->GetFieldRef<float>("lifeTime") = 999999999.0f;
		}

		if (General::Rocket::Gravity.value)
		{
			rocketSettings->GetFieldRef<bool>("useGravity") = true;
		}

		if (General::Rocket::Ricochet.value)
		{
			rocketSettings->GetFieldRef<bool>("isRickochet") = true;
			rocketSettings->GetFieldRef<int>("rickochetsCount") = 999999999;
		}

		switch (General::Rocket::RocketMode.index)
		{
			case 1: // Homing
				rocketSettings->GetFieldRef<int>("typeFly") = 10;
				rocketSettings->GetFieldRef<float>("autoRocketForce") = 15.0f;
				rocketSettings->GetFieldRef<float>("raduisDetectTarget") = 999999999999.0f;
				rocketSettings->GetFieldRef<float>("autoHomingAngle") = 999999999999.0f;
				break;
			case 2: // Follow Crosshair
				rocketSettings->GetFieldRef<int>("typeFly") = 4;
				rocketSettings->GetFieldRef<float>("autoRocketForce") = 15.0f;
				break; 
			default:
				break;
		}

		$CallOrig(Rocket, _this, rocketSettings);
	}

	$Hook(IL2CPP::Object*, CreateRocket, (IL2CPP::Object* weaponSounds, Vector3 pos, Quaternion rot, float chargePower, int smoke, int whateverthisis))
	{
		if (General::Rocket::RocketTower.value)
		{
			for (float i = 0; i < 15; ++i)
			{
				Vector3 moddedPos = pos + Vector3(0, i, 0);
				$CallOrig(CreateRocket, weaponSounds, moddedPos, rot, chargePower, smoke, whateverthisis);
			}
		}

		return $CallOrig(CreateRocket, weaponSounds, pos, rot, chargePower, smoke, whateverthisis);
	}

	$Hook(void, WeaponSounds, (IL2CPP::Object* _this))
	{
		if (General::Bullet::ForceShotgun.value)
		{
			_this->GetFieldRef<bool>("isShotGun") = true;
			_this->GetFieldRef<float>("shotgunMinDamageCoef") = 1;
			_this->GetFieldRef<float>("shotgunOverDamageDistance") = 1;
			_this->GetFieldRef<float>("shotgunOverDamageCoef") = 1;
			_this->GetFieldRef<int>("isShotGun") = 30;
		}

		if (General::Bullet::BulletExplode.value)
		{
			_this->GetFieldRef<bool>("bulletExplode") = true;
		}

		if (General::Bullet::Wallbreak.value)
		{
			_this->GetFieldRef<bool>("railgun") = true;
			_this->GetFieldRef<bool>("railgunStopAtWall") = true;
		}

		if (General::Effects::NoCharge.value)
		{
			_this->GetFieldRef<bool>("isCharging") = true;
			_this->GetFieldRef<int>("chargeMax") = 2;
		}

		if (General::Effects::EnemyMarker.value)
		{
			_this->GetFieldRef<bool>("enemyMarker") = true;
			_this->GetFieldRef<bool>("enemyMarkerWhenShot") = true;
			_this->GetFieldRef<bool>("enemyMarkerWhenAiming") = true;
			_this->GetFieldRef<float>("enemyMarketChargeTime") = 0;
		}

		if (General::Effects::KillSpeedboost.value)
		{
			_this->GetFieldRef<bool>("isFastAfterKill") = true;
			_this->GetFieldRef<float>("fastMultiplier") = 10.0f;
			_this->GetFieldRef<int>("maxStackAfterKill") = 10;
			_this->GetFieldRef<float>("timeFastAfterKill") = 5.0f;
		}

		if (General::Effects::Lifesteal.value)
		{
			_this->GetFieldRef<bool>("isDamageHeal") = true;
			_this->GetFieldRef<bool>("isDamageAndArmorHeal") = true;
			_this->GetFieldRef<float>("damageHealMultiplier") = 99999999.0f;
		}

		if (General::Effects::HeadMagnifier.value)
		{
			_this->GetFieldRef<bool>("isHeadMagnifier") = true;
			_this->GetFieldRef<float>("headMagnifierTime") = 99999999.0f;
		}

		if (General::Aim::Killaura.value)
		{
			float killauraRadius = General::Aim::InfKillauraRadius.value ? FLT_MAX : General::Aim::KillauraRadius.value;

			_this->GetFieldRef<bool>("isRoundMelee") = true;
			_this->GetFieldRef<float>("radiusRoundMelee") = killauraRadius;
		}

		$CallOrig(WeaponSounds, _this);
	}

	#pragma region PatchesHooks
	$Hook(void, TakeDamage, (IL2CPP::Object* _this, float dmg, int sex0, int sex1, int sex2, int sex3, Vector3 vec, IL2CPP::String* wepPrefab, int idfk, IL2CPP::Object* idfk1))
	{
		if (General::Player::Godmode.value)
		{
			return;
		}

		$CallOrig(TakeDamage, _this, dmg, sex0, sex1, sex2, sex3, vec, wepPrefab, idfk, idfk1);
	}

	$Hook(float, FireRateModifier, (IL2CPP::Object* _this))
	{
		if (General::Player::FirerateHack.value)
		{
			return General::Player::Firerate.value;
		}

		return $CallOrig(FireRateModifier, _this);
	}

	$Hook(void, ForceCatSpamTest, (IL2CPP::Object* _this, bool idfk0, float idfk1, bool idfk2, float idfk3))
	{
		if (General::Player::NoCatDelay.value)
		{
			return;
		}

		$CallOrig(ForceCatSpamTest, _this, idfk0, idfk1, idfk2, idfk3);
	}

	$Hook(float, get_SpeedModifier, (IL2CPP::Object* _this))
	{
		if (General::Movement::Speedhack.value)
		{
			return 9999;
		}

		return $CallOrig(get_SpeedModifier, _this);
	}
	#pragma endregion

	void INIT()
	{
		using namespace IL2CPP::ClassMapping;

		$RegisterHook(
			WeaponManager, 
			GetClass("WeaponManager")->GetMethod("Update"),
		);

		$RegisterHook(
			AimCrosshairController, 
			GetClass("AimCrosshairController")->GetMethod("LateUpdate")
		);

		$RegisterHook(
			GameConnect_OnRoomJoin,
			GetClass("GameConnect")->GetMethod("OnJoinedRoom")
		);

		$RegisterHook(
			Rocket,
			GetClass("Rocket")->GetMethodByPattern(
				{"private", "Boolean", nullptr, {"RocketSettings"}}
			),
		);

		$RegisterHook(
			CreateRocket,
			GetClass("Player_move_c")->GetMethodByPattern(
				{"internal static", "Rocket", nullptr, {"WeaponSounds", "Vector3", "Quaternion", "Single", "Int32", "Int32"}}
			),
		);

		$RegisterHook(
			WeaponSounds,
			GetClass("WeaponSounds")->GetMethod("Update"),
		);

		$RegisterHook(
			TakeDamage, 
			GetClass("Player_move_c")->GetMethodByPattern(
				{"private", "Void", nullptr, {"Single", "ENUM", "ENUM", "ENUM", "ENUM", "Vector3", "String", "Int32", nullptr}}
			)
		);

		$RegisterHook(
			FireRateModifier,
			GetClass("BalanceController")->GetMethod(0x64)
		);

		$RegisterHook(
			ForceCatSpamTest,
			GetClass("Player_move_c")->GetMethod("Awake", 0, 2)
		);

		$RegisterHook(
			get_SpeedModifier,
			GetClass("ItemRecord")->GetMethod("get_SpeedModifier")
		);
	}
}