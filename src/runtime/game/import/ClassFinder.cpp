#include "ClassFinder.hpp"

#include <Logger.hpp>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <IL2CPP.hpp>

#include "PointerFunctions.hpp"
namespace FieldPatterns
{
	using namespace IL2CPP;
	FieldPattern ProgressUpdater{ "List`1", "List`1", "String", "String", "String", nullptr, "Int32", "Int32", "Int32", "Int32", "String", "String", "String", "String", "String", "String", "String", "Action", "Action", "Action", "Action", "Boolean", "Boolean", "String", "String", "Queue`1", "List`1", "List`1", "Dictionary`2", "Dictionary`2", nullptr, "String", "Boolean", "Int32", "String", "Boolean", "String", "Boolean", "String", "Action`1", "Action`1", "Dictionary`2", "List`1", "Dictionary`2", "Dictionary`2", "Int32", "Int32", "Int32", "Int32", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "Dictionary`2", "List`1", "List`1", "Int32", "Int32", "Dictionary`2", "List`1", "Coroutine", "UInt32" };
	FieldPattern GadgetManager{ nullptr, nullptr, nullptr, "Action`1" };
	FieldPattern PhotonNetwork{ "String", "String", "PhotonHandler", nullptr, "Int32", "Int32", "String", "ServerSettings", "Boolean", "PhotonLogLevel", "List`1", "Single", "Single", "Single", "Boolean", "Boolean", "Dictionary`2", "HashSet`1", "HashSet`1", "Type", "Boolean", "Boolean", nullptr, "Int32", "Boolean", "Boolean", "Int32", "Int32", "Boolean", nullptr, "Boolean", "Stopwatch", "Single", "Boolean", nullptr, "Int32", "Int32", "List`1" };
	FieldPattern NetworkingPeer{ "String", nullptr, "String", "AuthModeOption", "EncryptionMode", "Boolean", "String", "String", "Dictionary`2", "String", "String", "ServerConnection", "ClientState", "Boolean", "Boolean", nullptr, "List`1", "Dictionary`2", nullptr, "String", "Boolean", nullptr, nullptr, "Int32", "Int32", "Int32", "JoinType", nullptr, "Boolean", "String[]", "Int32", "Boolean", "List`1", "CloudRegionCode", "Dictionary`2", nullptr, nullptr, "Boolean", "HashSet`1", "HashSet`1", "Dictionary`2", nullptr, nullptr, "Dictionary`2", "Dictionary`2", "Int16", "Boolean", "String", "String", "Boolean", nullptr, "Dictionary`2", "Dictionary`2", "Dictionary`2", "String", "String", "String", "ServerConnection", "AsyncOperation", "Boolean", "Dictionary`2", "Int32", nullptr, "Int32", "Int32", "Int32", "Int32" };
	FieldPattern PhotonPlayer{ "Int32", "String", "String", "Boolean", "Boolean", "Hashtable", "Object" };
	FieldPattern ItemModule{"Action", "Lazy`1", "Nullable`1", "Texture", "String", "String", "String", "String"};
	FieldPattern WeaponSkinManager{ "Single", "String", "Comparison`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Action`3", "Action`2", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Dictionary`2", "List`1", "List`1" };
	FieldPattern WeaponSkinIdk{ "Action", "String", "String", "String", "String", "List`1", "Boolean", "Dictionary`2" };
	FieldPattern FreeBundle1{ "LobbyItemsBundle", "Nullable`1", "LobbyItemSlot", "String", "String", "String", "Int32", "String[]", "LobbyItemBuff[]", "LobbyItemLockInfo[]", "Boolean", "String" };
	FieldPattern WearClass{ "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "List`1", "HashSet`1", "List`1", "String", "String", "String", "String", "String", "List`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Int32", "WearScriptableObject_GENERATED", "Action" };
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
	FieldPattern IdkBroClass{ "String", "String", nullptr, "Action`1", "Int32", "String" };
	FieldPattern PhotonStream{ "Boolean", "Queue`1", "Object[]", "Byte" };
}

namespace ClassFinder
{
	void DoAssemblyCSharpQueue()
	{
		using namespace IL2CPP::ClassMapping;

		#pragma region NO_NAMESPACE
		AddQueue("PixelView", "");
		AddQueue("BalanceController", "");
		AddQueue("ExperienceController", "");
		AddQueue("FirstPersonControlSharp", "");
		AddQueue("SkinName", "");
		AddQueue("PixelTime", "");
		AddQueue("WeaponManager", "");
		AddQueue("WeaponSounds", "");
		AddQueue("Player_move_c", "");
		AddQueue("NetworkStartTable", "");
		AddQueue("Rocket", "");
		AddQueue("GameConnect", "");
		AddQueue("PlayerDamageable", "");
		AddQueue("KillRateObject", "");
		AddQueue("KillrateController", "");
		AddQueue("AimCrosshairController", "");
		AddQueue("NotificationController", "");
		AddQueue("SmoothSyncMovement", "");
		AddQueue("PlayerSynchStream", "");

		AddQueue("ItemRecord", "");

		AddQueue("PlayerStatsController", "");

		AddQueue("AlmanachTutorialManager", "");
		AddQueue("AlmanachMainUI", "");
		AddQueue("TrainingController", "");

		AddQueue("PhotonObjectCacher", "");
		AddQueue("PhotonView", "");

		AddQueue("StoreKitEventListener", "");
		AddQueue("ProfileController", "");

		AddQueue("RocketStack", "");
		AddQueue("ProtocolListGetter", "");

		AddQueue("PlayerBotsManager", "");
		AddQueue("PrivateGamesPanel", "");
		AddQueue("UIInput", "");
		AddQueue("UIButton", "");
		AddQueue("BonusController", "");
		AddQueue("SquadController", "");
		AddQueue("UICamera", "");
		AddQueue("TimeGameController", "");
		AddQueue("PlaceableItemSettings", "");
		AddQueue("FriendsController", "");
		AddQueue("MapPreviewController", "");
		AddQueue("SceneInfoController", "");
		AddQueue("InGameConnection", "");
		AddQueue("PlayerBotsManager", "");
		AddQueue("PlayerBotInstance", "");

		AddQueue("PlayerData", "");

		AddQueue("FilterBadWorld", "", &FieldPatterns::FilterBadWorld, 5);
		AddQueue("SexClass0", "", &FieldPatterns::SexClass0, 22);
		AddQueue("GadgetManager", "", &FieldPatterns::GadgetManager, 25);
		AddQueue("RoomInfo", "", &FieldPatterns::RoomInfo, 28);
		AddQueue("NiggerSex", "", &FieldPatterns::NiggerSex, 10);
		AddQueue("RoyaleClass", "", &FieldPatterns::RoyaleClass, 48);
		AddQueue("NiggerObfuscatedClass", "", &FieldPatterns::NiggerObfuscatedClass, 45);
		AddQueue("ItemDb", "", &FieldPatterns::ItemDb, 46);
		AddQueue("WeaponUpgradeV8", "", &FieldPatterns::WeaponUpgradeV8, 13);
		AddQueue("WeaponW", "", &FieldPatterns::WeaponW, 12);
		AddQueue("KeyClass", "", &FieldPatterns::KeyClass, 41);
		AddQueue("WeaponSkinManager", "", &FieldPatterns::WeaponSkinManager, 118);
		AddQueue("PlayerListClass", "", &FieldPatterns::PlayerListClass);
		AddQueue("PhotonNetwork", "", &FieldPatterns::PhotonNetwork, 148);
		AddQueue("WearClass", "", &FieldPatterns::WearClass);
		AddQueue("IdkBroClass", "", &FieldPatterns::IdkBroClass);
		AddQueue("PhotonStream", "", &FieldPatterns::PhotonStream);
		#pragma endregion

		#pragma region Progress
		AddQueue("ProgressUpdater", "Progress", &FieldPatterns::ProgressUpdater);
		#pragma endregion

		#pragma region Rilisoft
		AddQueue("KeychainCleaner", "Rilisoft");
		AddQueue("LobbyItemsBundle", "Rilisoft");
		AddQueue("OfferItem", "Rilisoft");

		AddQueue("AesFacade", "Rilisoft", &FieldPatterns::WebsocketSex0, 3);
		#pragma endregion

		#pragma region Rilisoft.WebSocket
		AddQueue("WebSocketManager", "Rilisoft.WebSocket");
		#pragma endregion

		#pragma region PGCompany
		AddQueue("ItemModule", "PGCompany", &FieldPatterns::ItemModule, 47);
		AddQueue("ContentKeyRegister", "PGCompany", &FieldPatterns::ContentKeyRegister, 92);
		AddQueue("ContentKey", "PGCompany", &FieldPatterns::ContentKey, 21);
		AddQueue("InventoryItemType", "PGCompany", &FieldPatterns::InventoryItemType, 29);
		AddQueue("ModuleData", "PGCompany", &FieldPatterns::ModuleData, 47);
		AddQueue("LobbyGiverClass", "PGCompany", &FieldPatterns::LobbyGiverClass, 41);
		AddQueue("TutorialClass", "PGCompany", &FieldPatterns::TutorialClass, 36);
		AddQueue("XPNiggaSex", "PGCompany", &FieldPatterns::XPNiggaSex, 1);
		#pragma endregion

		#pragma region PGCompany.Analytics
		AddQueue("AnalyticsClass", "PGCompany.Analytics", &FieldPatterns::AnalyticsClass);
		#pragma endregion

		#pragma region PGCompany.AuthorizationScene
		AddQueue("AfterBanSceneController", "PGCompany.AuthorizationScene");
		AddQueue("AccountBlockedWindow", "PGCompany.AuthorizationScene");
		#pragma endregion

		#pragma region PGCompany.GameModeEvents
		AddQueue("MatchSettings", "PGCompany.GameModeEvents", &FieldPatterns::MatchSettings, 21);
		#pragma endregion

		AddQueue("BaseBot", "RilisoftBot");
		AddQueue("NetworkingPeer", "", &FieldPatterns::NetworkingPeer);
	}

	void DoUnityCoreQueue()
	{
		using namespace IL2CPP::ClassMapping;

		#pragma region UnityEngine
		AddQueue("Screen", "UnityEngine");
		AddQueue("Time", "UnityEngine");
		AddQueue("Camera", "UnityEngine");
		AddQueue("Cursor", "UnityEngine");
		AddQueue("Component", "UnityEngine");
		AddQueue("Transform", "UnityEngine");
		AddQueue("Object", "UnityEngine");
		AddQueue("GameObject", "UnityEngine");
		AddQueue("SystemInfo", "UnityEngine");
		AddQueue("Application", "UnityEngine");
		AddQueue("TouchScreenKeyboard", "UnityEngine");
		AddQueue("MonoBehaviour", "UnityEngine");
		AddQueue("Resources", "UnityEngine");
		AddQueue("Debug", "UnityEngine");
		AddQueue("Quaternion", "UnityEngine");
		AddQueue("Physics", "UnityEngine");
		#pragma endregion
	}

	void DoCorlibQueue()
	{
		using namespace IL2CPP::ClassMapping;

		AddQueue("Type", "System");
		AddQueue("RuntimeType", "System");
		AddQueue("Array", "System");
		AddQueue("Convert", "System");
		AddQueue("Activator", "System");

		AddQueue("Encoding", "System.Text");

		AddQueue("StackTrace", "System.Diagnostics");
		AddQueue("StackFrame", "System.Diagnostics");

		AddQueue("List`1", "System.Collections.Generic");
		AddQueue("Dictionary`2", "System.Collections.Generic");

		AddQueue("SymmetricAlgorithm", "System.Security.Cryptography");
		AddQueue("Rfc2898DeriveBytes", "System.Security.Cryptography");

		AddQueue("MethodBase", "System.Reflection");
	}

	void DoPhoton3Unity3DQueue()
	{
		using namespace IL2CPP::ClassMapping;

		AddQueue("Hashtable", "ExitGames.Client.Photon");
	}

	bool INIT()
	{	
		using namespace IL2CPP;
		using namespace IL2CPP::ClassMapping;
		try
		{
			AddImageToScan(DefaultImage::AssemblyCSharp);
			AddImageToScan(DefaultImage::UnityCoreModule);
			AddImageToScan(DefaultImage::UnityPhysicsModule);
			AddImageToScan(DefaultImage::Corlib);
			AddImageToScan(IL2CPP::CurrentDomain()->OpenAssembly("Photon3Unity3D.dll"));
			AddImageToScan(IL2CPP::CurrentDomain()->OpenAssembly("System.dll"));

			DoAssemblyCSharpQueue();
			DoUnityCoreQueue();
			DoCorlibQueue();
			DoPhoton3Unity3DQueue();

			StartMapping();

			LOG_INFO("Class mapping complete.");
		}
		catch (const IL2CPP::Exception::UnresolvedMappingQueue& err)
		{
			LOG_NOTAG("%s", err.what());
			return false;
		}

		return true;
	}
}