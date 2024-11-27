// Auto-generated with PointerFunctionsGen.py

#pragma once
#include "Structs.hpp"
#include "../util/PointerWrapper.hpp"

namespace Application 
{
	extern Pointer<bool()> IsFocused;
}

namespace Component
{
	extern Pointer<Il2CppObject* (Il2CppObject* component)> GetGameObject;

	extern Pointer<Il2CppObject* (Il2CppObject* component)> GetTransform;
}

namespace Time
{
	extern Pointer<float()> DeltaTime;

	extern Pointer<float()> GetTime;
}

namespace Physics
{
	extern Pointer<bool(Ray ray, RaycastHit* info, float maxDistance)> Raycast;
}

namespace Screen
{
	extern Pointer<int()> GetWidth;

	extern Pointer<int()> GetHeight;
}

namespace Camera
{
	extern Pointer<Il2CppObject* ()> GetMain;

	extern Pointer<Vector3(Il2CppObject* camera, Vector3 vec)> WorldToScreenPoint;
}

namespace GameObject
{
	extern Pointer<MonoArray<Il2CppObject*>* (void* type)> FindObjectsOfType;

	extern Pointer<Il2CppObject* (Il2CppObject* obj, MonoString* componentname)> GetComponent;

	extern Pointer<MonoArray<Il2CppObject*>* (Il2CppObject* obj, void* type)> GetComponentsInChildren;

	extern Pointer<MonoString* (Il2CppObject* gameObject)> GetTag;

	extern Pointer<void(Il2CppObject* gameObject, MonoString* tag)> SetTag;

	extern Pointer<Il2CppObject* (Il2CppObject* gameObject)> GetTransform;

	extern Pointer<void(Il2CppObject* gameObject, MonoString* name)> SetName;

	extern Pointer<MonoString* (Il2CppObject* gameObject)> GetName;

	extern Pointer<int(Il2CppObject* gameObject)> GetInstanceID;
}

namespace Transform
{
	extern Pointer<Vector3(Il2CppObject* transform)> GetPosition;

	extern Pointer<void(Il2CppObject* transform, Vector3 pos)> SetPosition;

	extern Pointer<Quaternion(Il2CppObject* transform)> GetRotation;

	extern Pointer<void(Il2CppObject* transform, Quaternion rot)> SetRotation;

	extern Pointer<void(Il2CppObject* transform, Vector3 eulers)> SetRotationFromEulers;

	extern Pointer<Vector3(Il2CppObject* transform)> GetFoward;

	extern Pointer<void(Il2CppObject* transform, Il2CppObject* target)> LookAt;

	extern Pointer<void(Il2CppObject* transform, Vector3 pos)> LookAtVec;

	extern Pointer<Il2CppObject* (Il2CppObject* transform)> GetParent;
}

namespace WeaponMenager
{
	extern Pointer<void* (Il2CppObject* obj, int level, int ItemRarity)> UpgradeV8Constructor;

	extern Pointer<void(Il2CppObject* instance, MonoString* name, int num, bool idfk1, bool idfk2, Il2CppObject* WeaponUpgradeV8, AnalyticsParamater params)> AddWeapon;

	extern Pointer<MonoList<Il2CppObject*>* (Il2CppObject* obj)> GetPlayerWeapons;

	extern Pointer<void(Il2CppObject* instance, int value)> SetClipAmmo;

	extern Pointer<MonoList<Il2CppObject*>* ()> GetItemRecordList;
}

namespace InventoryItemType
{
	extern Pointer<Il2CppObject* (Il2CppObject* item, int index)> GetTypeFromIndex;

	extern Pointer<Il2CppObject* (Il2CppObject* item, OfferItemType offerItemType, MonoString* str)> GetTypeNoAmount;

	extern Pointer<Il2CppObject* (Il2CppObject* item, OfferItemType offerItemType, MonoString* str, int amount)> GetType;

	extern Pointer<int(Il2CppObject* type)> GetIndex;

	extern Pointer<MonoString* (Il2CppObject* type)> GetKey;

	extern Pointer<OfferItemType(Il2CppObject* type)> GetOfferItemType;
}

namespace WebSocketManager
{
	extern Pointer<Il2CppObject* (Il2CppObject* instance, MonoString* msgType, Il2CppObject* jsonMsg)> SendAsync;

	extern Pointer<int(Il2CppObject* instance, MonoString* msgType, Il2CppObject* jsonMsg)> Send;

	extern Pointer<int(Il2CppObject* instance, MonoString* msgType)> SendEvent;
}

namespace KeyClass
{
	// skidded from asteroid. logged param: 1500 1 0 "" 0 3 
	extern Pointer<bool(int amount, bool idk1, int purchaseEnum, MonoString* idk3, bool idk4, int obtainCause)> AddKey;
}

