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
		{ "public static", "Boolean", "Raycast", {"Ray", "RaycastHit", "Single"} }
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
		"WorldToScreenPoint"
	);
}

namespace GameObject
{
	Pointer<IL2CPP::Array<IL2CPP::Object*>* (void* type)> FindObjectsOfType(
		"Object",
		{ "public static", "Object[]", "FindObjectsOfType", {"Type"} }
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* obj, IL2CPP::String* componentname)> GetComponent(
		"GameObject",
		{ "public", "Component", "GetComponent", {"String"} }
	);

	Pointer<IL2CPP::Array<IL2CPP::Object*>* (IL2CPP::Object* obj, void* type)> GetComponentsInChildren(
		"GameObject",
		{ "public", "Component[]", "GetComponentsInChildren", {"Type"} }
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
		"set_name"
	);

	Pointer<IL2CPP::String* (IL2CPP::Object* gameObject)> GetName(
		"Object",
		"get_name"
	);

	Pointer<void(IL2CPP::Object* gameObject)> Destroy(
		"Object",
		{ "public static", "Void", "Destroy", {"Object"} }
	);

	Pointer<int(IL2CPP::Object* gameObject)> GetInstanceID(
		"Object",
		"GetInstanceID"
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
		{ "public", "Void", "Rotate", {"Vector3", "Single"} }
	);

	Pointer<void(IL2CPP::Object* transform, Vector3 eulers)> SetRotationFromEulers(
		"Transform",
		"set_eulerAngles"
	);

	Pointer<Vector3(IL2CPP::Object* transform)> GetFoward(
		"Transform",
		"get_forward"
	);

	Pointer<void(IL2CPP::Object* transform, IL2CPP::Object* target)> LookAt(
		"Transform",
		{ "public", "Void", "LookAt", {"Transform"} }
	);

	Pointer<void(IL2CPP::Object* transform, Vector3 pos)> LookAtVec(
		"Transform",
		{ "public", "Void", "LookAt", {"Vector3"} }
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* transform)> GetParent(
		"Transform",
		"get_parent"
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
		{ "public static", "List`1", nullptr, {} }
	);
}

namespace NetworkStartTable
{
	Pointer<bool()> IsInMatch(
		"IdkBroClass",
		0xd
	);

}

namespace InventoryItemType
{
	Pointer<IL2CPP::Object* (int index)> GetTypeFromIndex(
		"InventoryItemType",
		0x0
	);

	Pointer<IL2CPP::Object* (OfferItemType offerItemType, IL2CPP::String* str)> GetTypeNoAmount(
		"InventoryItemType",
		0x2
	);

	Pointer<IL2CPP::Object* (OfferItemType offerItemType, IL2CPP::String* str, int amount)> GetType(
		"InventoryItemType",
		0x5
	);

	Pointer<int(IL2CPP::Object* type)> GetIndex(
		"InventoryItemType",
		0x6
	);

	Pointer<IL2CPP::String* (IL2CPP::Object* type)> GetKey(
		"InventoryItemType",
		{ "internal", "String", nullptr, {} }
	);

	Pointer<OfferItemType(IL2CPP::Object* type)> GetOfferItemType(
		"InventoryItemType",
		{ "internal", "OfferItemType", nullptr, {} }
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
		{ "internal", "Int32", nullptr, {"String", "Dictionary`2"} }
	);

	Pointer<int(IL2CPP::Object* instance, IL2CPP::String* msgType)> SendEvent(
		"WebSocketManager",
		{ "internal", "Int32", nullptr, {"String"} }
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
		{ "private static", "Void", nullptr, {"Dictionary`2"} },
		1
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

	Pointer<void(IL2CPP::String* petString, int idfk)> AddPet(
		"NiggerObfuscatedClass",
		{ "internal static", "Void", nullptr, {"String", "Int32"} }
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
		{ "public static", nullptr, nullptr, {} }
	);

	Pointer<IL2CPP::List<IL2CPP::String*>* (IL2CPP::Object* instance, OfferItemType type)> GetRegisterList(
		"ContentKeyRegister",
		{ "internal", "List`1", nullptr, {"ENUM"} }
	);

	Pointer<int(IL2CPP::Object* instance, OfferItemType type, IL2CPP::String* key)> GetItemIndex(
		"ContentKeyRegister",
		{ "public", "Int32", nullptr, {"ENUM", "String"} }
	);
}

namespace ExperienceController
{
	Pointer<int()> GetCurrentLevel(
		"ExperienceController",
		0x7
	);

