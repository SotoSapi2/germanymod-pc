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
#include "DumpsterFire.hpp"
#include "utils/MemPatcher.hpp"

namespace GameplayMain
{
	using namespace Menu::Gameplay;
	IL2CPP::Object* gMyPlayerMoveC = nullptr;
	IL2CPP::Object* gCurrentWeaponSounds = nullptr;
	IL2CPP::List<IL2CPP::Object*>* gPlayerMoveCList = nullptr;
	IL2CPP::List<IL2CPP::Object*>* gPhotonViewList = nullptr;
	bool gLogRPC = false;

	IL2CPP::Object* networkTable;
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

		constexpr float maxRaycastDistance = 100.0f;
		Vector3 screenCenter(Screen::GetWidth() * 0.5f, Screen::GetHeight() * 0.5f, 0.0f);

		IL2CPP::Object* cam = Camera::GetMain();
		IL2CPP::Object* camTransform = Component::GetTransform(cam);

		IL2CPP::Object* myPlrTransform = PlayerMoveC::GetPlayerTransform(gMyPlayerMoveC);
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

			if (Physics::Raycast(ray, &info, maxRaycastDistance))
			{
				IL2CPP::Object* bodyCollider = player->GetFieldRef<IL2CPP::Object*>("_bodyAimCollider");
				if (info.collider == GameObject::GetInstanceID(bodyCollider))
				{
					minDistance = distance;
					targetTransform = PlayerMoveC::GetPlayerTransform(player);
				}
			}
		});

		if (targetTransform && minDistance < currentFOV)
		{
			Vector3 targetPos = Transform::GetPosition(targetTransform) + aimOffset;
			Transform::LookAtVec(camTransform, targetPos);
		}
	}

	void HandleGotoPlayers()
	{
		IL2CPP::Object* cam = Camera::GetMain();
		IL2CPP::Object* camTransform = Component::GetTransform(cam);
		IL2CPP::Object* myPlrTransform = PlayerMoveC::GetPlayerTransform(gMyPlayerMoveC);
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
				targetTransform = PlayerMoveC::GetPlayerTransform(player);
			}
		});

		if (targetTransform)
		{
			Vector3 targetPos = Transform::GetPosition(targetTransform);
			Vector3 aimPos = targetPos + aimOffset;

			Transform::LookAtVec(camTransform, aimPos);
			Transform::SetPosition(myPlrTransform, targetPos + Vector3(0, 0, General::Player::GotoPlayersDistance.value));
		}
	}

	void HandleAimbot()
	{
		float deltaTime = Time::DeltaTime();
		Vector3 screenCenter(Screen::GetWidth() / 2, Screen::GetHeight() / 2, 0);
		IL2CPP::Object* cam = Camera::GetMain();
		IL2CPP::Object* camTransform = Component::GetTransform(cam);
		Vector3 camPos = Transform::GetPosition(camTransform);

		IL2CPP::Object* myPlrTransform = PlayerMoveC::GetPlayerTransform(gMyPlayerMoveC);
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
				targetTransform = PlayerMoveC::GetPlayerTransform(player);
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

	void HandleSpamChat(IL2CPP::Object* photonView, const std::string& content)
	{
		auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
		args->Set(0, IL2CPP::String::Create(content));
		args->Set(1, IL2CPP::String::Create(""));

		PhotonView::RPC(photonView, EventEnum::SendChatMessageWithIcon, PhotonTargets::All, args);
	}

	//void CrashEveryone()
	//{
	//	#include "CrashString.h"

	//	if (gPhotonViewList == nullptr) return;

	//	gPhotonViewList->ForEach([&](IL2CPP::Object* view)
	//	{
	//		if (!PhotonView::IsMine(view)) return;

	//		auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
	//		args->Set(0, IL2CPP::String::Create(crashString));
	//		args->Set(1, IL2CPP::String::Create(""));

	//		PhotonView::RPC(view, EventEnum::SendChatMessageWithIcon, PhotonTargets::Others, args);
	//	});
	//}

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

		auto gameObject = PhotonNetwork::SceneInstantiatePrefab(
			IL2CPP::String::Create("Bots/BotInstance"),
			Vector3::Zero(),
			Quaternion::Identity(),
			(char)0,
			settings
		);

		dontDespawnBot = true;
	}

	void HandleFlyhack()
	{
		float flySpeed = Menu::Gameplay::General::Movement::Flyspeed.value;
		bool isWDown = (GetAsyncKeyState('W') & 0x8000) != 0;
		bool isADown = (GetAsyncKeyState('A') & 0x8000) != 0;
		bool isSDown = (GetAsyncKeyState('S') & 0x8000) != 0;
		bool isDDown = (GetAsyncKeyState('D') & 0x8000) != 0;
		bool isSpaceDown = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
		bool isCtrlDown = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

		IL2CPP::Object* camera = Camera::GetMain();
		if (!camera)
			return;

		IL2CPP::Object* cameraTransform = Component::GetTransform(camera);
		if (!cameraTransform)
			return;

		IL2CPP::Object* playerTransform = PlayerMoveC::GetPlayerTransform(gMyPlayerMoveC);
		if (!playerTransform)
			return;

		Vector3 playerPos = Transform::GetPosition(playerTransform);
		Vector3 moveDirection(0.0f, 0.0f, 0.0f);

		Vector3 cameraForward = Transform::GetFoward(cameraTransform);
		Vector3 worldUp(0.0f, 1.0f, 0.0f);
		Vector3 cameraRight = Vector3::Cross(cameraForward, worldUp);

		cameraForward.Y = 0.0f;
		cameraRight.Y = 0.0f;

		cameraForward = Vector3::Normalized(cameraForward);
		cameraRight = Vector3::Normalized(cameraRight);

		if (isWDown) moveDirection = moveDirection + cameraForward;
		if (isSDown) moveDirection = moveDirection - cameraForward;
		if (isDDown) moveDirection = moveDirection - cameraRight;
		if (isADown) moveDirection = moveDirection + cameraRight;
		if (isSpaceDown) moveDirection.Y += 1.0f;
		if (isCtrlDown) moveDirection.Y -= 1.0f;

		if (isWDown || isADown || isSDown || isDDown || isSpaceDown || isCtrlDown)
		{
			moveDirection = Vector3::Normalized(moveDirection);

			Vector3 newPosition = playerPos + moveDirection * flySpeed;
			Transform::SetPosition(playerTransform, newPosition);
		}
	}

	void HandlePlayerSpawning()
	{
		#ifdef EXPERIMENTAL
		auto otherPlayerList = PhotonNetwork::GetOtherPlayerList();

		otherPlayerList->ForEach([&](IL2CPP::Object* player)
		{
			auto playerObj = PhotonNetwork::InstantiatePrefab(
				IL2CPP::String::Create("NetworkTable"),
				Vector3::Zero(),
				Quaternion::Identity(),
				0
			);

			auto photonView = GameObject::GetComponent(
				playerObj,
				IL2CPP::String::Create("PhotonView")
			);

			PhotonView::TransferOwnership(photonView, player);
		});
		#endif
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

			#ifdef EXPERIMENTAL
			if (General::Aim::SoftSilentAim.value && gPlayerMoveCList != nullptr)
			{
				HandleSoftSilent();
			}
			#endif

			if (General::Player::GotoPlayers.value && gPlayerMoveCList != nullptr)
			{
				HandleGotoPlayers();
			}

			if (General::Movement::Flyhack.value && gPlayerMoveCList != nullptr)
			{
				HandleFlyhack();
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
		#ifdef EXPERIMENTAL
		if (General::Aim::SilentRocket.value)
		{
			if (!gPlayerMoveCList || !gMyPlayerMoveC)
				return $CallOrig(CreateRocket, weaponSounds, pos, rot, chargePower, smoke, whateverthisis);

			const float MAX_RAYCAST_DIST = 200.0f;

			auto* cam = Camera::GetMain();
			if (!cam)
				return $CallOrig(CreateRocket, weaponSounds, pos, rot, chargePower, smoke, whateverthisis);

			auto* camTransform = Component::GetTransform(cam);
			if (!camTransform)
				return $CallOrig(CreateRocket, weaponSounds, pos, rot, chargePower, smoke, whateverthisis);


			Vector3 screenCenter(Screen::GetWidth() / 2.0f, Screen::GetHeight() / 2.0f, 0);


			auto* myTransform = PlayerMoveC::GetPlayerTransform(gMyPlayerMoveC);
			if (!myTransform)
				return $CallOrig(CreateRocket, weaponSounds, pos, rot, chargePower, smoke, whateverthisis);

			Vector3 myPosition = Transform::GetPosition(myTransform);

			IL2CPP::Object* bestTarget = nullptr;
			float closestDist = 999999.0f;
			float fov = General::Aim::AimbotFOV.value;

			gPlayerMoveCList->ForEach([&](IL2CPP::Object* plr) {
				if (!plr) return;

				if (PlayerMoveC::IsDead(plr) || !PlayerMoveC::IsEnemyTo(gMyPlayerMoveC, plr) || PlayerMoveC::IsMine(plr))
					return;

				Vector3 enemyPos = PlayerMoveC::GetPosition(plr);
				Vector3 screenPos = Camera::WorldToScreenPoint(cam, enemyPos);

				if (screenPos.Z <= 0)
					return;

				float dist = Vector3::Distance(screenCenter, screenPos);
				if (dist > closestDist || dist > fov)
					return;

				Vector3 dir = Vector3::Normalized(enemyPos - myPosition);
				Ray ray = { myPosition, dir };
				RaycastHit hit;

				if (Physics::Raycast(ray, &hit, MAX_RAYCAST_DIST)) {
					auto* bodyCollider = plr->GetFieldRef<IL2CPP::Object*>("_bodyAimCollider");

					if (hit.collider == GameObject::GetInstanceID(bodyCollider)) {
						closestDist = dist;
						bestTarget = PlayerMoveC::GetPlayerTransform(plr);
					}
				}
			});

			if (bestTarget && closestDist < fov) {
				Vector3 aimPos = Transform::GetPosition(bestTarget);
				return $CallOrig(CreateRocket, weaponSounds, aimPos, rot, chargePower, smoke, whateverthisis);
			}

			return $CallOrig(CreateRocket, weaponSounds, pos, rot, chargePower, smoke, whateverthisis);
		}
		#endif

		if (General::Rocket::TextToRocket.value)
		{
			std::string text = Menu::Gameplay::General::Rocket::RocketTextInput.GetValue();

			const float size = 0.9f;
			const float spacing = 5.3f;
			const float mainY = 5.0f;

			std::vector<Vector3> rocketPositions;
			float twidth = text.size() * spacing;
			float sX = -twidth / 2.0f;

			for (int letterIndex = 0; letterIndex < text.size(); ++letterIndex)
			{
				char c = toupper(text[letterIndex]);
				if (DumpsterFire::gTextVectorMap.find(c) == DumpsterFire::gTextVectorMap.end()) c = '?';

				for (const auto& pt : DumpsterFire::gTextVectorMap[c])
				{
					float x = sX + pt.first * size + letterIndex * spacing;
					float y = (7 - pt.second) * size + mainY;
					float z = 0.0f;

					rocketPositions.emplace_back(pos + Vector3(x, y, z));
				}
			}

			int maxRockets = 1000;
			int rocketCount = (rocketPositions.size() < maxRockets) ? rocketPositions.size() : maxRockets;

			for (int i = 0; i < rocketCount; ++i)
			{
				const Vector3& moddedPos = rocketPositions[i];
				Quaternion moddedRot = rot;
				float moddedPower = chargePower;
				$CallOrig(CreateRocket, weaponSounds, moddedPos, moddedRot, moddedPower, smoke, whateverthisis);
			}
		}

		if (Menu::Gameplay::General::Rocket::Box3DRocket.value)
		{
			std::vector<Vector3> rocketPositions;

			const int w = 12;
			const int h = 12;
			const int d = 12;
			const float size = 2.5f;
			const float mainY = 5.0f;

			float sX = -15.0f;
			float sY = mainY - (h * size) / 2.0f;
			float sZ = -15.0f;

			for (int x = 0; x < w; x++)
			{
				for (int y = 0; y < h; y++)
				{
					for (int z = 0; z < d; z++)
					{
						float posX = sX + x * size;
						float posY = sY + y * size;
						float posZ = sZ + z * size;

						rocketPositions.emplace_back(pos + Vector3(posX, posY, posZ));
					}
				}
			}

			int maxRockets = 1000;
			int rocketCount = (rocketPositions.size() < maxRockets) ? rocketPositions.size() : maxRockets;

			for (int i = 0; i < rocketCount; ++i)
			{
				const Vector3& moddedPos = rocketPositions[i];
				Quaternion moddedRot = rot;
				float moddedPower = chargePower;
				$CallOrig(CreateRocket, weaponSounds, moddedPos, moddedRot, moddedPower, smoke, whateverthisis);
			}
		}

		if (Menu::Gameplay::General::Rocket::PenisRocket.value)
		{
			const float size = 1.9f;
			const float mainY = 5.0f;
			std::vector<Vector3> rocketPositions;

			for (const auto& pt : DumpsterFire::gPenisVectorArray)
			{
				float x = pt.X * size;
				float y = pt.Y * size + mainY;
				float z = pt.Z;
				rocketPositions.emplace_back(pos + Vector3(x, y, z));
			}

			int maxRockets = 1000;
			int rocketCount = (rocketPositions.size() < maxRockets) ? rocketPositions.size() : maxRockets;

			for (int i = 0; i < rocketCount; ++i)
			{
				const Vector3& moddedPos = rocketPositions[i];
				Quaternion moddedRot = rot;
				float moddedPower = chargePower;
				$CallOrig(CreateRocket, weaponSounds, moddedPos, moddedRot, moddedPower, smoke, whateverthisis);
			}

			return nullptr;
		}

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

	$Hook(void, PlayerSynchStream_OnPhotonSerializeView, (IL2CPP::Object* _this, IL2CPP::Object* stream, PhotonMessageInfo info))
	{
		IL2CPP::Object* playerMoveC = _this->GetFieldPtr<IL2CPP::Object*>(0x3);
		IL2CPP::Object* gameObject = Component::GetGameObject(_this);
		IL2CPP::Object* transform = Component::GetTransform(_this);
		IL2CPP::Object* photonView = GameObject::GetComponent(gameObject, IL2CPP::String::Create("PhotonView"));

		//bool isMine = PlayerMoveC::IsMine(playerMoveC);

		if (gMyPlayerMoveC != nullptr && playerMoveC == gMyPlayerMoveC && General::Visual::Spinbot.value)
		{
			static float angle = 1;
			angle += General::Visual::SpinbotSpeed.value;

			IL2CPP::Object* transform = Component::GetTransform(_this);
			Quaternion rot = Transform::GetRotation(transform);

			Transform::Rotate(transform, Vector3::Up(), angle);

			$CallOrig(PlayerSynchStream_OnPhotonSerializeView, _this, stream, info);

			Transform::SetRotation(transform, rot);
			return;
		}

		if (ServerMods::World::TpAllToCenter.value && playerMoveC != gMyPlayerMoveC)
		{
			//if (networkTable)
			//{
			//	PhotonNetwork::Destroy(networkTable);
			//}
			//LOG_TEST();
			_this->GetFieldRef<bool>(0xa) = true;
			//_this->GetFieldRef<bool>(0x5) = false;
			//_this->GetFieldRef<bool>(0xe) = true; 
			//_this->GetFieldRef<bool>(0x10) = true;

			PhotonView::TransferOwnership(photonView, PhotonNetwork::GetLocalPlayer());
			if (gMyPlayerMoveC != nullptr)
			{
				Transform::SetPosition(transform, PlayerMoveC::GetPosition(gMyPlayerMoveC) + CameraUtils::GetMainCameraLookVector() * 7);
			}
			else
			{
				Transform::SetPosition(transform, Vector3::Zero());
			}
		}

		//if (gMyPlayerMoveC == nullptr)
		//{
		//	PhotonView::TransferOwnership(photonView, PhotonNetwork::GetLocalPlayer());
		//	auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 1);
		//	args->Set(0, IL2CPP::String::Create("avatar_spec_ops_boy"));

		//	PhotonView::RPC(photonView, EventEnum::SetRoyaleAvatarRPC, PhotonTargets::AllBuffered, args);
		//}

		if (playerMoveC != gMyPlayerMoveC && ServerMods::World::CrashEveryone.value)
		{
			PhotonView::TransferOwnership(photonView, PhotonNetwork::GetLocalPlayer());
			auto playerObj = Component::GetGameObject(photonView);
			PhotonNetwork::Destroy(playerObj);
		}

		$CallOrig(PlayerSynchStream_OnPhotonSerializeView, _this, stream, info);
	}

	$Hook(void, BaseBot_OnPhotonSerializeView, (IL2CPP::Object* _this, IL2CPP::Object* stream, PhotonMessageInfo info))
	{
		if (gMyPlayerMoveC != nullptr && PhotonStream::IsWriting(stream) && ServerMods::World::GrabMonster.value)
		{
			IL2CPP::Object* botTransform = Component::GetTransform(_this);
			IL2CPP::Object* localPlayerTransform = Component::GetTransform(gMyPlayerMoveC);

			Transform::SetPosition(
				botTransform,
				PlayerMoveC::GetPosition(gMyPlayerMoveC) + CameraUtils::GetMainCameraLookVector() * 7
			);
		}

		$CallOrig(BaseBot_OnPhotonSerializeView, _this, stream, info);
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

	$Hook(bool, isAvailable, (IL2CPP::Object* _this, int filterMaps))
	{
		if (Menu::Misc::Bypass::Armory::AllowSandbox.IsActive())
		{
			return true;
		}

		return $CallOrig(isAvailable, _this, filterMaps);
	}
	#pragma endregion

	$Hook(IL2CPP::Object*, InstantiatePrefab, (IL2CPP::String* prefab, Vector3 vec, Quaternion rot, char byte))//, IL2CPP::Array<IL2CPP::Object*> * settings))
	{
		//LOG_INFO("%s", prefab->ToString().c_str());

		//#ifdef EXPRIMENTAL
		//if (prefab->Equals("NetworkTable"))
		//{
		//	auto out = $CallOrig(InstantiatePrefab, prefab, vec, rot, byte); //settings);
		//	networkTable = out;
		//	//PhotonNetwork::Destroy(out);
		//	return out;
		//}
		//#endif


		return $CallOrig(InstantiatePrefab, prefab, vec, rot, byte);
	}

	$Hook(void, PeerRPC, (IL2CPP::Object* _this, IL2CPP::Object* photonView,
		EventEnum eventEnum, PhotonTargets target, IL2CPP::Object* player,
		bool encrypted, IL2CPP::Array<IL2CPP::Object*>* parameters))
	{
		#ifdef EXPRIMENTAL
		static std::vector<EventEnum> blockedEvent = {
			EventEnum::PropertyRPC,
			EventEnum::SetMySkin,
			EventEnum::SetPixelBookID,
			EventEnum::SetPlayerUniqID,
			EventEnum::SetReadyRPC,
			EventEnum::SynhCommandRPC,
		};

		if (Menu::Misc::Bypass::Misc::AntiReport.value)
		{
			if ((eventEnum == EventEnum::SetNickName || eventEnum == EventEnum::SynhNameRPC) && parameters)
			{
				parameters->Set(0, IL2CPP::String::Create(DumpsterFire::gNameSpoofList[rand() % DumpsterFire::gNameSpoofList.size()]));
			}
			else
			{
				for (EventEnum _event : blockedEvent)
				{
					if (_event == eventEnum)
					{
						return;
					}
				}
			}
		}
		#endif

		if (!gLogRPC)
		{
			$CallOrig(PeerRPC, _this, photonView, eventEnum, target, player, encrypted, parameters);
			return;
		}

		const char* rpcName = rpcEntries[(int)eventEnum];
		LOG_INFO("===[PeerRPC]===");
		LOG_INFO("[PeerRPC] eventEnum: %s", rpcName);

		if (parameters == nullptr) {
			LOG_INFO("[PeerRPC] (eventEnum: %s) parameters is null", rpcName);
			$CallOrig(PeerRPC, _this, photonView, eventEnum, target, player, encrypted, parameters);
			return;
		}

		if (player != nullptr)
		{
			LOG_INFO("[PeerRPC] (eventEnum: %s) targetPlayer is not null.", rpcName);
		}
		else
		{
			LOG_INFO("[PeerRPC] (eventEnum: %s) photonTargets: %i", rpcName, target);
		}
		for (int i = 0; i < parameters->GetLength(); ++i) {

			if (parameters->GetVectorPointer()[i] == nullptr)
			{
				LOG_INFO("[PeerRPC] (eventEnum: %s) index %i is null", rpcName, i);
			}
			else
			{
				//const char *namespaceName = il2cpp_class_get_namespace(il2cpp_object_get_class((Il2CppObject *) parameters->vector[i]));
				//const char *className = il2cpp_class_get_name(il2cpp_object_get_class((Il2CppObject *) parameters->vector[i]));
				std::string typeName = parameters->Get(i)->GetClass()->GetFullName();//il2cpp_type_get_name(il2cpp_class_get_type(il2cpp_object_get_class((Il2CppObject*)parameters->vector[i])));

				LOG_INFO("[PeerRPC] (eventEnum: %s) index: %i", rpcName, i);
				LOG_INFO("[PeerRPC] (eventEnum: %s) typeName: %s", rpcName, typeName.c_str());

				if (strcmp(typeName.c_str(), "System.Single") == 0)
				{
					LOG_INFO("[PeerRPC] (eventEnum: %s) value: %f", rpcName, parameters->Get(i)->Unbox<float>());
				}
				else if (strcmp(typeName.c_str(), "System.Int32") == 0)
				{
					LOG_INFO("[PeerRPC] (eventEnum: %s) value: %i", rpcName, parameters->Get(i)->Unbox<int>());
				}
				else if (strcmp(typeName.c_str(), "System.Double") == 0)
				{
					LOG_INFO("[PeerRPC] (eventEnum: %s) value: %f", rpcName, parameters->Get(i)->Unbox<double>());
				}
				else if (strcmp(typeName.c_str(), "System.Boolean") == 0)
				{
					LOG_INFO("[PeerRPC] (eventEnum: %s) value: %i", rpcName, parameters->Get(i)->Unbox<bool>());
				}
				else if (strcmp(typeName.c_str(), "System.String") == 0)
				{
					LOG_INFO("[PeerRPC] (eventEnum: %s) value: %s", rpcName, reinterpret_cast<IL2CPP::String*>(parameters->Get(i))->ToString().c_str());
				}
			}
		}

		$CallOrig(PeerRPC, _this, photonView, eventEnum, target, player, encrypted, parameters);
	}

	$Hook(IL2CPP::String*, ProfileController_GetPlayerNameOrDefault, ())
	{
		auto gex = $CallOrig(ProfileController_GetPlayerNameOrDefault);

		#ifdef EXPRIMENTAL
		if (Menu::Misc::Bypass::Misc::AntiReport.value)
		{
			IL2CPP::String::Create(DumpsterFire::gNameSpoofList[rand() % DumpsterFire::gNameSpoofList.size()]);
		}
		#endif

		return gex;
	}

	void INIT()
	{
		using namespace IL2CPP::ClassMapping;

		ServerMods::RPC::AttractEveryone.OnClick(AttractEveryone);
		ServerMods::PrefabSpawner::SpawnBot.OnClick(SpawnBotPrefab);

		//ServerMods::World::CrashEveryone.OnClick([&]
		//{
		//	Global::ExecuteOnGameThread([]
		//	{
		//		#if !defined(EXPERIMENTAL)
		//		CrashEveryone();
		//		#endif
		//	});
		//});

		ServerMods::PrefabSpawner::SpawnProjectile.OnClick([&]
		{
			Global::ExecuteOnGameThread([]
			{
				SpamProjectiles();
			});
		});

		#ifdef EXPERIMENTAL
		ServerMods::World::SpawnPlayer.OnClick(HandlePlayerSpawning);
		#endif
		ServerMods::World::NoClipEveryone.OnClick([&] { processNoClipAll = true; });

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
			PlayerSynchStream_OnPhotonSerializeView,
			GetClass("PlayerSynchStream")->GetMethod(0x6)
		);

		$RegisterHook(
			BaseBot_OnPhotonSerializeView,
			GetClass("BaseBot")->GetMethod(0xcd)
		);

		$RegisterHook(
			FireRateModifier,
			GetClass("BalanceController")->GetMethod(0x64)
		);

		$RegisterHook(
			get_SpeedModifier,
			GetClass("ItemRecord")->GetMethod("get_SpeedModifier")
		);

		$RegisterHook(
			PhotonNetwork_Destroy,
			GetClass("PhotonNetwork")->GetMethod(0x7a)
		);

		$RegisterHook(
			InstantiatePrefab,
			GetClass("PhotonNetwork")->GetMethod(0x71)
		);

		$RegisterHook(
			PeerRPC,
			GetClass("NetworkingPeer")->GetMethod(0x71)
		);

		$RegisterHook(
			ProfileController_GetPlayerNameOrDefault,
			GetClass("ProfileController")->GetMethod(0x33)
		);
	}
}