namespace ProgressUpdater
{
	extern Pointer<Il2CppObject* ()> GetInstance;

	extern Pointer<void(Il2CppObject* ths, MonoString* curName, int num, int num2, bool b1, bool b2, AnalyticsParamater params)> AddCurrency;

	extern Pointer<void(Il2CppObject* webInstance, int id, int level, MonoString* reason, AnalyticsParamater params)> BuyArmor;

	extern Pointer<void(Il2CppObject* webInstance, int chestId, int freeSpinEnum, int amount, int dumbEnum, AnalyticsParamater optional)> AddChest;

	extern Pointer<void(Il2CppObject* webInstance, int amountIg, MonoString* curName)> AddEventCurrency;

	extern Pointer<void(Il2CppObject* webInstance, int snapshotCause)> UpdateSnaphot;

	extern Pointer<void(Il2CppObject* webInstance, Il2CppObject* jsonDictionary)> SaveProgress;

	extern Pointer<void(Il2CppObject* webInstance, Il2CppObject* jsonDictionary)> SaveProgress2;

	extern Pointer<void(Il2CppObject* webInstance, CommandID cmdId, Il2CppObject* jsonDictionary, Il2CppObject* idfk, Il2CppObject* nullable) > SaveSnapshot;

	extern Pointer<void(MonoString* petString, int idfk)> AddPet;

	extern Pointer<void* (void* ptr, int offerItemType, MonoString* str, int index)> GetInvetoryItemType;

	extern Pointer<void* (void* ptr, int offerItemType, MonoString* str)> GetInvetoryItemTypeNoIndex;

	extern Pointer<bool(Il2CppObject* webInstance, void* inventoryItem, AnalyticsParamater sex)> AddInventoryItemType;

	extern Pointer<void(Il2CppObject* webInstance, Il2CppObject* moduleData)> UpgradeModule;

	extern Pointer<void(MonoString* item, bool idfk, AnalyticsParamater params)> AddRoyale;

	extern Pointer<void(Il2CppObject* webInstance, Il2CppObject* killRateObject)> AddCommonWin;

	extern Pointer<void(Il2CppObject* webInstance, long curTick, bool isWin)> SendBattleStatistic;

	extern Pointer<void(Il2CppObject* webInstance, int rank, int xp)> SetClanRank;

	extern Pointer<bool(Il2CppObject* webInstance, Il2CppObject* item, AnalyticsParamater sex)> AddInventorySingleItem;

	extern Pointer<bool(Il2CppObject* webInstance, Il2CppObject* item)> RemoveInventorySingleItem;

	extern Pointer<bool(Il2CppObject* webInstance, int categoryNames, MonoString* str)> BuyWear;

	extern Pointer<bool(Il2CppObject* webInstance)> BuyPixelPass;

	extern Pointer<bool(Il2CppObject* webInstance)> ComplateElitePixelPass;

	extern Pointer<void(Il2CppObject* webInstance, int value)> AddPixelPassExp;

	extern Pointer<void(Il2CppObject* webInstance, long idk, MonoString* skinName, MonoString* base64data)> UpdateCustomSkin;

	extern Pointer<void(Il2CppObject* webInstance, int idk, MonoString* key) > RemoveGadget;
}

namespace GameConnect
{
	extern Pointer<int()> CurrentGamemode;

	extern Pointer<bool(int gamemodeEnum)> Idk;
}

namespace AuthManager
{
	extern Pointer<MonoString* ()> GetPlayerID;
}

namespace ModuleData
{
	extern Pointer<int(Il2CppObject* x)> GetLevel;
}

namespace ContentKeyRegister
{
	extern Pointer<Il2CppObject* ()> GetInstance;

	extern Pointer<MonoList<MonoString*>* (Il2CppObject* instance, OfferItemType type)> GetRegisterList;

	extern Pointer<int(Il2CppObject* instance, OfferItemType type, MonoString* key)> GetItemIndex;
}

namespace ExperienceController
{
	extern Pointer<int(Il2CppObject* experienceController)> GetCurrentLevel;

	extern Pointer<void(Il2CppObject* experienceController, int value, int reason, Il2CppObject* dictionary, Il2CppObject* sex)> AddExperience;
}

namespace LobbyGiverClass
{
	extern Pointer<bool(MonoString* str)> AddLobbyItem;
}

namespace FriendsController
{
	extern Pointer<void(Il2CppObject* friendsController, bool winning)> SyncRatingAndRoundWon;
}

namespace GadgetManager
{
	extern Pointer<void(MonoString* name, int num)> ProvideGadget;
}

