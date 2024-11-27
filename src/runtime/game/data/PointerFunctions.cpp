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
	Pointer<Il2CppObject* (Il2CppObject* component)> GetGameObject(
		"Component",
		"get_gameObject"
	);

	Pointer<Il2CppObject* (Il2CppObject* component)> GetTransform(
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
}

namespace Camera
{
	Pointer<Il2CppObject* ()> GetMain(
		"Camera",
		"get_main"
	);

	Pointer<Vector3(Il2CppObject* camera, Vector3 vec)> WorldToScreenPoint(
		"Camera",
		0x38
	);
}

namespace GameObject
{
	Pointer<MonoArray<Il2CppObject*>* (void* type)> FindObjectsOfType(
		"Object",
		0x17
	);

	Pointer<Il2CppObject* (Il2CppObject* obj, MonoString* componentname)> GetComponent(
		"GameObject",
		0x5
	);

	Pointer<MonoArray<Il2CppObject*>* (Il2CppObject* obj, void* type)> GetComponentsInChildren(
		"GameObject",
		0x10
	);

	Pointer<MonoString* (Il2CppObject* gameObject)> GetTag(
		"GameObject",
		"get_tag"
	);

	Pointer<void(Il2CppObject* gameObject, MonoString* tag)> SetTag(
		"GameObject",
		"set_tag"
	);

	Pointer<Il2CppObject* (Il2CppObject* gameObject)> GetTransform(
		"GameObject",
		"get_transform"
	);

	Pointer<void(Il2CppObject* gameObject, MonoString* name)> SetName(
		"Object",
		"SetName"
	);

	Pointer<MonoString* (Il2CppObject* gameObject)> GetName(
		"Object",
		"GetName"
	);

	Pointer<int(Il2CppObject* gameObject)> GetInstanceID(
		"Object",
		0x0
	);
}

namespace Transform
{
	Pointer<Vector3(Il2CppObject* transform)> GetPosition(
		"Transform",
		"get_position"
	);

	Pointer<void(Il2CppObject* transform, Vector3 pos)> SetPosition(
		"Transform",
		"set_position"
	);

	Pointer<Quaternion(Il2CppObject* transform)> GetRotation(
		"Transform",
		"get_rotation"
	);

	Pointer<void(Il2CppObject* transform, Quaternion rot)> SetRotation(
		"Transform",
		"set_rotation"
	);

	Pointer<void(Il2CppObject* transform, Vector3 eulers)> SetRotationFromEulers(
		"Transform",
		0x21
	);

	Pointer<Vector3(Il2CppObject* transform)> GetFoward(
		"Transform",
		"get_forward"
	);

	Pointer<void(Il2CppObject* transform, Il2CppObject* target)> LookAt(
		"Transform",
		"LookAt"
	);

	Pointer<void(Il2CppObject* transform, Vector3 pos)> LookAtVec(
		"Transform",
		0x2a
	);

	Pointer<Il2CppObject* (Il2CppObject* transform)> GetParent(
		"Transform",
		0x14
	);
}

namespace WeaponMenager
{
	Pointer<void* (Il2CppObject* obj, int level, int ItemRarity)> UpgradeV8Constructor(
		"WeaponUpgradeV8",
		{ "internal", "Void", ".ctor", {"Int32", "ENUM"} }
	);

	Pointer<void(Il2CppObject* instance, MonoString* name, int num, bool idfk1, bool idfk2, Il2CppObject* WeaponUpgradeV8, AnalyticsParamater params)> AddWeapon(
		"WeaponManager",
		{ "internal", "Void", nullptr, {"String", "ENUM", "Boolean", "Boolean", nullptr, nullptr} }
	);

	Pointer<MonoList<Il2CppObject*>* (Il2CppObject* obj)> GetPlayerWeapons(
		"WeaponManager",
		{ "internal", "Void", nullptr, {nullptr, "Boolean", "Boolean"} },
		5
	);

	Pointer<void(Il2CppObject* instance, int value)> SetClipAmmo(
		"WeaponW",
		0x5
	);

	Pointer<MonoList<Il2CppObject*>* ()> GetItemRecordList(
		"ItemDb",
		0x6
	);
}

namespace InventoryItemType
{
	Pointer<Il2CppObject* (Il2CppObject* item, int index)> GetTypeFromIndex(
		"InventoryItemType",
		0x0
	);

