// Auto generated with ExternMenuComponents.py
#include "../framework/UIFramework.hpp"

namespace Menu
{
    extern bool gMenuShown;

    void INIT();
    
	namespace Gameplay
	{
		extern Tab TAB;

		namespace General
		{
			extern SubTab SUBTAB;

			namespace Player
			{
				extern Group GROUP;

				extern Checkbox InfAmmo;
				extern Checkbox Godmode;
				extern Checkbox FirerateHack;
				extern FloatSlider Firerate;
				extern Checkbox NoCatDelay;
				extern Checkbox NoFixedDelay;
			}

			namespace Bullet
			{
				extern Group GROUP;

				extern Checkbox ForceShotgun;
				extern Checkbox BulletExplode;
				extern Checkbox Wallbreak;
			}

			namespace Effects
			{
				extern Group GROUP;

				extern Checkbox NoCharge;
				extern Checkbox EnemyMarker;
				extern Checkbox KillSpeedboost;
				extern Checkbox Lifesteal;
				extern Checkbox HeadMagnifier;
			}

			namespace Movement
			{
				extern Group GROUP;

				extern Checkbox Speedhack;
				extern Checkbox AirJump;
				extern FloatSlider GravityPower;
				extern Checkbox GravityToggle;
			}

			namespace Aim
			{
				extern Group GROUP;

				extern Checkbox Killaura;
				extern Checkbox InfKillauraRadius;
				extern FloatSlider KillauraRadius;

				extern Checkbox Triggerbot;
				extern Checkbox Aimbot;

				extern FloatSlider AimbotSmoothing;
				extern FloatSlider AimbotFOV;
				extern Checkbox AimHead;
				extern Checkbox FOVCircle;

				
			}

			namespace Rocket
			{
				extern Group GROUP;

				extern Mode RocketMode;

				extern Checkbox RocketTower;
				extern Checkbox LongLifetime;
				extern Checkbox NuclearExplosion;
				extern Checkbox Gravity;
				extern Checkbox Ricochet;
			}

			namespace Visual
			{
				extern Group GROUP;

				extern Checkbox Xray;
				extern Checkbox TPS;
			}
		}

		namespace ServerMods
		{
			extern SubTab SUBTAB;

			namespace Modifier
			{
				extern Group GROUP;

				extern Checkbox FriendlyFire;
				extern Checkbox TargetFloatHit;
				extern Checkbox ElectricShock;
				extern Checkbox Polymorpher;
			}

			namespace ChatSpam
			{
				extern Group GROUP;
				extern StringInput message;
				extern Checkbox SpamChat;
			}

			namespace RPC
			{
				extern Group GROUP;

				extern Checkbox DisableJumpAll;
				extern Checkbox SpeedupAll;
				extern Checkbox SlowdownAll;

				extern Button AttractEveryone;
				extern Button NoClipEveryone;
				extern Button CrashEveryone;
			}

			namespace PrefabSpawner
			{
				extern Group GROUP;
				extern Mode PrefabType;

				extern const std::vector<std::string> projectilePrefabs;

				extern Browser ProjectileBrowser;
				extern Checkbox AutoSpawn;
				extern Checkbox LongLifetime;
				extern Button SpawnProjectile;

				extern Text Note;

				extern StringInput BotName;
				extern Button SpawnBot;

				
			}
		}
	}

	namespace Account
	{
		extern Tab TAB;

		namespace Unlocker
		{
			extern SubTab SUBTAB;

			namespace WeaponSkinUnlocker
			{
				extern Group GROUP;

				extern Mode UnlockMode;

				extern IntSlider FromIndexInput;
				extern IntSlider ToIndexInput;

				extern Browser WeaponSkinBrowser;

				extern Button UnlockButton;

				
			}

			namespace WeaponUnlocker
			{
				extern Group GROUP;

				extern Text NOTE;

				extern Mode UnlockMode;

				extern IntSlider FromIndexInput;
				extern IntSlider ToIndexInput;

				extern Browser WeaponBrowser;

				extern IntSlider WeaponLevel;
				extern Browser WeaponRarity;

				extern Button UnlockButton;
				extern Button RemoveButton;

				extern Button UnlockRGBSet;
				extern Button UnlockSecret;

				
			}


			namespace RoyaleUnlocker
			{
				extern Group GROUP;

				extern Mode UnlockMode;

				extern IntSlider FromIndexInput;
				extern IntSlider ToIndexInput;

				extern Browser RoyalesBrowser;

				extern Button UnlockButton;

				
			}

			namespace GadgetUnlocker
			{
				extern Group GROUP;

