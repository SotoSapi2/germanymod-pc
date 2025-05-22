// Auto-generated with PointerFunctionsGen.py
#pragma once
#include <IL2CPP.hpp>
#include "Structs.hpp"
#include "../structures/PointerWrapper.hpp"


namespace Application 
{
	extern Pointer<bool()> IsFocused;
}

namespace Component
{
	extern Pointer<IL2CPP::Object* (IL2CPP::Object* component)> GetGameObject;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* component)> GetTransform;
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

	extern Pointer<void(int fullScreenMode)> SetfullScreenMode;
}

namespace Camera
{
	extern Pointer<IL2CPP::Object* ()> GetMain;

	extern Pointer<Vector3(IL2CPP::Object* camera, Vector3 vec)> WorldToScreenPoint;
}

namespace GameObject
{
	extern Pointer<IL2CPP::Array<IL2CPP::Object*>* (void* type)> FindObjectsOfType;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* obj, IL2CPP::String* componentname)> GetComponent;

	extern Pointer<IL2CPP::Array<IL2CPP::Object*>* (IL2CPP::Object* obj, void* type)> GetComponentsInChildren;

	extern Pointer<IL2CPP::String* (IL2CPP::Object* gameObject)> GetTag;

	extern Pointer<void(IL2CPP::Object* gameObject, IL2CPP::String* tag)> SetTag;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* gameObject)> GetTransform;

	extern Pointer<void(IL2CPP::Object* gameObject, IL2CPP::String* name)> SetName;

	extern Pointer<IL2CPP::String* (IL2CPP::Object* gameObject)> GetName;

	extern Pointer<int(IL2CPP::Object* gameObject)> GetInstanceID;
}

namespace Transform
{
	extern Pointer<Vector3(IL2CPP::Object* transform)> GetPosition;

	extern Pointer<void(IL2CPP::Object* transform, Vector3 pos)> SetPosition;

	extern Pointer<Quaternion(IL2CPP::Object* transform)> GetRotation;

	extern Pointer<void(IL2CPP::Object* transform, Quaternion rot)> SetRotation;

	extern Pointer<void(IL2CPP::Object* transform, Vector3, float angle)> Rotate;

	extern Pointer<void(IL2CPP::Object* transform, Vector3 eulers)> SetRotationFromEulers;

	extern Pointer<Vector3(IL2CPP::Object* transform)> GetFoward;

	extern Pointer<void(IL2CPP::Object* transform, IL2CPP::Object* target)> LookAt;

	extern Pointer<void(IL2CPP::Object* transform, Vector3 pos)> LookAtVec;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* transform)> GetParent;
}

namespace WeaponMenager
{
	extern Pointer<void* (IL2CPP::Object* obj, int level, int ItemRarity)> UpgradeV8Constructor;

	extern Pointer<void(IL2CPP::Object* instance, IL2CPP::String* name, int num, bool idfk1, bool idfk2, IL2CPP::Object* WeaponUpgradeV8, AnalyticsParamater params)> AddWeapon;

	extern Pointer<IL2CPP::List<IL2CPP::Object*>* (IL2CPP::Object* obj)> GetPlayerWeapons;

	extern Pointer<void(IL2CPP::Object* instance, int value)> SetClipAmmo;

	extern Pointer<IL2CPP::List<IL2CPP::Object*>* ()> GetItemRecordList;
}

namespace NetworkStartTable 
{
	extern Pointer<bool()> IsInMatch;

}

namespace InventoryItemType
{
	extern Pointer<IL2CPP::Object* (IL2CPP::Object* item, int index)> GetTypeFromIndex;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* item, OfferItemType offerItemType, IL2CPP::String* str)> GetTypeNoAmount;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* item, OfferItemType offerItemType, IL2CPP::String* str, int amount)> GetType;

	extern Pointer<int(IL2CPP::Object* type)> GetIndex;

	extern Pointer<IL2CPP::String* (IL2CPP::Object* type)> GetKey;

	extern Pointer<OfferItemType(IL2CPP::Object* type)> GetOfferItemType;
}

namespace WebSocketManager
{
	// UNSAFE!!! CAN TRHOW EXCEPTION! DO NOT USE.
	extern Pointer<IL2CPP::Object* (IL2CPP::Object* instance, IL2CPP::String* msgType, IL2CPP::Object* jsonMsg)> SendAsync;

	// UNSAFE!!! CAN TRHOW EXCEPTION! DO NOT USE.
	extern Pointer<int(IL2CPP::Object* instance, IL2CPP::String* msgType, IL2CPP::Object* jsonMsg)> Send;

	extern Pointer<int(IL2CPP::Object* instance, IL2CPP::String* msgType)> SendEvent;
}

