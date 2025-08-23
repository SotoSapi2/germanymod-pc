// Auto generated with ExternMenuComponents.py
#include "../framework/UIFramework.hpp"

namespace Menu
{
    using namespace UIComponents;
    extern bool gMenuShown;

    void INIT();
    
	namespace Gameplay
	{
		extern Section SECTION;

		namespace General
		{
			extern Tab TAB;

			namespace Player
			{
				extern Group GROUP;

				extern Checkbox InfAmmo;
				extern Checkbox Godmode;
				extern Checkbox FirerateHack;
				extern FloatSlider Firerate;
				extern Checkbox NoFixedDelay;
				extern Checkbox GotoPlayers;
				extern FloatSlider GotoPlayersDistance;

				
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
				extern Checkbox AlwaysCritical;
			}

			namespace Movement
			{
				extern Group GROUP;

				extern Checkbox Flyhack;
				extern FloatSlider Flyspeed;

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
				#ifdef EXPERIMENTAL
				extern Checkbox SoftSilentAim;
				extern Checkbox SilentRocket;
				#endif

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

				extern Checkbox RainRocket;
				extern Checkbox Box3DRocket;
				extern Checkbox PenisRocket;
				extern Checkbox TextToRocket;
				extern StringInput RocketTextInput;
			}

			namespace Visual
			{
				extern Group GROUP;

				extern Checkbox Xray;
				extern Checkbox TPS;
				extern Checkbox Spinbot;
				extern Checkbox EspBox;
				extern FloatSlider SpinbotSpeed;
			}
		}

		namespace ServerMods
		{
			extern Tab TAB;

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
				//extern Button CrashEveryone;
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

			namespace World
			{
				extern Group GROUP;
				
				extern Checkbox CrashEveryone;
				extern Checkbox GrabMonster;
				extern Checkbox TpAllToCenter;
				extern Button NoClipEveryone;
				#ifdef EXPERIMENTAL
				extern Button SpawnPlayer;
				#endif
			}
		}
	}

	namespace Account
	{
		extern Section SECTION;

		namespace Unlocker
		{
			extern Tab TAB;

			namespace WeaponUnlocker
			{
				extern Group GROUP;

				extern Text NOTE;

				extern Mode UnlockMode;

				extern IntSlider FromIndexInput;
				extern IntSlider ToIndexInput;

				extern Browser WeaponBrowser;

				extern IntInput WeaponLevel;
				extern Browser WeaponRarity;

				extern Button UnlockButton;
				//extern Button RemoveButton;

				extern Button UnlockRGBSet;
				extern Button UnlockSecret;

				
			}

			namespace WeaponSkinUnlocker
			{
				extern Group GROUP;

				extern Mode UnlockMode;

				extern IntSlider FromIndexInput;
				extern IntSlider ToIndexInput;

				extern Browser WeaponSkinBrowser;

				extern Button UnlockDlcButton;
				extern Button UnlockButton;

				
			}

			namespace GadgetUnlocker
			{
				extern Group GROUP;

				extern Mode UnlockMode;

				extern Browser GadgetBrowser;
				extern IntInput GadgetLevel;
				extern Button UnlockButton;
				extern Button UnlockAllButton;

				
			}

			namespace ModuleUnlocker
			{
				extern Group GROUP;

				extern IntInput ModuleAmount;
				extern IntInput UpgradeAmount;

				extern Button UnlockModule;
				extern Button UpgradeModule;

				
			}

			namespace RoyaleUnlocker
			{	
				extern Group GROUP;

				extern Text NOTE;

				extern Mode UnlockMode;

				extern IntSlider FromIndexInput;
				extern IntSlider ToIndexInput;

				extern Browser RoyalesBrowser;

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

				extern Button UnlockWearButton;
				extern Button UnlockPets;
				extern Button UnlockGraffiti;
				extern Button UnlockLobbyItems;
				extern Button GetAllParts;
				extern Button GetVeteranBadge;

				
			}
		}

		namespace Adder
		{
			extern Tab TAB;
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
				extern Button AddCurrency;

				
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
			extern Tab TAB;

