#include "PointerFunctions.hpp"

namespace Application 
{
	Pointer<bool()> IsFocused(
		"Application",
		"get_isFocused"
	);
}

namespace Component
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* component)> GetGameObject(
		"Component",
		"get_gameObject"
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* component)> GetTransform(
		"Component",
		"get_transform"
	);
}

namespace Time
{
	Pointer<float()> DeltaTime(
		"Time",
		"get_deltaTime"
	);

	Pointer<float()> GetTime(
		"Time",
		"get_time"
	);
}

namespace Physics
{
	Pointer<bool(Ray ray, RaycastHit* info, float maxDistance)> Raycast(
		"Physics",
		0x15
	);
}

namespace Screen
{
	Pointer<int()> GetWidth(
		"Screen",
		"get_width"
	);

	Pointer<int()> GetHeight(
		"Screen",
		"get_height"
	);

	Pointer<void(int fullScreenMode)> SetfullScreenMode(
		"Screen",
		"set_fullScreenMode"
	);
}

namespace Camera
{
	Pointer<IL2CPP::Object* ()> GetMain(
		"Camera",
		"get_main"
	);

	Pointer<Vector3(IL2CPP::Object* camera, Vector3 vec)> WorldToScreenPoint(
		"Camera",
		0x38
	);
}

namespace GameObject
{
	Pointer<IL2CPP::Array<IL2CPP::Object*>* (void* type)> FindObjectsOfType(
		"Object",
		0x17
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* obj, IL2CPP::String* componentname)> GetComponent(
		"GameObject",
		0x5
	);

	Pointer<IL2CPP::Array<IL2CPP::Object*>* (IL2CPP::Object* obj, void* type)> GetComponentsInChildren(
		"GameObject",
		0x10
	);

	Pointer<IL2CPP::String* (IL2CPP::Object* gameObject)> GetTag(
		"GameObject",
		"get_tag"
	);

	Pointer<void(IL2CPP::Object* gameObject, IL2CPP::String* tag)> SetTag(
		"GameObject",
		"set_tag"
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* gameObject)> GetTransform(
		"GameObject",
		"get_transform"
	);

	Pointer<void(IL2CPP::Object* gameObject, IL2CPP::String* name)> SetName(
		"Object",
		"SetName"
	);

	Pointer<IL2CPP::String* (IL2CPP::Object* gameObject)> GetName(
		"Object",
		"GetName"
	);

	Pointer<int(IL2CPP::Object* gameObject)> GetInstanceID(
		"Object",
		0x0
	);
}

namespace Transform
{
	Pointer<Vector3(IL2CPP::Object* transform)> GetPosition(
		"Transform",
		"get_position"
	);

	Pointer<void(IL2CPP::Object* transform, Vector3 pos)> SetPosition(
		"Transform",
		"set_position"
	);

	Pointer<Quaternion(IL2CPP::Object* transform)> GetRotation(
		"Transform",
		"get_rotation"
	);

	Pointer<void(IL2CPP::Object* transform, Quaternion rot)> SetRotation(
		"Transform",
		"set_rotation"
	);

	Pointer<void(IL2CPP::Object* transform, Vector3, float angle)> Rotate(
		"Transform",
		0x26
	);

	Pointer<void(IL2CPP::Object* transform, Vector3 eulers)> SetRotationFromEulers(
		"Transform",
		0x21
	);

	Pointer<Vector3(IL2CPP::Object* transform)> GetFoward(
		"Transform",
		"get_forward"
	);

	Pointer<void(IL2CPP::Object* transform, IL2CPP::Object* target)> LookAt(
		"Transform",
		"LookAt"
	);

	Pointer<void(IL2CPP::Object* transform, Vector3 pos)> LookAtVec(
		"Transform",
		0x2a
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* transform)> GetParent(
		"Transform",
		0x14
	);
}