namespace KeyClass
{
	// skidded from asteroid. logged param: 1500 1 0 "" 0 3 
	extern Pointer<bool(int amount, bool idk1, int purchaseEnum, IL2CPP::String* idk3, bool idk4, int obtainCause)> AddKey;
}

namespace ProgressUpdater
{
	extern Pointer<IL2CPP::Object* ()> GetInstance;

	extern Pointer<void(IL2CPP::Object* ths, IL2CPP::String* curName, int num, int num2, bool b1, bool b2, AnalyticsParamater params)> AddCurrency;

	extern Pointer<void(IL2CPP::Object* webInstance, int id, int level, IL2CPP::String* reason, AnalyticsParamater params)> BuyArmor;

	extern Pointer<void(IL2CPP::Object* webInstance, int chestId, int freeSpinEnum, int amount, int dumbEnum, AnalyticsParamater optional)> AddChest;

	extern Pointer<void(IL2CPP::Object* webInstance, int amountIg, IL2CPP::String* curName)> AddEventCurrency;

	extern Pointer<void(IL2CPP::Object* webInstance, int snapshotCause)> UpdateSnaphot;

	extern Pointer<void(IL2CPP::Object* webInstance, IL2CPP::Object* jsonDictionary)> SaveProgress;

	extern Pointer<void(IL2CPP::Object* webInstance, IL2CPP::Object* jsonDictionary)> SaveProgress2;

	extern Pointer<void(IL2CPP::Object* webInstance, CommandID cmdId, IL2CPP::Object* jsonDictionary, IL2CPP::Object* idfk, IL2CPP::Object* nullable) > SaveSnapshot;

	extern Pointer<void(IL2CPP::String* petString, int idfk)> AddPet;

	extern Pointer<void* (void* ptr, int offerItemType, IL2CPP::String* str, int index)> GetInvetoryItemType;

	extern Pointer<void* (void* ptr, int offerItemType, IL2CPP::String* str)> GetInvetoryItemTypeNoIndex;

	extern Pointer<bool(IL2CPP::Object* webInstance, void* inventoryItem, AnalyticsParamater sex)> AddInventoryItemType;

	extern Pointer<void(IL2CPP::Object* webInstance, IL2CPP::Object* moduleData)> UpgradeModule;

	extern Pointer<void(IL2CPP::String* item, bool idfk, AnalyticsParamater params)> AddRoyale;

	extern Pointer<void(IL2CPP::Object* webInstance, IL2CPP::Object* killRateObject)> AddCommonWin;

	extern Pointer<void(IL2CPP::Object* webInstance, long curTick, bool isWin)> SendBattleStatistic;

	extern Pointer<void(IL2CPP::Object* webInstance, int rank, int xp)> SetClanRank;

	extern Pointer<bool(IL2CPP::Object* webInstance, IL2CPP::Object* item, AnalyticsParamater sex)> AddInventorySingleItem;

	extern Pointer<bool(IL2CPP::Object* webInstance, IL2CPP::Object* item)> RemoveInventorySingleItem;

	extern Pointer<bool(IL2CPP::Object* webInstance, int categoryNames, IL2CPP::String* str)> BuyWear;

	extern Pointer<bool(IL2CPP::Object* webInstance)> BuyPixelPass;

	extern Pointer<bool(IL2CPP::Object* webInstance)> ComplateElitePixelPass;

	extern Pointer<void(IL2CPP::Object* webInstance, int value)> AddPixelPassExp;

	extern Pointer<void(IL2CPP::Object* webInstance, long idk, IL2CPP::String* skinName, IL2CPP::String* base64data)> UpdateCustomSkin;

	extern Pointer<void(IL2CPP::Object* webInstance, int idk, IL2CPP::String* key) > RemoveGadget;
}

namespace GameConnect
{
	extern Pointer<int()> CurrentGamemode;

	extern Pointer<bool(int gamemodeEnum)> Idk;
}

namespace AuthManager
{
	extern Pointer<IL2CPP::String* ()> GetPlayerID;
}

namespace ModuleData
{
	extern Pointer<int(IL2CPP::Object* x)> GetLevel;
}

namespace ContentKeyRegister
{
	extern Pointer<IL2CPP::Object* ()> GetInstance;

	extern Pointer<IL2CPP::List<IL2CPP::String*>* (IL2CPP::Object* instance, OfferItemType type)> GetRegisterList;

	extern Pointer<int(IL2CPP::Object* instance, OfferItemType type, IL2CPP::String* key)> GetItemIndex;
}

namespace ExperienceController
{
	extern Pointer<int(IL2CPP::Object* experienceController)> GetCurrentLevel;

