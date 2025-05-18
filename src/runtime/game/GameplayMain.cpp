#pragma once
#include "GameplayMain.hpp"
#include "Functions.hpp"
#include "../util/HookingUtil.hpp"
#include "Menu.hpp"
#include <json.hpp>
#include "Global.hpp"
#include "structures/DictionaryBuilder.hpp"
#include "structures/Stacktrace.hpp"
#include <sstream>
#include "utils/MemPatcher.hpp"

namespace GameplayMain
{
	using namespace Menu::Gameplay;

	IL2CPP::Object* gMyPlayerMoveC = nullptr;
	IL2CPP::Object* gCurrentWeaponSounds = nullptr;
	IL2CPP::List<IL2CPP::Object*>* gPlayerMoveCList = nullptr;
	IL2CPP::List<IL2CPP::Object*>* gPhotonViewList = nullptr;

	bool processNoClipAll = false;
	bool processCrashAll = false;
	bool processProjectileSpawning = false;

	bool dontDespawnBot = false;

	void HandleSoftSilent()
	{
		bool lbuttonDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

		if (!lbuttonDown)
		{
			return;
		}

		constexpr float maxraycastdistance = 100.0f;
		Vector3 screenCenter(Screen::GetWidth() * 0.5f, Screen::GetHeight() * 0.5f, 0.0f);

		IL2CPP::Object* cam = Camera::GetMain();
		IL2CPP::Object* camTransform = Component::GetTransform(cam);

		IL2CPP::Object* myPlrTransform = PlayerMoveC::GetTransform(gMyPlayerMoveC);
		Vector3 myPos = Transform::GetPosition(myPlrTransform);

		float minDistance = FLT_MAX;
		float currentFOV = General::Aim::AimbotFOV.value;
		Vector3 aimOffset = General::Aim::AimHead.value ? Vector3(0.0f, 0.55f, 0.0f) : Vector3(0.0f, 0.0f, 0.0f);

		IL2CPP::Object* targetTransform = nullptr;
		gPlayerMoveCList->ForEach([&](IL2CPP::Object* player)
		{
			if (!player || PlayerMoveC::IsDead(player) || PlayerMoveC::IsMine(player) || !PlayerMoveC::IsEnemyTo(gMyPlayerMoveC, player))
			{
				return;
			}

			Vector3 pos = PlayerMoveC::GetPosition(player);
			Vector3 plrScreenPos = Camera::WorldToScreenPoint(cam, pos);
			float distance = Vector3::Distance(screenCenter, plrScreenPos);

			if (plrScreenPos.Z <= 0 || distance >= minDistance || distance > currentFOV)
			{
				return;
			}

			Vector3 direction = Vector3::Normalized(pos - myPos);
			Ray ray = { myPos, direction };
			RaycastHit info;

			if (Physics::Raycast(ray, &info, maxraycastdistance))
			{
				IL2CPP::Object* bodyCollider = player->GetFieldRef<IL2CPP::Object*>("_bodyAimCollider");
				if (info.collider == GameObject::GetInstanceID(bodyCollider))
				{
					minDistance = distance;
					targetTransform = PlayerMoveC::GetTransform(player);
				}
			}
		});

		if (targetTransform && minDistance < currentFOV)
		{
			Vector3 targetPos = Transform::GetPosition(targetTransform) + aimOffset;
			Transform::LookAtVec(camTransform, targetPos);
		}
	}

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

