#include "ClassFinder.hpp"

#include <Logger.hpp>
#include <iostream>
#include <unordered_map>
#include <functional>

namespace FieldPatterns
{
	FieldPattern ProgressUpdater{ "List`1", "List`1", "String", "String", "String", nullptr, "Int32", "Int32", "Int32", "Int32", "String", "String", "String", "String", "String", "String", "String", "Action", "Action", "Action", "Action", "Boolean", "Boolean", "String", "String", "Queue`1", "List`1", "List`1", "Dictionary`2", "Dictionary`2", nullptr, "String", "Boolean", "Int32", "String", "Boolean", "String", "Boolean", "String", "Action`1", "Action`1", "Dictionary`2", "List`1", "Dictionary`2", "Dictionary`2", "Int32", "Int32", "Int32", "Int32", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "Dictionary`2", "List`1", "List`1", "Int32", "Int32", "Dictionary`2", "List`1", "Coroutine", "UInt32" };
	FieldPattern GadgetManager{ nullptr, nullptr, nullptr, "Action`1" };
	FieldPattern PhotonNetwork{ "String", "String", "PhotonHandler", nullptr, "Int32", "Int32", "String", "ServerSettings", "Boolean", "PhotonLogLevel", "List`1", "Single", "Single", "Single", "Boolean", "Boolean", "Dictionary`2", "HashSet`1", "HashSet`1", "Type", "Boolean", "Boolean", nullptr, "Int32", "Boolean", "Boolean", "Int32", "Int32", "Boolean", nullptr, "Boolean", "Stopwatch", "Single", "Boolean", nullptr, "Int32", "Int32", "List`1" };
	FieldPattern NetworkingPeer{ "String", nullptr, "String", "AuthModeOption", "EncryptionMode", "Boolean", "String", "String", "Dictionary`2", "String", "String", "ServerConnection", "ClientState", "Boolean", "Boolean", nullptr, "List`1", "Dictionary`2", nullptr, "String", "Boolean", nullptr, nullptr, "Int32", "Int32", "Int32", "JoinType", nullptr, "Boolean", "String[]", "Int32", "Boolean", "List`1", "CloudRegionCode", "Dictionary`2", nullptr, nullptr, "Boolean", "HashSet`1", "HashSet`1", "Dictionary`2", nullptr, nullptr, "Dictionary`2", "Dictionary`2", "Int16", "Boolean", "String", "String", "Boolean", nullptr, "Dictionary`2", "Dictionary`2", "Dictionary`2", "String", "String", "String", "ServerConnection", "AsyncOperation", "Boolean", "Dictionary`2", "Int32", nullptr, "Int32", "Int32", "Int32", "Int32" };
	FieldPattern PhotonPlayer{ "Int32", "String", "String", "Boolean", "Boolean", "Hashtable", "Object" };
	FieldPattern ItemModule{ "Action", "Lazy`1", "Nullable`1", "Texture", "String", "String", "String", "String" };
	FieldPattern WeaponSkinManager{ "Single", "String", "Comparison`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Action`3", "Action`2", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Dictionary`2", "List`1", "List`1" };
	FieldPattern WeaponSkinIdk{ "Action", "String", "String", "String", "String", "List`1", "Boolean", "Dictionary`2" };
	FieldPattern FreeBundle1{ "LobbyItemsBundle", "Nullable`1", "LobbyItemSlot", "String", "String", "String", "Int32", "String[]", "LobbyItemBuff[]", "LobbyItemLockInfo[]", "Boolean", "String" };
	FieldPattern WearClass{"String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "List`1", "HashSet`1", "List`1", "String", "String", "String", "String", "String", "List`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Int32", "WearScriptableObject_GENERATED", "Action" };
	FieldPattern RoyaleClass{ "String", "String", "String", "String", "String", "String", "String", "String", "String", "RoyaleScriptableObject_GENERATED", "Action`1", "Action`1", "Action`1", "Action`1", "Action`1", "Dictionary`2" };
	FieldPattern WeaponUpgradeV8{ "Int32", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ItemRarity", nullptr };
	FieldPattern WeaponW{ nullptr, nullptr, "ItemRecord", nullptr, "Int32", "Int32[]", "Boolean", "Boolean", "Boolean" };
	FieldPattern AccountCreated{ "String", "Int32", "String", "Boolean", "Boolean", "Nullable`1", "Boolean", "String", "String", "Action" };
	FieldPattern FilterBadWorld{ "String", "String", "RegexOptions", "String[]", "String[]", "String[]", "String[]", "Boolean" };
	FieldPattern PlayerListClass{ "List`1", "List`1", "List`1", "List`1", "List`1", "List`1", "List`1", "List`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Dictionary`2", "List`1", "Dictionary`2", "List`1", "Dictionary`2", "HashSet`1" };
	FieldPattern NiggerObfuscatedClass{ "String", "String", nullptr, "Action`1", "Int32", "String" };
	FieldPattern RaiseEventOptions{ nullptr, "EventCaching", "Byte", "Int32[]", "ReceiverGroup", "Byte", "Boolean", "Boolean" };
	FieldPattern ContentKeyRegister{ nullptr, nullptr, nullptr, "Dictionary`2", "Dictionary`2" };
	FieldPattern ContentKey{ "IDictionary`2" };
	FieldPattern SaltedInt{ "Int32", "Int32" };
	FieldPattern Storager{ "String", "HashSet`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Boolean", "HashSet`1", "IDictionary`2", "String[]" };
	FieldPattern Authorization{ "Int32", "Int32", "Int32", "Int32", "Int32", "Int32", "Int32", "Int32", "Int32", "Int32", "Int32", "Single", "Single", "Single", "Single", nullptr, "Coroutine", nullptr, "ValueTuple`2", "String", "String", nullptr, "String", "String", "Callback`1", "String", "HAuthTicket", "AuthorizationStates", "Coroutine", "String", "String", "String", "Int32", "Single", "String", "Boolean", "Boolean", nullptr, "Boolean", "Boolean", "Action", "Action", "Action", "Action", "Action", "Action`1", "Action", "Action" };
	FieldPattern SlotsController{ "String", "Action", "List`1", "Dictionary`2", "HashSet`1" };
	FieldPattern Room{ "String[]" };
	FieldPattern RoomInfo{ "Boolean", "Hashtable", "Byte", "Int32", "Int32", "String[]", "Boolean", "Boolean", "Boolean", "String", "Int32", "Boolean", "Int32", "Boolean" };
	FieldPattern PlayerStatsEnumrator{ "Int32", "Object", "PlayerStatsController", nullptr, "Boolean", "Boolean", "Boolean" };
	FieldPattern ProgressCommand{ "Action`1", "Action`1", "Action`1", nullptr, nullptr };
	FieldPattern InhouseAnalyticsControllerEnumrator{ "Int32", "Object", "Boolean", "Int64" };
	FieldPattern InhouseAnalyticsControllerEnumrator2{ "Int32", "Object", "InhouseAnalyticsController", "CancellationToken", "Single", "List`1", "UnityWebRequest" };
	FieldPattern AnalyticsClass{ nullptr, nullptr, nullptr, "Dictionary`2", "String", "Int32" };
	FieldPattern InventoryItemType{ nullptr, nullptr, "Nullable`1", nullptr, nullptr, nullptr, "Int32" };
	FieldPattern ModuleData{ "Action", "Lazy`1", "Nullable`1", "Texture", "String", "String", "String", "String" };
	FieldPattern ItemDb{ "IEqualityComparer`1", "Int32", "String[]", "String[]", "Color[]", "List`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "HashSet`1", "Dictionary`2", nullptr, nullptr, nullptr, nullptr, "Dictionary`2", "WeakReference" };
	FieldPattern EnterRoomParams{ "String", nullptr, nullptr, "Hashtable", "Boolean", "Boolean", "Boolean", "String[]" };
	FieldPattern SexClass0{ nullptr, "Dictionary`2", "Dictionary`2", "HashSet`1", nullptr, "Action" };
	FieldPattern AuthManager{ "Int32", "Int32", "Int32", "Single", "Single", "Single", "Single", nullptr, "Coroutine", nullptr, "ValueTuple`2", "String", "String", nullptr, "Boolean", "Boolean", "String", "String", "Int32", "String", "HAuthTicket", "AuthorizationStates", "Coroutine", "Coroutine", "CancellationTokenSource", "Task`1", "String", "String", "Single", "String", "Boolean", "Boolean", nullptr, "Boolean", "Action", "Action", "Action", "Action", "Action", "Action`1", "Action", "Action" };
	FieldPattern DataStorager{ "String", "HashSet`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Boolean", "HashSet`1", "IDictionary`2", "String[]" };
	FieldPattern WebsocketSex0{ "SymmetricAlgorithm" };
	FieldPattern WebsocketSex1{ "Byte[]", "String" };
	FieldPattern LobbyGiverClass{ "Int32", "Action`2", "Action`1", "Action`1" };
	FieldPattern NiggerSex{ "String", "Int32", "String", "Boolean", "Boolean", "Nullable`1", "Boolean", "String", "String", "Action" };
	FieldPattern MatchSettings{ "String", "String", "Int32", "ModificatorType", "Dictionary`2", "String", "String", "String", "String[]", "Dictionary`2", "String[]", "String[]", "String[]", "String[]", "String[]", "Dictionary`2", "Dictionary`2", "String[]", "String[]", "String[]" };
	FieldPattern NiggaTest{ "String", "Action", "List`1", "Dictionary`2", "HashSet`1" };
	FieldPattern TutorialClass{ nullptr, nullptr, "List`1", "List`1", "List`1", "List`1", nullptr };
	FieldPattern SaltedDecimal{ "Int32", "Int32", "Int32", "Int32", "Int32", "Int32[]" };
	FieldPattern KeyClass{ "Boolean", "Action`1", "Action`2", "Action`2", "Action`2", "Action`1", "Action`1", "Action" };
	FieldPattern XPNiggaSex{ nullptr, "String", nullptr };
}

namespace ClassManager
{
	std::unordered_map<std::string, Il2CppClass*> classMap{};

	void RegisterClass(Il2CppClass* klass, const char* name)
	{
		if (klass == nullptr)
		{
			LOG_ERROR("First Arg (Il2CppClass*) is null");
			return;
		}

		classMap.insert({ std::string(name), klass });
	}

	Il2CppClass* GetRegisteredClass(const char* name)
	{
		auto it = classMap.find(std::string(name));

		if (it == classMap.end())
		{
			LOG_ERROR("Could not find %s.", name);
			return nullptr;
		}

		if (it->second == nullptr)
		{
			LOG_ERROR("%s is null", name);
			return nullptr;
		}

		return it->second;
	}

	void ScanImage(const Il2CppImage* image, std::function<void(Il2CppClass*)> iterator)
	{
		for (int i = 0; i < il2cpp_image_get_class_count(image); i++)
		{
			auto klass = il2cpp_image_get_class(image, i);
			auto type = il2cpp_class_get_type(const_cast<Il2CppClass*>(klass));

			iterator(il2cpp_class_from_type(type));
		}
	}
}

Il2CppClass* GetClass(const char* name)
{
	return ClassManager::GetRegisteredClass(name);
}

static void REG_CLASS(Il2CppClass* klass, const char* name)
{
	ClassManager::RegisterClass(klass, name);
}

static bool NAME_CHECK(Il2CppClass* klass, const char* name)
{
	return strcmp(il2cpp_class_get_name(klass), name) == 0;
}

static bool NAMESPACE_CHECK(Il2CppClass* klass, const char* namespaze)
{
	return strcmp(il2cpp_class_get_namespace(klass), namespaze) == 0;
}

static void REG_CLASS_NAMECHECK(Il2CppClass* klass, const char* name)
{
	if (NAME_CHECK(klass, name))
	{
		ClassManager::RegisterClass(klass, name);
	}
}

static void REG_CLASS_PATTERNCHECK(Il2CppClass* klass, size_t fieldCount, size_t methodCount, const FieldPattern& pattern, const char* className)
{
	if (Il2CppUtils::CheckFieldPattern(klass, fieldCount, methodCount, pattern))																
	{
		ClassManager::RegisterClass(klass, className);
	}
}

namespace ClassFinder
{
	void AssemblyCSharpCheck(Il2CppClass* klass)
	{
		if (NAMESPACE_CHECK(klass, ""))
		{
			REG_CLASS_NAMECHECK(klass, "PixelView");
			REG_CLASS_NAMECHECK(klass, "BalanceController");
			REG_CLASS_NAMECHECK(klass, "ExperienceController");
			REG_CLASS_NAMECHECK(klass, "FirstPersonControlSharp");
			REG_CLASS_NAMECHECK(klass, "SkinName");
			REG_CLASS_NAMECHECK(klass, "PixelTime");
			REG_CLASS_NAMECHECK(klass, "WeaponManager");
			REG_CLASS_NAMECHECK(klass, "WeaponSounds");
			REG_CLASS_NAMECHECK(klass, "Player_move_c");
			REG_CLASS_NAMECHECK(klass, "NetworkStartTable");
			REG_CLASS_NAMECHECK(klass, "Rocket");
			REG_CLASS_NAMECHECK(klass, "GameConnect");
			REG_CLASS_NAMECHECK(klass, "PlayerDamageable");
			REG_CLASS_NAMECHECK(klass, "KillRateObject");
			REG_CLASS_NAMECHECK(klass, "KillrateController");
			REG_CLASS_NAMECHECK(klass, "AimCrosshairController");
			REG_CLASS_NAMECHECK(klass, "NotificationController");

			REG_CLASS_NAMECHECK(klass, "ItemRecord");

			REG_CLASS_NAMECHECK(klass, "PlayerStatsController");

			REG_CLASS_NAMECHECK(klass, "AlmanachTutorialManager");
			REG_CLASS_NAMECHECK(klass, "AlmanachMainUI");
			REG_CLASS_NAMECHECK(klass, "TrainingController");

			REG_CLASS_NAMECHECK(klass, "PhotonObjectCacher");
			REG_CLASS_NAMECHECK(klass, "PhotonView");

			REG_CLASS_NAMECHECK(klass, "StoreKitEventListener");
			REG_CLASS_NAMECHECK(klass, "ProfileController");

			REG_CLASS_NAMECHECK(klass, "RocketStack");
			REG_CLASS_NAMECHECK(klass, "ProtocolListGetter");

			REG_CLASS_NAMECHECK(klass, "PlayerBotsManager");
			REG_CLASS_NAMECHECK(klass, "PrivateGamesPanel");
			REG_CLASS_NAMECHECK(klass, "UIInput");
			REG_CLASS_NAMECHECK(klass, "UIButton");
			REG_CLASS_NAMECHECK(klass, "BonusController");
			REG_CLASS_NAMECHECK(klass, "SquadController");
			REG_CLASS_NAMECHECK(klass, "UICamera");
			REG_CLASS_NAMECHECK(klass, "TimeGameController");
			REG_CLASS_NAMECHECK(klass, "PlaceableItemSettings");
			REG_CLASS_NAMECHECK(klass, "FriendsController");
			REG_CLASS_NAMECHECK(klass, "MapPreviewController");
			REG_CLASS_NAMECHECK(klass, "SceneInfoController");
			REG_CLASS_NAMECHECK(klass, "InGameConnection");
			REG_CLASS_NAMECHECK(klass, "PlayerBotsManager");
			REG_CLASS_NAMECHECK(klass, "PlayerBotInstance");

			REG_CLASS_NAMECHECK(klass, "PlayerData");

			REG_CLASS_PATTERNCHECK(klass, 4, 25, FieldPatterns::GadgetManager, "GadgetManager");
			REG_CLASS_PATTERNCHECK(klass, 14, 28, FieldPatterns::RoomInfo, "RoomInfo");
			REG_CLASS_PATTERNCHECK(klass, 204, 65, FieldPatterns::WearClass, "WearClass");
			REG_CLASS_PATTERNCHECK(klass, 10, 10, FieldPatterns::NiggerSex, "NiggerSex");
			REG_CLASS_PATTERNCHECK(klass, 16, 48, FieldPatterns::RoyaleClass, "RoyaleClass");
			REG_CLASS_PATTERNCHECK(klass, 6, 45, FieldPatterns::NiggerObfuscatedClass, "NiggerObfuscatedClass");
			REG_CLASS_PATTERNCHECK(klass, 17, 46, FieldPatterns::ItemDb, "ItemDb");
			REG_CLASS_PATTERNCHECK(klass, 10, 13, FieldPatterns::WeaponUpgradeV8, "WeaponUpgradeV8");
			REG_CLASS_PATTERNCHECK(klass, 9, 12, FieldPatterns::WeaponW, "WeaponW");
			REG_CLASS_PATTERNCHECK(klass, 8, 41, FieldPatterns::KeyClass, "KeyClass");
			REG_CLASS_PATTERNCHECK(klass, 14, 118, FieldPatterns::WeaponSkinManager, "WeaponSkinManager");
			REG_CLASS_PATTERNCHECK(klass, 18, 30, FieldPatterns::PlayerListClass, "PlayerListClass");
			REG_CLASS_PATTERNCHECK(klass, 38, 148, FieldPatterns::PhotonNetwork, "PhotonNetwork");
		}

		if (NAMESPACE_CHECK(klass, "Progress"))
		{
			REG_CLASS_PATTERNCHECK(klass, 72, 471, FieldPatterns::ProgressUpdater, "ProgressUpdater");
		}

		if (NAMESPACE_CHECK(klass, "Rilisoft"))
		{
			REG_CLASS_NAMECHECK(klass, "KeychainCleaner");
			REG_CLASS_NAMECHECK(klass, "LobbyItemsBundle");
			REG_CLASS_NAMECHECK(klass, "OfferItem");

			REG_CLASS_PATTERNCHECK(klass, 1, 3, FieldPatterns::WebsocketSex0, "WebsocketSex0");
		}

		if (NAMESPACE_CHECK(klass, "Rilisoft.WebSocket"))
		{
			REG_CLASS_NAMECHECK(klass, "WebSocketManager");
		}

		if (NAMESPACE_CHECK(klass, "PGCompany"))
		{
			REG_CLASS_PATTERNCHECK(klass, 8, 35, FieldPatterns::ItemModule, "ItemModule");
			REG_CLASS_PATTERNCHECK(klass, 5, 92, FieldPatterns::ContentKeyRegister, "ContentKeyRegister");
			REG_CLASS_PATTERNCHECK(klass, 1, 21, FieldPatterns::ContentKey, "ContentKey");
			REG_CLASS_PATTERNCHECK(klass, 7, 29, FieldPatterns::InventoryItemType, "InventoryItemType");
			REG_CLASS_PATTERNCHECK(klass, 8, 47, FieldPatterns::ModuleData, "ModuleData");
			REG_CLASS_PATTERNCHECK(klass, 4, 41, FieldPatterns::LobbyGiverClass, "LobbyGiverClass");
			REG_CLASS_PATTERNCHECK(klass, 7, 36, FieldPatterns::TutorialClass, "TutorialClass");
			REG_CLASS_PATTERNCHECK(klass, 3, 1, FieldPatterns::XPNiggaSex, "XPNiggaSex");
		}

		if (NAMESPACE_CHECK(klass, "PGCompany.Analytics"))
		{
			REG_CLASS_PATTERNCHECK(klass, 6, 202, FieldPatterns::AnalyticsClass, "AnalyticsClass");
		}	

		if (NAMESPACE_CHECK(klass, "PGCompany.AuthorizationScene"))
		{
			REG_CLASS_NAMECHECK(klass, "AfterBanSceneController");
			REG_CLASS_NAMECHECK(klass, "AccountBlockedWindow");
		}

		if (NAMESPACE_CHECK(klass, "PGCompany.GameModeEvents"))
		{
			REG_CLASS_PATTERNCHECK(klass, 20, 21, FieldPatterns::MatchSettings, "MatchSettings");
		}
	}

	void UnityCoreLoad(Il2CppClass* klass)
	{
		if (NAMESPACE_CHECK(klass, "UnityEngine"))
		{
			REG_CLASS_NAMECHECK(klass, "Screen");
			REG_CLASS_NAMECHECK(klass, "Time");
			REG_CLASS_NAMECHECK(klass, "Camera");
			REG_CLASS_NAMECHECK(klass, "Cursor");
			REG_CLASS_NAMECHECK(klass, "Component");
			REG_CLASS_NAMECHECK(klass, "Transform");
			REG_CLASS_NAMECHECK(klass, "Object");
			REG_CLASS_NAMECHECK(klass, "GameObject");
			REG_CLASS_NAMECHECK(klass, "SystemInfo");
			REG_CLASS_NAMECHECK(klass, "Application");
			REG_CLASS_NAMECHECK(klass, "TouchScreenKeyboard");
			REG_CLASS_NAMECHECK(klass, "MonoBehaviour");
			REG_CLASS_NAMECHECK(klass, "Resources");
			REG_CLASS_NAMECHECK(klass, "Debug");
			REG_CLASS_NAMECHECK(klass, "Quaternion");
			REG_CLASS_NAMECHECK(klass, "Physics");
		}
	}

	void CorlibLoad(Il2CppClass* klass)
	{
		if (NAMESPACE_CHECK(klass, "System.Text"))
		{
			REG_CLASS_NAMECHECK(klass, "Encoding");
		}

		if (NAMESPACE_CHECK(klass, "System.Diagnostics"))
		{
			REG_CLASS_NAMECHECK(klass, "StackTrace");
			REG_CLASS_NAMECHECK(klass, "StackFrame");
		}

		if (NAMESPACE_CHECK(klass, "System.Threading"))
		{
			REG_CLASS_NAMECHECK(klass, "Monitor");
		}

		if (NAMESPACE_CHECK(klass, "System.Collections.Generic"))
		{
			REG_CLASS_NAMECHECK(klass, "List`1");
			REG_CLASS_NAMECHECK(klass, "Dictionary`2");
		}

		if (NAMESPACE_CHECK(klass, "System.Text"))
		{
			REG_CLASS_NAMECHECK(klass, "StringBuilder");
		}

		if (NAMESPACE_CHECK(klass, "System.IO"))
		{
			REG_CLASS_NAMECHECK(klass, "File");
		}

		if (NAMESPACE_CHECK(klass, "System"))
		{
			if (NAME_CHECK(klass, "Object"))
			{
				REG_CLASS(klass, "SystemObject");
			}
			REG_CLASS_NAMECHECK(klass, "Type");
			REG_CLASS_NAMECHECK(klass, "RuntimeType");
			REG_CLASS_NAMECHECK(klass, "Array");
			REG_CLASS_NAMECHECK(klass, "Uri");
			REG_CLASS_NAMECHECK(klass, "Convert");
			REG_CLASS_NAMECHECK(klass, "Activator");
		}

		if (NAMESPACE_CHECK(klass, "System.Reflection"))
		{
			REG_CLASS_NAMECHECK(klass, "MethodBase");
			REG_CLASS_NAMECHECK(klass, "PropertyInfo");
		}

		if (NAMESPACE_CHECK(klass, "System.Security.Cryptography"))
		{
			REG_CLASS_NAMECHECK(klass, "SymmetricAlgorithm");
			REG_CLASS_NAMECHECK(klass, "Rfc2898DeriveBytes");
		}
	}

	void UserSessionManagementLoad(Il2CppClass* klass)
	{
		if (NAMESPACE_CHECK(klass, "Rilisoft.MiniJson"))
		{
			if (NAME_CHECK(klass, "Json"))
			{
				REG_CLASS(klass, "MiniJson");
			}
		}
	}

	void INIT()
	{
		ClassManager::ScanImage(CommonLibImages::AssemblyCSharp, AssemblyCSharpCheck);
		ClassManager::ScanImage(CommonLibImages::UnityCoreModule, UnityCoreLoad);
		ClassManager::ScanImage(CommonLibImages::UnityPhysicsModule, UnityCoreLoad);
		ClassManager::ScanImage(CommonLibImages::Corlib, CorlibLoad);
		ClassManager::ScanImage(CommonLibImages::UserSessionManagement, UserSessionManagementLoad);
	}
}