	Pointer<Il2CppObject* (Il2CppObject* item, OfferItemType offerItemType, MonoString* str)> GetTypeNoAmount(
		"InventoryItemType",
		0x2
	);

	Pointer<Il2CppObject* (Il2CppObject* item, OfferItemType offerItemType, MonoString* str, int amount)> GetType(
		"InventoryItemType",
		0x5
	);

	Pointer<int(Il2CppObject* type)> GetIndex(
		"InventoryItemType",
		0x6
	);

	Pointer<MonoString* (Il2CppObject* type)> GetKey(
		"InventoryItemType",
		0x9
	);

	Pointer<OfferItemType(Il2CppObject* type)> GetOfferItemType(
		"InventoryItemType",
		0x8
	);
}

namespace WebSocketManager
{
	// UNSAFE!!! CAN TRHOW EXCEPTION! DO NOT USE.
	Pointer<Il2CppObject* (Il2CppObject* instance, MonoString* msgType, Il2CppObject* jsonMsg)> SendAsync(
		"WebSocketManager",
		0x10
	);

	// UNSAFE!!! CAN TRHOW EXCEPTION! DO NOT USE.
	Pointer<int(Il2CppObject* instance, MonoString* msgType, Il2CppObject* jsonMsg)> Send(
		"WebSocketManager",
		0x16
	);

	Pointer<int(Il2CppObject* instance, MonoString* msgType)> SendEvent(
		"WebSocketManager",
		0x15
	);
}

namespace KeyClass
{
	// skidded from asteroid. logged param: 1500 1 0 "" 0 3 
	Pointer<bool(int amount, bool idk1, int purchaseEnum, MonoString* idk3, bool idk4, int obtainCause)> AddKey(
		"KeyClass",
		{ "internal static", "Boolean", nullptr, {"Int32", "Boolean", "ENUM", "String", "Boolean", "ENUM"} }
	);
}

namespace ProgressUpdater
{
	Pointer<Il2CppObject* ()> GetInstance(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"Dictionary`2", "Int32"} },
		5
	);

	Pointer<void(Il2CppObject* ths, MonoString* curName, int num, int num2, bool b1, bool b2, AnalyticsParamater params)> AddCurrency(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"String", "Int32", "ENUM", "Boolean", "Boolean", nullptr} }
	);

	Pointer<void(Il2CppObject* webInstance, int id, int level, MonoString* reason, AnalyticsParamater params)> BuyArmor(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"Int32", "Int32", "String", nullptr} }
	);

	Pointer<void(Il2CppObject* webInstance, int chestId, int freeSpinEnum, int amount, int dumbEnum, AnalyticsParamater optional)> AddChest(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"Int32", "ENUM", "Int32", "ENUM", nullptr} }
	);

	Pointer<void(Il2CppObject* webInstance, int amountIg, MonoString* curName)> AddEventCurrency(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"Int32", "ENUM", "Int32", "ENUM", nullptr} },
		2
	);

	Pointer<void(Il2CppObject* webInstance, int snapshotCause)> UpdateSnaphot(
		"ProgressUpdater",
		{ "internal", "UInt32", nullptr, {"Action", "ENUM", "UInt32"} },
		-1
	);

	Pointer<void(Il2CppObject* webInstance, Il2CppObject* jsonDictionary)> SaveProgress(
		"ProgressUpdater",
		0x74
		//OBFINT(0x173)
	);

	Pointer<void(Il2CppObject* webInstance, Il2CppObject* jsonDictionary)> SaveProgress2(
		"ProgressUpdater",
		0x176
	);

	Pointer<void(Il2CppObject* webInstance, CommandID cmdId, Il2CppObject* jsonDictionary, Il2CppObject* idfk, Il2CppObject* nullable) > SaveSnapshot(
		"ProgressUpdater",
		0x71
	);

	Pointer<void(MonoString* petString, int idfk)> AddPet(
		"NiggerObfuscatedClass",
		{ "internal static", "Void", nullptr, {"String", "Int32"} }
	);

	Pointer<void* (void* ptr, int offerItemType, MonoString* str, int index)> GetInvetoryItemType(
		"InventoryItemType",
		0x5
	);

	Pointer<void* (void* ptr, int offerItemType, MonoString* str)> GetInvetoryItemTypeNoIndex(
		"InventoryItemType",
		0x2
	);

	Pointer<bool(Il2CppObject* webInstance, void* inventoryItem, AnalyticsParamater sex)> AddInventoryItemType(
		"ProgressUpdater",
		{ "internal", "Boolean", nullptr, {nullptr, "Int64", nullptr} },
		-2
	);

	Pointer<void(Il2CppObject* webInstance, Il2CppObject* moduleData)> UpgradeModule(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"AdaptiveProgress"} },
		-10
	);

	Pointer<void(MonoString* item, bool idfk, AnalyticsParamater params)> AddRoyale(
		"RoyaleClass",
		{ "internal static", "Void", nullptr, {"String", "Boolean", nullptr} }
	);

	Pointer<void(Il2CppObject* webInstance, Il2CppObject* killRateObject)> AddCommonWin(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"KillRateObject"} }
	);

	Pointer<void(Il2CppObject* webInstance, long curTick, bool isWin)> SendBattleStatistic(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"ENUM", "Int64", "Int32", "Int32"} },
		1
	);

	Pointer<void(Il2CppObject* webInstance, int rank, int xp)> SetClanRank(
		"ProgressUpdater",
		{ "private", "Void", nullptr, {"ENUM", "ENUM", nullptr, "Int32", "Nullable`1", nullptr} },
		-1
	);

	Pointer<bool(Il2CppObject* webInstance, Il2CppObject* item, AnalyticsParamater sex)> AddInventorySingleItem(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"KillRateObject"} },
		-2
	);

	Pointer<bool(Il2CppObject* webInstance, Il2CppObject* item)> RemoveInventorySingleItem(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"KillRateObject"} },
		-1
	);

	Pointer<bool(Il2CppObject* webInstance, int categoryNames, MonoString* str)> BuyWear(
		"ProgressUpdater",
		{ "private", "String", nullptr, {"String", "Int32"} },
		-1
	);

	Pointer<bool(Il2CppObject* webInstance)> BuyPixelPass(
		"ProgressUpdater",
		0x148
	);

	Pointer<bool(Il2CppObject* webInstance)> ComplateElitePixelPass(
		"ProgressUpdater",
		0x149
	);

	Pointer<void(Il2CppObject* webInstance, int value)> AddPixelPassExp(
		"ProgressUpdater",
		0x14a
	);

	Pointer<void(Il2CppObject* webInstance, long idk, MonoString* skinName, MonoString* base64data)> UpdateCustomSkin(
		"ProgressUpdater",
		{ "internal", "Void", nullptr, {"String", "Int32", "ENUM", "Boolean", "Boolean", nullptr} },
		1
	);

	Pointer<void(Il2CppObject* webInstance, int idk, MonoString* key) > RemoveGadget(
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
	Pointer<MonoString* ()> GetPlayerID(
		"NiggerSex",
		0x2
	);
}