	void HandleGotoPlayers()
	{
		IL2CPP::Object* cam = Camera::GetMain();
		IL2CPP::Object* camTransform = Component::GetTransform(cam);
		IL2CPP::Object* myPlrTransform = PlayerMoveC::GetTransform(gMyPlayerMoveC);
		bool lbuttonDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;

		Vector3 myPos = Transform::GetPosition(myPlrTransform);
		IL2CPP::Object* targetTransform = nullptr;
		float minDistance = FLT_MAX;
		Vector3 aimOffset = General::Aim::AimHead.value ? Vector3(0.0f, 0.55f, 0.0f) : Vector3(0.0f, 0.0f, 0.0f);

		gPlayerMoveCList->ForEach([&](IL2CPP::Object* player)
		{
			if (PlayerMoveC::IsDead(player) || PlayerMoveC::IsMine(player) || !PlayerMoveC::IsEnemyTo(gMyPlayerMoveC, player))
			{
				return;
			}
				
			Vector3 pos = PlayerMoveC::GetPosition(player);
			float distance = Vector3::Distance(myPos, pos);

			if (distance < minDistance)
			{
				minDistance = distance;
				targetTransform = PlayerMoveC::GetTransform(player);
			}
		});

		if (targetTransform)
		{
			Vector3 targetPos = Transform::GetPosition(targetTransform);
			Vector3 aimPos = targetPos + aimOffset;

			Transform::LookAtVec(camTransform, aimPos);
			Transform::SetPosition(myPlrTransform, targetPos + Vector3(0, 0, 2.5));
		}
	}

	void HandleSpamChat(IL2CPP::Object* photonView, const std::string& content)
	{
		auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
		args->Set(0, IL2CPP::String::Create(content));
		args->Set(1, IL2CPP::String::Create(""));

		PhotonView::RPC(photonView, EventEnum::SendChatMessageWithIcon, PhotonTargets::All, args);
	}

	void CrashEveryone()
	{
		#include "CrashString.h"

		if (gPhotonViewList == nullptr) return;

		gPhotonViewList->ForEach([&](IL2CPP::Object* view)
		{
			if (!PhotonView::IsMine(view)) return;

			auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
			args->Set(0, IL2CPP::String::Create(crashString));
			args->Set(1, IL2CPP::String::Create(""));

			PhotonView::RPC(view, EventEnum::SendChatMessageWithIcon, PhotonTargets::Others, args);
		});
	}