	extern Pointer<void(IL2CPP::Object* experienceController, int value, ExpObtainCause reason, IL2CPP::Object* dictionary, IL2CPP::Object* sex)> AddExperience;
}

namespace LobbyGiverClass
{
	extern Pointer<bool(IL2CPP::String* str)> AddLobbyItem;
}

namespace FriendsController
{
	extern Pointer<void(IL2CPP::Object* friendsController, bool winning)> SyncRatingAndRoundWon;
}

namespace GadgetManager
{
	extern Pointer<void(IL2CPP::String* name, int num)> ProvideGadget;
}

namespace WearClass
{
	extern Pointer<void* (CategoryNames categoryName, IL2CPP::String* str)> AddWear;

	extern Pointer<int(IL2CPP::String* id)> WearIndex;
}

namespace RoomInfo
{
	extern Pointer<IL2CPP::Object* (IL2CPP::Object* room)> GetHashtable;
}

namespace PhotonHashtable
{
	extern Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable)> Constructor;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable, IL2CPP::Object* key)> Get;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable, IL2CPP::Object* key, IL2CPP::Object* val)> Set;
};

namespace PhotonNetwork
{
	extern Pointer<IL2CPP::Object* ()> GetCurrentRoom;

	extern Pointer<IL2CPP::Object* ()> GetLocalPlayer;

	extern Pointer<bool()> IsMasterClient;

	extern Pointer<void(IL2CPP::Object* photonPlayer)> SetMasterClient;

	extern Pointer<IL2CPP::Object* (IL2CPP::String* prefab, Vector3 vec, Quaternion rot, char byte, IL2CPP::Array<IL2CPP::Object*>* settings)> InstantiatePrefab;
}

namespace MatchSettings
{
	extern Pointer<IL2CPP::Array<IL2CPP::Object*>* (IL2CPP::String* json)> CreateSettings;
}

namespace PlayerMoveC
{
	extern Pointer<void(IL2CPP::Object* plrMoveC, bool active)> SetXrayShader;

	extern Pointer<void(IL2CPP::Object* plrMoveC)> ShotPressed;

	extern Pointer<void(IL2CPP::Object* plrMoveC, IL2CPP::String* message, bool isClan, IL2CPP::String* icon)> SendChat;
}

namespace PlayerDamageable
{
	extern Pointer<bool(IL2CPP::Object* player)> IsDead;

	extern Pointer<bool(IL2CPP::Object* player, IL2CPP::Object* target)> IsEnemyTo;
}

namespace PhotonView
{
	extern Pointer<void(IL2CPP::Object* instance, EventEnum eventEnum, PhotonTargets photonTarget, void* param)> RPC;

	extern Pointer<bool(IL2CPP::Object* instance)> IsMine;
}

namespace PixelView
{
	extern Pointer<int(IL2CPP::Object* instance)> GetViewID;
}

namespace TutorialClass
{
	extern Pointer<IL2CPP::Object* ()> GetInstance;

	extern Pointer<void(IL2CPP::Object* instance)> Fill;
}

namespace TrainingController
{
	extern Pointer<void()> FinishTraining;
}

namespace Encoding
{
	extern Pointer<IL2CPP::Object* ()> GetUTF8;

	extern Pointer<IL2CPP::Array<char>* (IL2CPP::Object* encoder, IL2CPP::String* string)> GetBytes;
}

namespace Type
{
	extern Pointer<IL2CPP::Object* (IL2CPP::Object* type, IL2CPP::String* name)> GetMethod;

	extern Pointer<IL2CPP::Object* (IL2CPP::String* typeName)> GetType;

	extern Pointer<IL2CPP::Object* (IL2CPP::Object* base, IL2CPP::Array<IL2CPP::Object*>* generics)> MakeGenericType;
}

namespace Activator
{
	extern Pointer<IL2CPP::Object* (IL2CPP::Object* type)> CreateInstance;
}

namespace MethodBase
{
	extern Pointer<IL2CPP::Object* (IL2CPP::Object* info, IL2CPP::Object* instance, IL2CPP::Array<IL2CPP::Object*>* args)> Invoke;
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
		Screen::SetfullScreenMode.Define();

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
		Transform::Rotate.Define();
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

		// NetworkStartTable
		NetworkStartTable::IsInMatch.Define();

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

		// PhotonHashtable
		PhotonHashtable::Constructor.Define();
		PhotonHashtable::Get.Define();
		PhotonHashtable::Set.Define();

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
		Encoding::GetBytes.Define();

		// Type
		Type::GetMethod.Define();
		Type::GetType.Define();
		Type::MakeGenericType.Define();

		// Activator
		Activator::CreateInstance.Define();

		// MethodBase
		MethodBase::Invoke.Define();

	}
}