namespace WearClass
{
	extern Pointer<void* (int _enum, MonoString* str)> AddWear;

	extern Pointer<int(MonoString* id)> WearIndex;
}

namespace RoomInfo
{
	extern Pointer<Il2CppObject* (Il2CppObject* room)> GetHashtable;
}

namespace PhotonNetwork
{
	extern Pointer<Il2CppObject* ()> GetCurrentRoom;

	extern Pointer<Il2CppObject* ()> GetLocalPlayer;

	extern Pointer<bool()> IsMasterClient;

	extern Pointer<void(Il2CppObject* photonPlayer)> SetMasterClient;

	extern Pointer<Il2CppObject* (MonoString* prefab, Vector3 vec, Quaternion rot, char byte, MonoArray<Il2CppObject*>* settings)> InstantiatePrefab;
}

namespace MatchSettings
{
	extern Pointer<MonoArray<Il2CppObject*>* (MonoString* json)> CreateSettings;
}

namespace PlayerMoveC
{
	extern Pointer<void(Il2CppObject* plrMoveC, bool active)> SetXrayShader;

	extern Pointer<void(Il2CppObject* plrMoveC)> ShotPressed;

	extern Pointer<void(Il2CppObject* plrMoveC, MonoString* message, bool isClan, MonoString* icon)> SendChat;
}

namespace PlayerDamageable
{
	extern Pointer<bool(Il2CppObject* player)> IsDead;

	extern Pointer<bool(Il2CppObject* player, Il2CppObject* target)> IsEnemyTo;
}

namespace PhotonView
{
	extern Pointer<void(Il2CppObject* instance, int eventEnum, int photonTarget, void* param)> RPC;

	extern Pointer<bool(Il2CppObject* instance)> IsMine;
}

namespace PixelView
{
	extern Pointer<int(Il2CppObject* instance)> GetViewID;
}

namespace TutorialClass
{
	extern Pointer<Il2CppObject* ()> GetInstance;

	extern Pointer<void(Il2CppObject* instance)> Fill;
}

namespace TrainingController
{
	extern Pointer<void()> FinishTraining;
}

namespace Encoding
{
	extern Pointer<Il2CppObject* (MonoString* jsonString)> GetUTF8;
}

namespace MiniJson
{
	extern Pointer<Il2CppObject* (MonoString* jsonString)> Deserialize;

	extern Pointer<MonoString* (Il2CppObject* jsonObject)> Serialize;
}

namespace UIButton
{
	extern Pointer<void(Il2CppObject* uiButtonComponent)> OnClick;
}

namespace Type
{
	extern Pointer<Il2CppObject* (Il2CppObject* type, MonoString* name)> GetMethod;

	extern Pointer<Il2CppObject* (MonoString* typeName)> GetType;

	extern Pointer<Il2CppObject* (Il2CppObject* obj)> GetObjectType;

	extern Pointer<Il2CppObject* (Il2CppObject* base, Il2CppArray* generics)> MakeGenericType;
}

namespace Activator
{
	extern Pointer<Il2CppObject* (Il2CppObject* type)> CreateInstance;
}

namespace MethodBase
{
	extern Pointer<Il2CppObject* (Il2CppObject* info, Il2CppObject* instance, Il2CppArray* args)> Invoke;
}