	void HandleSpeedhackRPC(IL2CPP::Object* photonView, float speed)
	{
		if (PhotonView::IsMine(photonView)) return;

		auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
		args->Set(0, IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Float, speed));
		args->Set(1, IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Float, 9999999999.0f));

		PhotonView::RPC(photonView, EventEnum::PlayerEffectRPC, PhotonTargets::All, args);
	}

	void HandleJumpDisableRPC(IL2CPP::Object* photonView)
	{
		if (PhotonView::IsMine(photonView)) return;

		auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
		args->Set(0, IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Float, 2.f));
		args->Set(1, IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Float, 2.f));

		PhotonView::RPC(photonView, EventEnum::JumpDisableRPC, PhotonTargets::All, args);
	}

	void SpamProjectiles()
	{
		if (!gCurrentWeaponSounds || !gMyPlayerMoveC) return;

		gPlayerMoveCList->ForEach([&](IL2CPP::Object* player)
		{
			if (PlayerMoveC::IsMine(player)) return;

			Rocket::Create(
				ServerMods::PrefabSpawner::projectilePrefabs[ServerMods::PrefabSpawner::ProjectileBrowser.index],
				gMyPlayerMoveC,
				gCurrentWeaponSounds,
				ServerMods::PrefabSpawner::LongLifetime.value,
				PlayerMoveC::GetPosition(player)
			);
		});
	}

	void AttractEveryone()
	{
		if (gMyPlayerMoveC == nullptr) return;

		IL2CPP::Object* pixelView = gMyPlayerMoveC->GetFieldPtr<IL2CPP::Object*>("pixelView");
		IL2CPP::Object* photonView = gMyPlayerMoveC->GetFieldPtr<IL2CPP::Object*>("photonView");

		auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 4);
		args->Set(0, IL2CPP::String::Create("Weapon1652"));
		args->Set(1, IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Int32, 9));
		args->Set(2, IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Float, 9999999999.0f));
		args->Set(3, IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Int32, PixelView::GetViewID(pixelView)));

		PhotonView::RPC(photonView, EventEnum::PlayerEffectRPC, PhotonTargets::OthersBuffered, args);
	}

	void NoClipEveryone()
	{
		processNoClipAll = true;
	}

	void SpawnBotPrefab()
	{
		if (gPlayerMoveCList == nullptr || gPlayerMoveCList->GetSize() == 0) return;

		auto arr = IL2CPP::Array<int>::Create(IL2CPP::DefaultTypeClass::Int32, 6);
		arr->Set(0, 834);
		arr->Set(1, 834);
		arr->Set(2, 834);
		arr->Set(3, 834);
		arr->Set(4, 834);
		arr->Set(5, 834);

		std::string botname = ServerMods::PrefabSpawner::BotName.GetValue();
		DictionaryBuilder dic = DictionaryBuilder::Create(
			IL2CPP::DefaultTypeClass::String,
			IL2CPP::DefaultTypeClass::Object
		);

		dic.Add(IL2CPP::String::Create("w"), (IL2CPP::Object*)arr);
		dic.Add(IL2CPP::String::Create("av"), IL2CPP::String::Create("avatar_unknown"));
		dic.Add(IL2CPP::String::Create("sn"), IL2CPP::String::Create("league_skin_steel"));
		dic.Add(IL2CPP::String::Create("n"), IL2CPP::String::Create(botname));
		dic.Add(IL2CPP::String::Create("gg"), IL2CPP::String::Create("gadget_singularity"));
		dic.Add(IL2CPP::String::Create("pa"), IL2CPP::String::Create("armor_ultimate_defense"));
		dic.Add(IL2CPP::String::Create("spa"), IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Boolean, true));
		dic.Add(IL2CPP::String::Create("pid"), IL2CPP::Object::BoxValue(IL2CPP::DefaultTypeClass::Int32, rand()));

		auto settings = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 1);
		settings->Set(0, dic.GetInstance());

		auto gameObject = PhotonNetwork::InstantiatePrefab(
			IL2CPP::String::Create("Bots/BotInstance"),
			Vector3::Zero(),
			Quaternion::Identity(),
			(char)0,
			settings
		);

		dontDespawnBot = true;
	}

	$Hook(void, WeaponManager, (IL2CPP::Object* _this))
	{
		gMyPlayerMoveC = _this->GetFieldPtr<IL2CPP::Object*>("myPlayerMoveC");
		gPlayerMoveCList = IL2CPP::ClassMapping::GetClass("PlayerListClass")->GetField(0x0)->GetValue<IL2CPP::List<IL2CPP::Object*>*>(nullptr);
		gPhotonViewList = IL2CPP::ClassMapping::GetClass("PhotonObjectCacher")->GetField(0x0)->GetValue<IL2CPP::List<IL2CPP::Object*>*>(nullptr);

		if (gMyPlayerMoveC != nullptr)
			gCurrentWeaponSounds = _this->GetFieldPtr<IL2CPP::Object*>("myPlayerMoveC", 10);

		if (gPlayerMoveCList == nullptr || gPlayerMoveCList->GetSize() == 0)
			dontDespawnBot = false;

		if (ServerMods::PrefabSpawner::AutoSpawn.value && Global::gGlobalTick % 20)
		{
			SpamProjectiles();
		}

		if (gMyPlayerMoveC != nullptr)
		{
			if (!PhotonNetwork::IsMasterClient())
			{
				PhotonNetwork::SetMasterClient(PhotonNetwork::GetLocalPlayer());
			}

			if (General::Aim::Aimbot.value && gPlayerMoveCList != nullptr)
			{
				HandleAimbot();
			}

			if (General::Aim::SoftSilentAim.value && gPlayerMoveCList != nullptr)
			{
				HandleSoftSilent();
			}

			if (General::Player::GotoPlayers.value && gPlayerMoveCList != nullptr)
			{
				HandleGotoPlayers();
			}

			if (gPhotonViewList != nullptr)
			{
				gPhotonViewList->ForEach([&](IL2CPP::Object* view)
				{
					if (ServerMods::ChatSpam::SpamChat.value && Global::gGlobalTick % 10 == 0)
					{
						HandleSpamChat(view, ServerMods::ChatSpam::message.GetValue());
					}

					if (Global::gGlobalTick % 60 != 0) return;

					if (ServerMods::RPC::SlowdownAll.value)
					{
						HandleSpeedhackRPC(view, -99999999999.0f);
					}

					if (ServerMods::RPC::SpeedupAll.value)
					{
						HandleSpeedhackRPC(view, 99999999999.0f);
					}

					if (ServerMods::RPC::DisableJumpAll.value)
					{
						HandleJumpDisableRPC(view);
					}
				});
			}

			if (processNoClipAll)
			{
				static int tpToNormalPosTimer = 0;
				static Vector3 oldPos;

				if (tpToNormalPosTimer <= 0)
				{
					oldPos = PlayerMoveC::GetPosition(gMyPlayerMoveC);
				}

				if (tpToNormalPosTimer < 30)
				{
					tpToNormalPosTimer++;
					// DUMB COMPILER
					volatile float nanFloat = 0.0f;
					PlayerMoveC::SetPosition(gMyPlayerMoveC, Vector3(0.0f / nanFloat, 0.0f / nanFloat, 0.0f / nanFloat));
				}
				else
				{
					PlayerMoveC::SetPosition(gMyPlayerMoveC, oldPos);
					tpToNormalPosTimer = 0;
					processNoClipAll = false;
				}
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
		Color crosshairColor = _this->GetFieldRef<Color>(0x27);
		if (gMyPlayerMoveC != nullptr && General::Aim::Triggerbot.value
			&& crosshairColor.r >= 0.5f && crosshairColor.g == 0.0f && crosshairColor.b == 0.0f)
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
		if (_this->GetFieldRef<bool>("harpoonMinDistance", 14) == false)
		{
			$CallOrig(Rocket, _this, rocketSettings);
			return;
		}

		IL2CPP::Object* gameObj = Component::GetGameObject(_this);

		if (GameObject::GetName(gameObj)->Equals("ThugHunting"))
		{
			rocketSettings->GetFieldRef<float>("lifeTime") = 9999999999999.0f;
			GameObject::SetName(gameObj, IL2CPP::String::Create("Rocket (Clone)"));
		}

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
			_this->GetFieldRef<bool>("isCharging") = false;
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

		if (ServerMods::Modifier::TargetFloatHit.value)
		{
			_this->GetFieldRef<bool>("isPoisoning") = true;
			_this->GetFieldRef<int>("poisonCount") = 3;
			_this->GetFieldRef<int>("poisonType") = 2;
			_this->GetFieldRef<float>("poisonDamageMultiplier") = 0.1f;
		}

		if (ServerMods::Modifier::ElectricShock.value)
		{
			_this->GetFieldRef<bool>("isElectricShock") = true;
			_this->GetFieldRef<float>("electricShockCoeff") = 99999999.0f;
			_this->GetFieldRef<float>("electricShockTime") = 99999999.0f;
		}

		if (ServerMods::Modifier::Polymorpher.value)
		{
			_this->GetFieldRef<bool>("polymorpher") = true;
			_this->GetFieldRef<int>("polymorphType") = rand() % 3;
			_this->GetFieldRef<float>("polymorphDuarationTime") = 99999999.0f;
			_this->GetFieldRef<float>("polymorphMaxHealth") = 99999999.0f;

			_this->GetFieldRef<bool>("harpoon") = true;
			_this->GetFieldRef<float>("harpoonMaxDistance") = 99999999.0f;
			_this->GetFieldRef<float>("harpoonMinDistance") = 99999999.0f;
		}

		$CallOrig(WeaponSounds, _this);
	}

	$Hook(void, SendPlayerEffect, (void* _this, void* player, IL2CPP::String* source, int effectIndex, float duration, int senderPixelID))
	{
		if (_this != nullptr && ServerMods::Modifier::TargetFloatHit.value)
		{
			for (int i = 1; i < 77; i++)
			{
				if (i == 9) continue;
				$CallOrig(SendPlayerEffect, _this, player, source, i, 99999999999.0f, senderPixelID);
			}
		}
		$CallOrig(SendPlayerEffect, _this, player, source, effectIndex, duration, senderPixelID);
	}

	$Hook(bool, isAvailable, (int filterMaps))
	{
		//if (Menu::Misc::Bypass::Armory::AllowSandbox.IsActive())
		//{
		//	return true;
		//}

		return false;
	}

	#pragma region PatchesHooks
	$Hook(float, FireRateModifier, (IL2CPP::Object* _this))
	{
		if (General::Player::FirerateHack.value)
		{
			return General::Player::Firerate.value;
		}

		return $CallOrig(FireRateModifier, _this);
	}

	$Hook(float, get_SpeedModifier, (IL2CPP::Object* _this))
	{
		if (General::Movement::Speedhack.value)
		{
			return 9999;
		}

		return $CallOrig(get_SpeedModifier, _this);
	}

	$Hook(void, PhotonNetwork_Destroy, (IL2CPP::Object* obj))
	{
		if (dontDespawnBot && Stacktrace::New()->ToString()->Contains("PlayerBotsManager"))
		{
			return;
		}

		$CallOrig(PhotonNetwork_Destroy, obj);
	}
	#pragma endregion

	void INIT()
	{
		using namespace IL2CPP::ClassMapping;

		ServerMods::RPC::AttractEveryone.OnClick(AttractEveryone);
		ServerMods::PrefabSpawner::SpawnBot.OnClick(SpawnBotPrefab);
		ServerMods::RPC::CrashEveryone.OnClick([&]
		{
			Global::ExecuteOnGameThread([]
			{
				CrashEveryone();
			});
		});

		ServerMods::PrefabSpawner::SpawnProjectile.OnClick([&]
		{
			Global::ExecuteOnGameThread([]
			{
				SpamProjectiles();
			});
		});

		ServerMods::RPC::NoClipEveryone.OnClick([&] { processNoClipAll = true; });

		General::Player::Godmode.OnToggle([&](bool value)
		{
			static IL2CPP::SignaturePattern pattern{ "private", "Void", nullptr, {"Single", "ENUM", "ENUM", "ENUM", "ENUM", "Vector3", "String", "Int32", nullptr} };
			static auto ptr = GetClass("Player_move_c")->GetMethodByPattern(pattern)->GetPointer();

			if (value)
			{
				MemPatcher::Nop(ptr);
			}
			else
			{
				MemPatcher::Restore(ptr);
			}
		});


		ServerMods::Modifier::FriendlyFire.OnToggle([&](bool value)
		{
			static IL2CPP::SignaturePattern pattern{ "internal static", "Void", nullptr, {"Boolean"} };
			static auto ptr = GetClass("GameConnect")->GetMethodByPattern(pattern, -6)->GetPointer();

			if (value)
			{
				MemPatcher::ReturnFalse(ptr);
			}
			else
			{
				MemPatcher::Restore(ptr);
			}
		});

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
				{ "private", "Boolean", nullptr, {"RocketSettings"} }
			),
			);

		$RegisterHook(
			CreateRocket,
			GetClass("Player_move_c")->GetMethodByPattern(
				{ "internal static", "Rocket", nullptr, {"WeaponSounds", "Vector3", "Quaternion", "Single", "Int32", "Int32"} }
			),
			);

		$RegisterHook(
			WeaponSounds,
			GetClass("WeaponSounds")->GetMethod("Update"),
			);

		$RegisterHook(
			SendPlayerEffect,
			GetClass("Player_move_c")->GetMethodByPattern(
				{ "internal", "Void", nullptr, {nullptr, "String", "Int32", "Single", "Int32"} }
			)
		);

		$RegisterHook(
			FireRateModifier,
			GetClass("BalanceController")->GetMethod(0x64)
		);

		$RegisterHook(
			get_SpeedModifier,
			GetClass("ItemRecord")->GetMethod("get_SpeedModifier")
		);

		//$RegisterHook(
		//	isAvailable,
		//	GetClass("ItemRecord")->GetMethod(0x56)
		//);

		$RegisterHook(
			PhotonNetwork_Destroy,
			GetClass("PhotonNetwork")->GetMethod(0x7a)
		);
	}
}