				extern Mode UnlockMode;

				extern Button UnlockAllButton;

				extern Browser GadgetBrowser;

				extern Button UnlockButton;

				
			}

			namespace ArmorUnlocker
			{
				extern Group GROUP;

				extern Mode UnlockMode;

				extern Browser ArmorBrowser;

				extern IntInput ArmorLevel;
				extern Button UnlockAllArmors;
				extern Button UnlockArmors;

				
			}

			namespace ModuleUnlocker
			{
				extern Group GROUP;

				extern IntInput ModuleAmount;
				extern IntInput UpgradeAmount;

				extern Button UnlockModule;
				extern Button UpgradeModule;
			}

			namespace PixelPassUnlocker
			{
				extern Group GROUP;

				extern IntInput PassXP;
				extern Button AddPassXP;

				extern Button UnlockPixelPass;
			}

			namespace MiscUnlocker
			{
				extern Group GROUP;

				extern Button UnlockWears;
				extern Button UnlockPets;
				extern Button UnlockGraffiti;
				extern Button UnlockLobbyItems;
				extern Button GetAllParts;
				extern Button GetVeteranBadge;
			}
		}

		namespace Adder
		{
			extern SubTab SUBTAB;
			namespace XpEditor
			{
				extern Group GROUP;

				extern Text Note;

				extern IntInput XpAmount;
				extern Button AddXp;
			}

			namespace CurrencyAdder
			{
				extern Group GROUP;

				extern Browser CurrencyBrowser;
				extern IntInput CurrencyAmount;
				extern Button AddXp;

				
			}

			namespace ChestAdder
			{
				extern Group GROUP;

				extern Browser ChestBrowser;
				extern IntInput ChestAmount;
				extern Button AddChest;

				
			}

			namespace BuffAdder
			{
				extern Group GROUP;

				extern Browser BoosterBrowser;
				extern IntInput Duration;
				extern Button AddBuff;

				
			}
		}

		namespace Stats
		{
			extern SubTab SUBTAB;

			namespace GlobalWin
			{
				extern Group GROUP;

				extern const std::vector<std::string> gamemodes;

				extern Browser Gamemode;
				extern Button AddStat;
			}

			namespace MonthlyMatch
			{
				extern Group GROUP;

				extern Checkbox MatchIsWin;
				extern Button AddStat;
			}

			namespace Kill
			{
				extern Group GROUP;

				extern IntInput HeadshotAmount;
				extern Button AddStat;
			}

			namespace Winstreak
			{
				extern Group GROUP;

				extern IntInput KillstreakAmount;
				extern IntInput WinstreakAmount;
				extern Button AddStat;
			}
		}
	}

	namespace Misc
	{
		extern Tab TAB;

		namespace Bypass
		{
			extern SubTab SUBTAB;

			namespace Chat
			{
				extern Group GROUP;

				extern Checkbox BypassChatFilter;
			}

			namespace Armory
			{
				extern Group GROUP;

				extern Checkbox AllowShovel;
			}

			namespace Misc
			{
				extern Group GROUP;
				extern Button ForceReload;
				extern Button SkipTutorial;
			}

			namespace Analytics
			{
				extern Group GROUP;

				extern Checkbox AnalyticsBypass;
				extern Checkbox DeeperAnalyticsBypass;
			}
		}

		namespace Skin
		{
			extern SubTab SUBTAB;

			namespace CustomSkinImporter
			{
				extern Group GROUP;
				extern Text NOTE;

				extern Button ImportSkin;
			}

			namespace SkinStealer
			{
				extern Group GROUP;
				extern Text NOTE;
				extern IntInput TargetID;
				extern Button AddChest;
			}

			namespace ClanIconStealer
			{
				extern Group GROUP;
				extern Text NOTE;
				extern IntInput TargetID;
				extern Button AddChest;
			}
		}
	}

	namespace Settings
	{
		extern Tab TAB;

		namespace Menu
		{
			extern SubTab SUBTAB;

			namespace MenuCustomization
			{
				extern Group GROUP;

				extern const std::vector<MenuColorScheme> colorSchemeList;

				extern const std::vector<std::string> themseList;

				extern Browser Themes;
				extern Button SetTheme;

				
			}

			namespace MouseFix
			{
				extern Group GROUP;

				extern Checkbox DisableGameClickEvent;
				extern Checkbox DisableCameraMovement;
			}

			namespace Config
			{
				extern Group GROUP;

				extern std::string currentConfigPath;
				extern Checkbox DefaultConfig;
				extern Button SelectConfig;
			}
		}
	}
	
}