	Pointer<void(IL2CPP::Object* experienceController, int value, ExpObtainCause reason, IL2CPP::Object* dictionary, IL2CPP::Object* sex)> AddExperience(
		"ExperienceController",
		{ "internal", "Void", nullptr, {"Int32", "ENUM", "Dictionary`2", nullptr} }
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
		{ "internal static", "Void", nullptr, {"String", "Int32"} }
	);
}

namespace WearClass
{
	Pointer<void* (CategoryNames categoryName, IL2CPP::String* str)> AddWear(
		"WearClass",
		{ "internal static", "Void", nullptr, {"ENUM", "String"} }
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

namespace PhotonStream
{
	Pointer<bool(IL2CPP::Object* PhotonStream)> IsWriting(
		"PhotonStream",
		0x3
	);
}

namespace PhotonHashtable
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable)> Constructor(
		"Hashtable",
		".ctor"
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable, IL2CPP::Object* key)> Get(
		"Hashtable",
		"get_Item"
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* hashtable, IL2CPP::Object* key, IL2CPP::Object* val)> Set(
		"Hashtable",
		"set_Item"
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

	Pointer<void(IL2CPP::Object* object)> Destroy(
		"PhotonNetwork",
		{ "public static", "Void", "Destroy", {"GameObject"} }
	);

	Pointer<IL2CPP::Array<IL2CPP::Object*>* ()> GetOtherPlayerList(
		"PhotonNetwork",
		0x13
	);

	Pointer<IL2CPP::Object* (IL2CPP::String* prefab, Vector3 vec, Quaternion rot, char byte)> InstantiatePrefab(
		"PhotonNetwork",
		{ "public static", "GameObject", nullptr, {"String", "Vector3", "Quaternion", "Byte"} }
	);

	Pointer<IL2CPP::Object* (IL2CPP::String* prefab, Vector3 vec, Quaternion rot, char byte, IL2CPP::Array<IL2CPP::Object*>* settings)> SceneInstantiatePrefab(
		"PhotonNetwork",
		{ "public static", "GameObject", nullptr, {"String", "Vector3", "Quaternion", "Byte"} },
		2
	);
}

namespace MatchSettings
{
	Pointer<IL2CPP::Array<IL2CPP::Object*>* (IL2CPP::String* json)> CreateSettings(
		"MatchSettings",
		{ "internal static", "List`1", nullptr, {"String"} }
	);
}

namespace PlayerMoveC
{
	Pointer<void(IL2CPP::Object* plrMoveC, bool active)> SetXrayShader(
		"Player_move_c",
		0x1ea
		//{"internal", "Void", nullptr, {"Boolean", "ENUM"}},
		//2)
	);

	Pointer<void(IL2CPP::Object* plrMoveC, bool unknown)> ShotPressed(
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

	Pointer<IL2CPP::Object* (IL2CPP::Object* instance)> GetOwner(
		"PhotonView",
		"get_owner"
	);

	Pointer<bool(IL2CPP::Object* instance, IL2CPP::Object* player)> TransferOwnership(
		"PhotonView",
		"TransferOwnership"
	);

	Pointer<int(IL2CPP::Object* instance)> GetViewID(
		"PhotonView",
		"get_viewID"
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
		{ "internal static", nullptr, nullptr, {} }
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
		{ "public virtual", "Byte[]", "GetBytes", {"String"} }
	);
}

namespace Type
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* type, IL2CPP::String* name)> GetMethod(
		"Type",
		{ "public", "MethodInfo", "GetMethod", {"String"} }
	);

	Pointer<IL2CPP::Object* (IL2CPP::String* typeName)> GetType(
		"Type",
		{ "public static", "Type", "GetType", {"String"} }
	);

	Pointer<IL2CPP::Object* (IL2CPP::Object* base, IL2CPP::Array<IL2CPP::Object*>* generics)> MakeGenericType(
		"RuntimeType",
		{ "private static", "Type", "MakeGenericType", {"Type", "Type[]"} }
	);
}

namespace Activator
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* type)> CreateInstance(
		"Activator",
		{ "public static", "Object", "CreateInstance", {"Type"} }
	);
}

namespace MethodBase
{
	Pointer<IL2CPP::Object* (IL2CPP::Object* info, IL2CPP::Object* instance, IL2CPP::Array<IL2CPP::Object*>* args)> Invoke(
		"MethodBase",
		"Invoke"
	);
}