namespace PointerFunctions
{
	static void INIT()
	{
		// Application
		Application::IsFocused.Define();

		// Component
		Component::GetGameObject.Define();
		Component::GetTransform.Define();

		// Time
		Time::DeltaTime.Define();
		Time::GetTime.Define();

		// Physics
		Physics::Raycast.Define();

		// Screen
		Screen::GetWidth.Define();
		Screen::GetHeight.Define();

		// Camera
		Camera::GetMain.Define();
		Camera::WorldToScreenPoint.Define();

		// GameObject
		GameObject::FindObjectsOfType.Define();
		GameObject::GetComponent.Define();
		GameObject::GetComponentsInChildren.Define();
		GameObject::GetTag.Define();
		GameObject::SetTag.Define();
		GameObject::GetTransform.Define();
		GameObject::SetName.Define();
		GameObject::GetName.Define();
		GameObject::GetInstanceID.Define();

		// Transform
		Transform::GetPosition.Define();
		Transform::SetPosition.Define();
		Transform::GetRotation.Define();
		Transform::SetRotation.Define();
		Transform::SetRotationFromEulers.Define();
		Transform::GetFoward.Define();
		Transform::LookAt.Define();
		Transform::LookAtVec.Define();
		Transform::GetParent.Define();

		// WeaponMenager
		WeaponMenager::UpgradeV8Constructor.Define();
		WeaponMenager::AddWeapon.Define();
		WeaponMenager::GetPlayerWeapons.Define();
		WeaponMenager::SetClipAmmo.Define();
		WeaponMenager::GetItemRecordList.Define();

		// InventoryItemType
		InventoryItemType::GetTypeFromIndex.Define();
		InventoryItemType::GetTypeNoAmount.Define();
		InventoryItemType::GetType.Define();
		InventoryItemType::GetIndex.Define();
		InventoryItemType::GetKey.Define();
		InventoryItemType::GetOfferItemType.Define();

		// WebSocketManager
		WebSocketManager::SendAsync.Define();
		WebSocketManager::Send.Define();
		WebSocketManager::SendEvent.Define();

		// KeyClass
		KeyClass::AddKey.Define();

		// ProgressUpdater
		ProgressUpdater::GetInstance.Define();
		ProgressUpdater::AddCurrency.Define();
		ProgressUpdater::BuyArmor.Define();
		ProgressUpdater::AddChest.Define();
		ProgressUpdater::AddEventCurrency.Define();
		ProgressUpdater::UpdateSnaphot.Define();
		ProgressUpdater::SaveProgress.Define();
		ProgressUpdater::SaveProgress2.Define();
		ProgressUpdater::SaveSnapshot.Define();
		ProgressUpdater::AddPet.Define();
		ProgressUpdater::GetInvetoryItemType.Define();
		ProgressUpdater::GetInvetoryItemTypeNoIndex.Define();
		ProgressUpdater::AddInventoryItemType.Define();
		ProgressUpdater::UpgradeModule.Define();
		ProgressUpdater::AddRoyale.Define();
		ProgressUpdater::AddCommonWin.Define();
		ProgressUpdater::SendBattleStatistic.Define();
		ProgressUpdater::SetClanRank.Define();
		ProgressUpdater::AddInventorySingleItem.Define();
		ProgressUpdater::RemoveInventorySingleItem.Define();
		ProgressUpdater::BuyWear.Define();
		ProgressUpdater::BuyPixelPass.Define();
		ProgressUpdater::ComplateElitePixelPass.Define();
		ProgressUpdater::AddPixelPassExp.Define();
		ProgressUpdater::UpdateCustomSkin.Define();
		ProgressUpdater::RemoveGadget.Define();

		// GameConnect
		GameConnect::CurrentGamemode.Define();
		GameConnect::Idk.Define();

		// AuthManager
		AuthManager::GetPlayerID.Define();

		// ModuleData
		ModuleData::GetLevel.Define();

		// ContentKeyRegister
		ContentKeyRegister::GetInstance.Define();
		ContentKeyRegister::GetRegisterList.Define();
		ContentKeyRegister::GetItemIndex.Define();

		// ExperienceController
		ExperienceController::GetCurrentLevel.Define();
		ExperienceController::AddExperience.Define();

		// LobbyGiverClass
		LobbyGiverClass::AddLobbyItem.Define();

		// FriendsController
		FriendsController::SyncRatingAndRoundWon.Define();

		// GadgetManager
		GadgetManager::ProvideGadget.Define();

		// WearClass
		WearClass::AddWear.Define();
		WearClass::WearIndex.Define();

		// RoomInfo
		RoomInfo::GetHashtable.Define();

		// PhotonNetwork
		PhotonNetwork::GetCurrentRoom.Define();
		PhotonNetwork::GetLocalPlayer.Define();
		PhotonNetwork::IsMasterClient.Define();
		PhotonNetwork::SetMasterClient.Define();
		PhotonNetwork::InstantiatePrefab.Define();

		// MatchSettings
		MatchSettings::CreateSettings.Define();

		// PlayerMoveC
		PlayerMoveC::SetXrayShader.Define();
		PlayerMoveC::ShotPressed.Define();
		PlayerMoveC::SendChat.Define();

		// PlayerDamageable
		PlayerDamageable::IsDead.Define();
		PlayerDamageable::IsEnemyTo.Define();

		// PhotonView
		PhotonView::RPC.Define();
		PhotonView::IsMine.Define();

		// PixelView
		PixelView::GetViewID.Define();

		// TutorialClass
		TutorialClass::GetInstance.Define();
		TutorialClass::Fill.Define();

		// TrainingController
		TrainingController::FinishTraining.Define();

		// Encoding
		Encoding::GetUTF8.Define();

		// MiniJson
		MiniJson::Deserialize.Define();
		MiniJson::Serialize.Define();

		// UIButton
		UIButton::OnClick.Define();

		// Type
		Type::GetMethod.Define();
		Type::GetType.Define();
		Type::GetObjectType.Define();
		Type::MakeGenericType.Define();

		// Activator
		Activator::CreateInstance.Define();

		// MethodBase
		MethodBase::Invoke.Define();

	}
}