namespace WeaponMenager
{
	Pointer<void* (IL2CPP::Object* obj, int level, int ItemRarity)> UpgradeV8Constructor(
		"WeaponUpgradeV8",
		{ "internal", "Void", ".ctor", {"Int32", "ENUM"} }
	);

	Pointer<void(IL2CPP::Object* instance, IL2CPP::String* name, int num, bool idfk1, bool idfk2, IL2CPP::Object* WeaponUpgradeV8, AnalyticsParamater params)> AddWeapon(
		"WeaponManager",
		{ "internal", "Void", nullptr, {"String", "ENUM", "Boolean", "Boolean", nullptr, nullptr} }
	);

	Pointer<IL2CPP::List<IL2CPP::Object*>* (IL2CPP::Object* obj)> GetPlayerWeapons(
		"WeaponManager",
		{ "internal", "Void", nullptr, {nullptr, "Boolean", "Boolean"} },
		5
	);

	Pointer<void(IL2CPP::Object* instance, int value)> SetClipAmmo(
		"WeaponW",
		0x5
	);

	Pointer<IL2CPP::List<IL2CPP::Object*>* ()> GetItemRecordList(
		"ItemDb",
		0x6
	);
}

namespace NetworkStartTable 
{
	Pointer<bool()> IsInMatch(
		"IdkBroClass",
		0xc
	);

}

namespace InventoryItemType
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* item, int index)> GetTypeFromIndex(
		"InventoryItemType",
		0x0
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* item, OfferItemType offerItemType, IL2CPP::String* str)> GetTypeNoAmount(
		"InventoryItemType",
		0x2
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* item, OfferItemType offerItemType, IL2CPP::String* str, int amount)> GetType(
		"InventoryItemType",
		0x5
	);

	Pointer<int(IL2CPP::Object* type)> GetIndex(
		"InventoryItemType",
		0x6
	);

	Pointer<IL2CPP::String* (IL2CPP::Object* type)> GetKey(
		"InventoryItemType",
		0x9
	);

	Pointer<OfferItemType(IL2CPP::Object* type)> GetOfferItemType(
		"InventoryItemType",
		0x8
	);
}

namespace WebSocketManager
{
	// UNSAFE!!! CAN TRHOW EXCEPTION! DO NOT USE.
	Pointer<IL2CPP::Object* (IL2CPP::Object* instance, IL2CPP::String* msgType, IL2CPP::Object* jsonMsg)> SendAsync(
		"WebSocketManager",
		0x10
	);

	// UNSAFE!!! CAN TRHOW EXCEPTION! DO NOT USE.
	Pointer<int(IL2CPP::Object* instance, IL2CPP::String* msgType, IL2CPP::Object* jsonMsg)> Send(
		"WebSocketManager",
		0x16
	);

	Pointer<int(IL2CPP::Object* instance, IL2CPP::String* msgType)> SendEvent(
		"WebSocketManager",
		0x15
	);
}

namespace KeyClass
{
	// skidded from asteroid. logged param: 1500 1 0 "" 0 3 
	Pointer<bool(int amount, bool idk1, int purchaseEnum, IL2CPP::String* idk3, bool idk4, int obtainCause)> AddKey(
		"KeyClass",
		{ "internal static", "Boolean", nullptr, {"Int32", "Boolean", "ENUM", "String", "Boolean", "ENUM"} }
	);
}