namespace ModuleData
{
	Pointer<int(Il2CppObject* x)> GetLevel(
		"ModuleData",
		0x5
	);
}

namespace ContentKeyRegister
{
	Pointer<Il2CppObject* ()> GetInstance(
		"ContentKeyRegister",
		0x47
	);

	Pointer<MonoList<MonoString*>* (Il2CppObject* instance, OfferItemType type)> GetRegisterList(
		"ContentKeyRegister",
		0x54
	);

	Pointer<int(Il2CppObject* instance, OfferItemType type, MonoString* key)> GetItemIndex(
		"ContentKeyRegister",
		0x4f
	);
}

namespace ExperienceController
{
	Pointer<int(Il2CppObject* experienceController)> GetCurrentLevel(
		"ExperienceController",
		0x6
	);

	Pointer<void(Il2CppObject* experienceController, int value, int reason, Il2CppObject* dictionary, Il2CppObject* sex)> AddExperience(
		"ExperienceController",
		0x1c
	);
}

namespace LobbyGiverClass
{
	Pointer<bool(MonoString* str)> AddLobbyItem(
		"LobbyGiverClass",
		0x16
	);
}

namespace FriendsController
{
	Pointer<void(Il2CppObject* friendsController, bool winning)> SyncRatingAndRoundWon(
		"FriendsController",
		"SyncRatingAndRoundWon"
	);
}

namespace GadgetManager
{
	Pointer<void(MonoString* name, int num)> ProvideGadget(
		"GadgetManager",
		{"internal static", "Void", nullptr, {"String", "Int32"}}
	);
}

namespace WearClass
{
	Pointer<void* (int _enum, MonoString* str)> AddWear(
		"WearClass",
		0x25
	);

	Pointer<int(MonoString* id)> WearIndex(
		"WearClass",
		0xf
	);
}

