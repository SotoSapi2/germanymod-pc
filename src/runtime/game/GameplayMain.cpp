#pragma once
#include "GameplayMain.hpp"
#include "Functions.hpp"
#include "util/FieldWrapper.hpp"
#include "data/ClassFinder.hpp"
#include "data/PointerFunctions.hpp"
#include "../util/HookingUtil.hpp"
#include "../framework/UIManager.hpp"

namespace GameplayMain
{
	Il2CppObject* myPlayerMoveC = nullptr;
	MonoList<Il2CppObject*>* playerMoveCList = nullptr;

	// Aim
	bool isTriggerbotAtive = false;
	bool isAimbotActive = false;
	bool aimAtHead = false;
	float aimbotSmoothness = 0.0f;
	float aimbotFov = 180.0f;

	void HandleAimbot()
	{
		float deltaTime = Time::DeltaTime();
		Vector3 screenCenter(Screen::GetWidth() / 2, Screen::GetHeight() / 2, 0);
		Il2CppObject* cam = Camera::GetMain();
		Il2CppObject* camTransform = Component::GetTransform(cam);
		Vector3 camPos = Transform::GetPosition(camTransform);

		Il2CppObject* myPlrTransform = PlayerMoveC::GetTransform(myPlayerMoveC);
		Vector3 myPos = Transform::GetPosition(myPlrTransform);

		Il2CppObject* targetTransform = nullptr;
		float minDistance = FLT_MAX;

		playerMoveCList->foreach([&](Il2CppObject* player)
		{
			if (player == nullptr) return;
			Vector3 pos = PlayerMoveC::GetPosition(player);
			Vector3 plrScreenPos = Camera::WorldToScreenPoint(cam, pos);
			Il2CppObject* bodyCollider = Field<Il2CppObject*>(player, "_bodyAimCollider").Get();
			Il2CppObject* headCollider = Field<Il2CppObject*>(player, "_headAimCollider").Get();

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
				&& PlayerMoveC::IsEnemyTo(myPlayerMoveC, player) && plrScreenPos.Z > 0 && isVisible)
			{
				minDistance = distance;
				targetTransform = PlayerMoveC::GetTransform(player);
			}
		});

		if (targetTransform != nullptr && minDistance < aimbotFov)
		{
			Vector3 aimOffset;
			if (aimAtHead)
			{
				aimOffset = Vector3(0, 0.55, 0);
			}

			Vector3 targetPos = Transform::GetPosition(targetTransform);
			Vector3 camDirVec = Vector3::Normalized((targetPos + aimOffset) - camPos) * Time::DeltaTime();
			Vector3 myPlayerDirVec = Vector3::Normalized((targetPos + aimOffset) - myPos) * Time::DeltaTime();

			float smooth = 1.0f - (1.0f * aimbotSmoothness);

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

	$Hook(void, WeaponManager, (Il2CppObject* _this))
	{
		myPlayerMoveC = Field<Il2CppObject*>(_this, "myPlayerMoveC").Get();
		playerMoveCList = Il2CppUtils::GetStaticFieldByIndex<MonoList<Il2CppObject*>*>(GetClass("PlayerListClass"), 0x0);

		if (myPlayerMoveC != nullptr)
		{
			if (isAimbotActive && playerMoveCList != nullptr)
			{
				HandleAimbot();
			}
		}

		$CallOrig(WeaponManager, _this);
	}

	$Hook(void, AimCrosshairController, (Il2CppObject* _this))
	{
		auto aimColor = Field<Color>(_this, 0x27).Get();
		if (isTriggerbotAtive && aimColor.r == 1 && aimColor.g == 0 && aimColor.b == 0)
		{
			PlayerMoveC::ShotPressed(myPlayerMoveC);
		}

		$CallOrig(AimCrosshairController, _this);
	}

	void UIUpdate()
	{
		ImGui::Checkbox("Triggerbot", &isTriggerbotAtive);
		ImGui::Checkbox("Aimbot", &isAimbotActive);
		ImGui::Checkbox("Aim at head", &aimAtHead);
		ImGui::SliderFloat("Smoothness", &aimbotSmoothness, 0, 1);
		ImGui::SliderFloat("Fov", &aimbotFov, 0, 1000);
	}

	void INIT()
	{
		$RegisterHook(WeaponManager, Il2CppUtils::GetMethodPointerByName(
			GetClass("WeaponManager"),
			"Update"
		));

		$RegisterHook(AimCrosshairController, Il2CppUtils::GetMethodPointerByName(
			GetClass("AimCrosshairController"),
			"LateUpdate"
		));

		UIManager::RegisterUIUpdate(UIUpdate);
	}
}