namespace ProgressUpdater
{
	Pointer<IL2CPP::Object* ()> GetInstance(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"Dictionary`2", "Int32"} },
		5
	);

	Pointer<void(IL2CPP::Object* ths, IL2CPP::String* curName, int num, int num2, bool b1, bool b2, AnalyticsParamater params)> AddCurrency(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"String", "Int32", "ENUM", "Boolean", "Boolean", nullptr} }
	);

	Pointer<void(IL2CPP::Object* webInstance, int id, int level, IL2CPP::String* reason, AnalyticsParamater params)> BuyArmor(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"Int32", "Int32", "String", nullptr} }
	);

	Pointer<void(IL2CPP::Object* webInstance, int chestId, int freeSpinEnum, int amount, int dumbEnum, AnalyticsParamater optional)> AddChest(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"Int32", "ENUM", "Int32", "ENUM", nullptr} }
	);

	Pointer<void(IL2CPP::Object* webInstance, int amountIg, IL2CPP::String* curName)> AddEventCurrency(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"Int32", "ENUM", "Int32", "ENUM", nullptr} },
		2
	);

	Pointer<void(IL2CPP::Object* webInstance, int snapshotCause)> UpdateSnaphot(
		"ProgressUpdater",
		{ "internal", "UInt32", nullptr, {"Action", "ENUM", "UInt32"} },
		-1
	);

	Pointer<void(IL2CPP::Object* webInstance, IL2CPP::Object* jsonDictionary)> SaveProgress(
		"ProgressUpdater",
		0x74
		//OBFINT(0x173)
	);

	Pointer<void(IL2CPP::Object* webInstance, IL2CPP::Object* jsonDictionary)> SaveProgress2(
		"ProgressUpdater",
		0x176
	);

	Pointer<void(IL2CPP::Object* webInstance, CommandID cmdId, IL2CPP::Object* jsonDictionary, IL2CPP::Object* idfk, IL2CPP::Object* nullable) > SaveSnapshot(
		"ProgressUpdater",
		0x71
	);

	Pointer<void(IL2CPP::String* petString, int idfk)> AddPet(
		"NiggerObfuscatedClass",
		{ "internal static", "Void", nullptr, {"String", "Int32"} }
	);

	Pointer<void* (void* ptr, int offerItemType, IL2CPP::String* str, int index)> GetInvetoryItemType(
		"InventoryItemType",
		0x5
	);

	Pointer<void* (void* ptr, int offerItemType, IL2CPP::String* str)> GetInvetoryItemTypeNoIndex(
		"InventoryItemType",
		0x2
	);

	Pointer<bool(IL2CPP::Object* webInstance, void* inventoryItem, AnalyticsParamater sex)> AddInventoryItemType(
		"ProgressUpdater",
		{ "internal", "Boolean", nullptr, {nullptr, "Int64", nullptr} },
		-2
	);

	Pointer<void(IL2CPP::Object* webInstance, IL2CPP::Object* moduleData)> UpgradeModule(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"AdaptiveProgress"} },
		-10
	);

	Pointer<void(IL2CPP::String* item, bool idfk, AnalyticsParamater params)> AddRoyale(
		"RoyaleClass",
		{ "internal static", "Void", nullptr, {"String", "Boolean", nullptr} }
	);

	Pointer<void(IL2CPP::Object* webInstance, IL2CPP::Object* killRateObject)> AddCommonWin(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"KillRateObject"} }
	);

	Pointer<void(IL2CPP::Object* webInstance, long curTick, bool isWin)> SendBattleStatistic(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"ENUM", "Int64", "Int32", "Int32"} },
		1
	);

	Pointer<void(IL2CPP::Object* webInstance, int rank, int xp)> SetClanRank(
		"ProgressUpdater",
		{ "private", "Void", nullptr, {"ENUM", "ENUM", nullptr, "Int32", "Nullable`1", nullptr} },
		-1
	);

	Pointer<bool(IL2CPP::Object* webInstance, IL2CPP::Object* item, AnalyticsParamater sex)> AddInventorySingleItem(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"KillRateObject"} },
		-2
	);

	Pointer<bool(IL2CPP::Object* webInstance, IL2CPP::Object* item)> RemoveInventorySingleItem(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"KillRateObject"} },
		-1
	);

	Pointer<bool(IL2CPP::Object* webInstance, int categoryNames, IL2CPP::String* str)> BuyWear(
		"ProgressUpdater",
		{ "private", "String", nullptr, {"String", "Int32"} },
		-1
	);

	Pointer<bool(IL2CPP::Object* webInstance)> BuyPixelPass(
		"ProgressUpdater",
		0x148
	);

	Pointer<bool(IL2CPP::Object* webInstance)> ComplateElitePixelPass(
		"ProgressUpdater",
		0x149
	);

	Pointer<void(IL2CPP::Object* webInstance, int value)> AddPixelPassExp(
		"ProgressUpdater",
		0x14a
	);

	Pointer<void(IL2CPP::Object* webInstance, long idk, IL2CPP::String* skinName, IL2CPP::String* base64data)> UpdateCustomSkin(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"String", "Int32", "ENUM", "Boolean", "Boolean", nullptr} },
		1
	);

	Pointer<void(IL2CPP::Object* webInstance, int idk, IL2CPP::String* key) > RemoveGadget(
		"ProgressUpdater",
		0x30
	);
}