namespace RoomInfo
{
	Pointer<Il2CppObject* (Il2CppObject* room)> GetHashtable(
		"RoomInfo",
		0x4
	);
}

namespace PhotonNetwork
{
	Pointer<Il2CppObject* ()> GetCurrentRoom(
		"PhotonNetwork",
		0xd
	);

	Pointer<Il2CppObject* ()> GetLocalPlayer(
		"PhotonNetwork",
		0xe
	);

	Pointer<bool()> IsMasterClient(
		"PhotonNetwork",
		0x32
	);

	Pointer<void(Il2CppObject* photonPlayer)> SetMasterClient(
		"PhotonNetwork",
		0x78
	);

	Pointer<Il2CppObject* (MonoString* prefab, Vector3 vec, Quaternion rot, char byte, MonoArray<Il2CppObject*>* settings)> InstantiatePrefab(
		"PhotonNetwork",
		{ "public static", "GameObject", nullptr, {"String", "Vector3", "Quaternion", "Byte"} },
		2
	);
}

namespace MatchSettings
{
	Pointer<MonoArray<Il2CppObject*>* (MonoString* json)> CreateSettings(
		"MatchSettings",
		0x8
	);
}

namespace PlayerMoveC
{
	Pointer<void(Il2CppObject* plrMoveC, bool active)> SetXrayShader(
		"Player_move_c",
		0x183
		//{"internal", "Void", nullptr, {"Boolean", "ENUM"}},
		//2)
	);

	Pointer<void(Il2CppObject* plrMoveC)> ShotPressed(
		"Player_move_c",
		{ "private", "IEnumerator", nullptr, {"Int32", "Animation", nullptr} },
		-2
	);

	Pointer<void(Il2CppObject* plrMoveC, MonoString* message, bool isClan, MonoString* icon)> SendChat(
		"Player_move_c",
		"SendChat"
	);
}

namespace PlayerDamageable
{
	Pointer<bool(Il2CppObject* player)> IsDead(
		"PlayerDamageable",
		"IsDead"
	);

	Pointer<bool(Il2CppObject* player, Il2CppObject* target)> IsEnemyTo(
		"PlayerDamageable",
		"IsEnemyTo"
	);
}

namespace PhotonView
{
	Pointer<void(Il2CppObject* instance, int eventEnum, int photonTarget, void* param)> RPC(
		"PhotonView",
		{ "internal", "Void", nullptr, {"ENUM", "ENUM", "Object[]"} }
	);

	Pointer<bool(Il2CppObject* instance)> IsMine(
		"PhotonView",
		"get_isMine"
	);
}

namespace PixelView
{
	Pointer<int(Il2CppObject* instance)> GetViewID(
		"PixelView",
		"get_viewID"
	);
}

namespace TutorialClass
{
	Pointer<Il2CppObject* ()> GetInstance(
		"TutorialClass",
		0x0
	);

	Pointer<void(Il2CppObject* instance)> Fill(
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
	Pointer<Il2CppObject* (MonoString* jsonString)> GetUTF8(
		"Encoding",
		0x0
	);
}

namespace MiniJson
{
	Pointer<Il2CppObject* (MonoString* jsonString)> Deserialize(
		"MiniJson",
		0x0
	);

	Pointer<MonoString* (Il2CppObject* jsonObject)> Serialize(
		"MiniJson",
		0x1
	);
}

namespace UIButton
{
	Pointer<void(Il2CppObject* uiButtonComponent)> OnClick(
		"UIButton",
		"OnClick"
	);
}

namespace Type
{
	Pointer<Il2CppObject* (Il2CppObject* type, MonoString* name)> GetMethod(
		"Type",
		0x60
	);

	Pointer<Il2CppObject* (MonoString* typeName)> GetType(
		"Type",
		0x94
	);

	Pointer<Il2CppObject* (Il2CppObject* obj)> GetObjectType(
		"SystemObject",
		0x5
	);

	Pointer<Il2CppObject* (Il2CppObject* base, Il2CppArray* generics)> MakeGenericType(
		"RuntimeType",
		0x6c
	);
}

namespace Activator
{
	Pointer<Il2CppObject* (Il2CppObject* type)> CreateInstance(
		"Activator",
		0x4
	);
}

namespace MethodBase
{
	Pointer<Il2CppObject* (Il2CppObject* info, Il2CppObject* instance, Il2CppArray* args)> Invoke(
		"MethodBase",
		0x14
	);
}