			namespace ClanRank
			{
				extern Group GROUP;
				extern Browser ClanRankBrowser;
				extern IntInput ClanRankXP;

				extern Button SetClanRank;

				
			}

			namespace GlobalWin
			{
				extern Group GROUP;

				extern const std::vector<std::string> gamemodes;

				extern Browser Gamemode;
				extern IntInput Amount;
				extern Button AddStat;

				
			}

			namespace MonthlyMatch
			{
				extern Group GROUP;

				extern Checkbox MatchIsWin;
				extern IntInput Amount;
				extern Button AddStat;

				
			}

			namespace Kill
			{
				extern Group GROUP;

				extern IntInput KillAmount;
				extern IntInput DeathAmount;
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

		namespace ClanStuff
		{
			extern Tab TAB;

			namespace ClanID
			{
				extern Group GROUP;
				extern Text NOTE;

				extern IntInput Input;
			}

			namespace PlaceableUnlocker
			{

				extern Group GROUP;
				extern Mode UnlockMode;

				extern Browser PlaceableBrowser;
				extern IntInput PlaceableAmount;

				extern Button UnlockPlaceable;
				extern Button UnlockAllPlaceable;

				
			}

			namespace Misc
			{
				extern Group GROUP;
				extern Button PromoteClan;

				
			}
		}
	}

	namespace Misc
	{
		extern Section SECTION;

		namespace Bypass
		{
			extern Tab TAB;

			namespace Chat
			{
				extern Group GROUP;

				extern Checkbox BypassChatFilter;

				
			}

			namespace Armory
			{
				extern Group GROUP;

				extern Checkbox AllowShovel;
				extern Checkbox AllowSandbox;

				
			}

			namespace Misc
			{
				extern Group GROUP;

				extern Text NOTE;

				extern Checkbox SafeMode;
				#ifdef EXPERIMENTAL
				extern Checkbox AntiReport;
				#endif
				extern Button ForceReload;
				extern Button SkipTutorial;

				
			}

			namespace Analytics
			{
				extern Group GROUP;

				extern Checkbox AnalyticsBypass;
				//extern Checkbox DeeperAnalyticsBypass;

				
			}
		}

		namespace Skin
		{
			extern Tab TAB;

			

			namespace CustomSkinImporter
			{
				extern Group GROUP;
				extern Text NOTE;

				extern Button ImportSkin;

				
			}

			namespace CustomCapeImporter
			{
				extern Group GROUP;
				extern Text NOTE;

				extern Button ImportCape;

				
			}

			namespace SkinStealer
			{
				extern Group GROUP;
				extern Text NOTE;
				extern IntInput TargetID;
				extern Button Steal;

				
			}

			//namespace ClanIconStealer
			//{
			//	extern Group GROUP;
			//	extern Text NOTE;
			//	extern IntInput TargetID;
			//	extern Button Steal;
			//}
		}
	}

	namespace Guide
	{
		extern Section SECTION;
		namespace BanGuide
		{
			extern Tab TAB;
			namespace Guide
			{
				extern Group GROUP;

				extern Text NOTE;
			}
		}

		namespace ReportBug
		{
			extern Tab TAB;
			namespace Guide
			{
				extern Group GROUP;

				extern Text NOTE;

				extern Button DiscordLink;
				extern Button LocateCrash;

				
			}
		}
	}

	namespace Settings
	{
		extern Section SECTION;

		namespace Menu
		{
			extern Tab TAB;

			namespace MenuCustomization
			{
				extern Group GROUP;

				extern const std::vector<MenuColorScheme> colorSchemeList;

				extern const std::vector<std::string> themseList;

				extern Browser Themes;
				extern Button SetTheme;

				
			}

			//namespace MouseFix
			//{
			//	extern Group GROUP;

			//	extern Checkbox DisableGameClickEvent;
			//	//extern Checkbox DisableCameraMovement;
			//}

			namespace Config
			{
				extern Group GROUP;

				extern Button LoadConfig;
				extern Button SaveConfig;

				
			}
		}

		namespace Credit
		{
			extern Tab TAB;

			extern Group GROUP;
			extern Text NOTE;

			extern Button OpenDC;

			
		}
	}
	
}