namespace GameConnect
{
	Pointer<int()> CurrentGamemode(
		"GameConnect",
		0xa
	);

	Pointer<bool(int gamemodeEnum)> Idk(
		"GameConnect",
		0x0
	);
}

namespace AuthManager
{
	Pointer<IL2CPP::String* ()> GetPlayerID(
		"NiggerSex",
		0x2
	);
}

namespace ModuleData
{
	Pointer<int(IL2CPP::Object* x)> GetLevel(
		"ModuleData",
		0x5
	);
}

namespace ContentKeyRegister
{
	Pointer<IL2CPP::Object* ()> GetInstance(
		"ContentKeyRegister",
		0x47
	);

	Pointer<IL2CPP::List<IL2CPP::String*>* (IL2CPP::Object* instance, OfferItemType type)> GetRegisterList(
		"ContentKeyRegister",
		0x54
	);

	Pointer<int(IL2CPP::Object* instance, OfferItemType type, IL2CPP::String* key)> GetItemIndex(
		"ContentKeyRegister",
		0x4f
	);
}

namespace ExperienceController
{
	Pointer<int(IL2CPP::Object* experienceController)> GetCurrentLevel(
		"ExperienceController",
		0x6
	);

	Pointer<void(IL2CPP::Object* experienceController, int value, ExpObtainCause reason, IL2CPP::Object* dictionary, IL2CPP::Object* sex)> AddExperience(
		"ExperienceController",
		0x1c
	);
}

namespace LobbyGiverClass
{
	Pointer<bool(IL2CPP::String* str)> AddLobbyItem(
		"LobbyGiverClass",
		0x16
	);
}

namespace FriendsController
{
	Pointer<void(IL2CPP::Object* friendsController, bool winning)> SyncRatingAndRoundWon(
		"FriendsController",
		"SyncRatingAndRoundWon"
	);
}

namespace GadgetManager
{
	Pointer<void(IL2CPP::String* name, int num)> ProvideGadget(
		"GadgetManager",
		{"internal static", "Void", nullptr, {"String", "Int32"}}
	);
}

namespace WearClass
{
	Pointer<void* (CategoryNames categoryName, IL2CPP::String* str)> AddWear(
		"WearClass",
		{"internal static", "Void", nullptr, {"ENUM", "String"}}
	);

	Pointer<int(IL2CPP::String* id)> WearIndex(
		"WearClass",
		0xf
	);
}

namespace RoomInfo
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* room)> GetHashtable(
		"RoomInfo",
		0x4
	);
}

namespace PhotonHashtable
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable)> Constructor(
		"Hashtable",
		0x0
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable, IL2CPP::Object* key)> Get(
		"Hashtable",
		0x2
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable, IL2CPP::Object* key, IL2CPP::Object* val)> Set(
		"Hashtable",
		0x3
	);
};

namespace PhotonNetwork
{
	Pointer<IL2CPP::Object* ()> GetCurrentRoom(
		"PhotonNetwork",
		0xd
	);

	Pointer<IL2CPP::Object* ()> GetLocalPlayer(
		"PhotonNetwork",
		0xe
	);

	Pointer<bool()> IsMasterClient(
		"PhotonNetwork",
		0x32
	);

	Pointer<void(IL2CPP::Object* photonPlayer)> SetMasterClient(
		"PhotonNetwork",
		0x78
	);

	Pointer<IL2CPP::Object* (IL2CPP::String* prefab, Vector3 vec, Quaternion rot, char byte, IL2CPP::Array<IL2CPP::Object*>* settings)> InstantiatePrefab(
		"PhotonNetwork",
		{ "public static", "GameObject", nullptr, {"String", "Vector3", "Quaternion", "Byte"} },
		2
	);
}

namespace MatchSettings
{
	Pointer<IL2CPP::Array<IL2CPP::Object*>* (IL2CPP::String* json)> CreateSettings(
		"MatchSettings",
		0x8
	);
}

namespace PlayerMoveC
{
	Pointer<void(IL2CPP::Object* plrMoveC, bool active)> SetXrayShader(
		"Player_move_c",
		0x184
		//{"internal", "Void", nullptr, {"Boolean", "ENUM"}},
		//2)
	);

	Pointer<void(IL2CPP::Object* plrMoveC)> ShotPressed(
		"Player_move_c",
		{ "private", "IEnumerator", nullptr, {"Int32", "Animation", nullptr} },
		-2
	);

	Pointer<void(IL2CPP::Object* plrMoveC, IL2CPP::String* message, bool isClan, IL2CPP::String* icon)> SendChat(
		"Player_move_c",
		"SendChat"
	);
}

namespace PlayerDamageable
{
	Pointer<bool(IL2CPP::Object* player)> IsDead(
		"PlayerDamageable",
		"IsDead"
	);

	Pointer<bool(IL2CPP::Object* player, IL2CPP::Object* target)> IsEnemyTo(
		"PlayerDamageable",
		"IsEnemyTo"
	);
}

namespace PhotonView
{
	Pointer<void(IL2CPP::Object* instance, EventEnum eventEnum, PhotonTargets photonTarget, void* param)> RPC(
		"PhotonView",
		{ "internal", "Void", nullptr, {"ENUM", "ENUM", "Object[]"} }
	);

	Pointer<bool(IL2CPP::Object* instance)> IsMine(
		"PhotonView",
		"get_isMine"
	);
}

namespace PixelView
{
	Pointer<int(IL2CPP::Object* instance)> GetViewID(
		"PixelView",
		"get_viewID"
	);
}

namespace TutorialClass
{
	Pointer<IL2CPP::Object* ()> GetInstance(
		"TutorialClass",
		0x0
	);

	Pointer<void(IL2CPP::Object* instance)> Fill(
		"TutorialClass",
		0x22
	);
}

namespace TrainingController
{
	Pointer<void()> FinishTraining(
		"TrainingController",
		0x13
	);
}

namespace Encoding
{
	Pointer<IL2CPP::Object* ()> GetUTF8(
		"Encoding",
		"get_UTF8"
	);

	Pointer<IL2CPP::Array<char>* (IL2CPP::Object* encoder, IL2CPP::String* string)> GetBytes(
		"Encoding",
		{"public virtual", "Byte[]", "GetBytes", {"String"}}
	);
}

namespace Type
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* type, IL2CPP::String* name)> GetMethod(
		"Type",
		0x60
	);

	Pointer<IL2CPP::Object* (IL2CPP::String* typeName)> GetType(
		"Type",
		0x94
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* base, IL2CPP::Array<IL2CPP::Object*>* generics)> MakeGenericType(
		"RuntimeType",
		0x6c
	);
}

namespace Activator
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* type)> CreateInstance(
		"Activator",
		0x4
	);
}

namespace MethodBase
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* info, IL2CPP::Object* instance, IL2CPP::Array<IL2CPP::Object*>* args)> Invoke(
		"MethodBase",
		0x14
	);
}