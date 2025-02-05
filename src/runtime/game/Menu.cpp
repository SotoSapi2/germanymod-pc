#include "Menu.hpp"
#include "../entrypoint.hpp"
#include "../util/Converter.hpp"
#include "MouseFix.hpp"
#include "Functions.hpp"

namespace Menu
{
	using namespace UIFramework;
	bool gMenuShown = false;

	MainWindow WINDOW(ImVec2(900, 600));

	#pragma region MenuComponent
	namespace Gameplay
	{
		Tab TAB(&WINDOW, ICON_FA_GAMEPAD);

		namespace General
		{
			SubTab SUBTAB(&TAB, "Genaral");

			namespace Player
			{
				Group GROUP(&SUBTAB, "Player");

				Checkbox InfAmmo(&GROUP, "Infinite Ammo");
				Checkbox Godmode(&GROUP, "Godmode (client-sided)");
				Checkbox FirerateHack(&GROUP, "Hack Firerate (kickable)");
				FloatSlider Firerate(&GROUP, "Firerate", 0, 50);
				Checkbox NoCatDelay(&GROUP, "No input/output delay");
				Checkbox NoFixedDelay(&GROUP, "No fixed delay (must be enabled in lobby)");
			}

			namespace Bullet
			{
				Group GROUP(&SUBTAB, "Bullet");

				Checkbox ForceShotgun(&GROUP, "Force Shotgun");
				Checkbox BulletExplode(&GROUP, "Force Explosive Bullet");
				Checkbox Wallbreak(&GROUP, "Force Railgun Bullet");
			}

			namespace Effects
			{
				Group GROUP(&SUBTAB, "Effects");

				Checkbox NoCharge(&GROUP, "Instant Charge");
				Checkbox EnemyMarker(&GROUP, "Enemy Marker");
				Checkbox KillSpeedboost(&GROUP, "Kill Speedboost");
				Checkbox Lifesteal(&GROUP, "Lifesteal");
				Checkbox HeadMagnifier(&GROUP, "Head Magnifier");
			}

			namespace Movement
			{
				Group GROUP(&SUBTAB, "Movement");

				Checkbox Speedhack(&GROUP, "Speedhack");
				Checkbox AirJump(&GROUP, "Air jump (Double jump boots needed)");
				FloatSlider GravityPower(&GROUP, "Gravity power", 0.0f, 2.0f, 1.0f);
				Checkbox GravityToggle(&GROUP, "Modify gravity (must be enabled in lobby)");
			}

			namespace Aim
			{
				Group GROUP(&SUBTAB, "Aim");

				Checkbox Killaura(&GROUP, "Killaura (kickable)");
				Checkbox InfKillauraRadius(&GROUP, "Infinite Killaura radius", true);
				FloatSlider KillauraRadius(&GROUP, "Killaura radius", "Killaura_optional", 0.0f, 100, 15);

				Checkbox Triggerbot(&GROUP, "Triggerbot");
				Checkbox Aimbot(&GROUP, "Aimbot");

				FloatSlider AimbotSmoothing(&GROUP, "Smoothing", "Aimbot_options", 0.0f, 1, 0.0);
				FloatSlider AimbotFOV(&GROUP, "FOV", "Aimbot_options", 0.0f, 1000, 180);
				Checkbox AimHead(&GROUP, "Aim at head", "Aimbot_options", false);
				Checkbox FOVCircle(&GROUP, "Show FOV Circle", "Aimbot_options", true);

				#pragma region MenuFunctions
				void Update()
				{
					TagService::ToggleTagVisibility("Killaura_optional", !InfKillauraRadius.value);
					TagService::ToggleTagVisibility("Aimbot_options", Aimbot.value);
				}
				#pragma endregion
			}

			namespace Rocket
			{
				Group GROUP(&SUBTAB, "Rocket");

				Mode RocketMode(&GROUP, "Rocket mode", { "None", "Homing", "Follow Crosshair" });

				Checkbox RocketTower(&GROUP, "Rocket tower");
				Checkbox LongLifetime(&GROUP, "Long lifetime");
				Checkbox NuclearExplosion(&GROUP, "Nuclear explosion");
				Checkbox Gravity(&GROUP, "Gravity");
				Checkbox Ricochet(&GROUP, "Ricochet");
			}

			namespace Visual
			{
				Group GROUP(&SUBTAB, "Visual", GroupPlacementType::RIGHT);

				Checkbox Xray(&GROUP, "X-Ray vision");
				Checkbox TPS(&GROUP, "Third-person view (must be enabled in lobby)");
			}
		}

		namespace ServerMods
		{
			SubTab SUBTAB(&TAB, "Server Mods");

			namespace Modifier
			{
				Group GROUP(&SUBTAB, "Modifier");

				Checkbox FriendlyFire(&GROUP, "Friendly Fire");
				Checkbox TargetFloatHit(&GROUP, "Target float on hit");
				Checkbox ElectricShock(&GROUP, "Electric Shock (Sensivity Troll)");
				Checkbox Polymorpher(&GROUP, "Polymorpher");
			}

			namespace ChatSpam
			{
				Group GROUP(&SUBTAB, "Chat Spam");
				StringInput message(&GROUP, "Spam message", "Nazi Mod - discord.gg/Y3gj2Rszq6", 255);
				Checkbox SpamChat(&GROUP, "Spam chat");
			}

			namespace RPC
			{
				Group GROUP(&SUBTAB, "RPC");

				Checkbox DisableJumpAll(&GROUP, "Disable jump all");
				Checkbox SpeedupAll(&GROUP, "Speedup all");
				Checkbox SlowdownAll(&GROUP, "Slowdown all");

				Button AttractEveryone(&GROUP, "Attract everyone");
				Button NoClipEveryone(&GROUP, "No-clip everyone");
				Button CrashEveryone(&GROUP, "Crash everyone (spam to trigger)");
			}

			namespace PrefabSpawner
			{
				Group GROUP(&SUBTAB, "Prefab Spawner");
				Mode PrefabType(&GROUP, "Prefab type", { "Projectile", "Bot" });

				const std::vector<std::string> projectilePrefabs = {
					"placeholder"
				};

				Browser ProjectileBrowser(&GROUP, "Projectile Prefab", projectilePrefabs);
				Checkbox AutoSpawn(&GROUP, "Auto-spawn", "Prefab_proj", false);
				Checkbox LongLifetime(&GROUP, "Long lifetime", "Prefab_proj", true);
				Button SpawnProjectile(&GROUP, "Spawn projectile", "Prefab_proj");

				Text Note(&GROUP,
					"Bot nickname only works on public match.",
					"Prefab_bot"
				);

				StringInput BotName(&GROUP, "Bot nickname", "Prefab_bot", "Nazi Mod - discord.gg/Y3gj2Rszq6", 255);
				Button SpawnBot(&GROUP, "Spawn Bot", "Prefab_bot");

				#pragma region MenuFunctions
				void Update()
				{
					TagService::ToggleTagVisibility("Prefab_proj", PrefabType.index == 0);
					TagService::ToggleTagVisibility("Prefab_bot", PrefabType.index == 1);
				}
				#pragma endregion
			}
		}
	}

	namespace Account
	{
		Tab TAB(&WINDOW, ICON_FA_USER);

		namespace Unlocker
		{
			SubTab SUBTAB(&TAB, "Content Unlocker");

			namespace WeaponSkinUnlocker
			{
				Group GROUP(&SUBTAB, "Weapon Skin Unlocker");

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual" });

				IntSlider FromIndexInput(&GROUP, "From index", "WepSkinUnlock_auto", 0, 1);
				IntSlider ToIndexInput(&GROUP, "To index", "WepSkinUnlock_auto", 0, 1);

				Browser WeaponSkinBrowser(&GROUP, "Weapon Skin browser", "WepSkinUnlock_manual");

				Button UnlockButton(&GROUP, "Unlock Skin");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::WeaponSkin)->ForEach([&](IL2CPP::String* key)
					{
						WeaponSkinBrowser.list.push_back(key->ToString());
					});

					FromIndexInput.max = WeaponSkinBrowser.list.size() - 2;
					ToIndexInput.max = WeaponSkinBrowser.list.size() - 1;
				}

				void Update()
				{
					TagService::ToggleTagVisibility("WepSkinUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("WepSkinUnlock_manual", UnlockMode.index == 1);
				}
				#pragma endregion
			}

			namespace WeaponUnlocker
			{
				Group GROUP(&SUBTAB, "Weapon Unlocker");

				Text NOTE(&GROUP,
					"Weapon level can't go higher than your current \n"
					"account level.\n"
					"Otherwise the game will refuse to add weapons."
				);

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual", "Misc" });

				IntSlider FromIndexInput(&GROUP, "From index", "WepUnlock_auto", 0, 0);
				IntSlider ToIndexInput(&GROUP, "To index", "WepUnlock_auto", 1, 1);

				Browser WeaponBrowser(&GROUP, "Weapon browser", "WepUnlock_manual");

				IntSlider WeaponLevel(&GROUP, "Weapon level", 0, 65, 10);
				Browser WeaponRarity(&GROUP, "Weapon rarity", { "Common", "Uncommon", "Rare", "Epic", "Legendary", "Mythical" });

				Button UnlockButton(&GROUP, "Unlock Weapon", "WepUnlock_unlockButton");
				Button RemoveButton(&GROUP, "Remove Weapon", "WepUnlock_unlockButton");

				Button UnlockRGBSet(&GROUP, "Unlock RGB Pioneer set", "WepUnlock_misc");
				Button UnlockSecret(&GROUP, "Unlock Secret Weapons", "WepUnlock_misc");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Weapon)->ForEach([&](IL2CPP::String* key)
					{
						WeaponBrowser.list.push_back(key->ToString());
					});

					FromIndexInput.max = WeaponBrowser.list.size() - 2;
					ToIndexInput.max = WeaponBrowser.list.size() - 1;
				}

				void Update()
				{
					TagService::ToggleTagVisibility("WepUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("WepUnlock_manual", UnlockMode.index == 1);
					TagService::ToggleTagVisibility("WepUnlock_misc", UnlockMode.index == 2);
					TagService::ToggleTagVisibility("WepUnlock_unlockButton", UnlockMode.index != 2);
				}
				#pragma endregion
			}


			namespace RoyaleUnlocker
			{
				Group GROUP(&SUBTAB, "Royale Cosmetic Unlocker");

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual" });

				IntSlider FromIndexInput(&GROUP, "From index", "RoyaleUnlock_auto", 0, 1);
				IntSlider ToIndexInput(&GROUP, "To index", "RoyaleUnlock_auto", 0, 1);

				Browser RoyalesBrowser(&GROUP, "Royale browser", "RoyaleUnlock_manual");

				Button UnlockButton(&GROUP, "Unlock Royale");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Royale)->ForEach([&](IL2CPP::String* key)
					{
						RoyalesBrowser.list.push_back(key->ToString());
					});

					FromIndexInput.max = RoyalesBrowser.list.size() - 2;
					ToIndexInput.max = RoyalesBrowser.list.size() - 1;
				}

				void Update()
				{
					TagService::ToggleTagVisibility("RoyaleUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("RoyaleUnlock_manual", UnlockMode.index == 1);
				}
				#pragma endregion
			}

			namespace GadgetUnlocker
			{
				Group GROUP(&SUBTAB, "Gadget Unlocker");

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual" });

				Button UnlockAllButton(&GROUP, "Unlock all Gadget", "GadgetUnlock_auto");

				Browser GadgetBrowser(&GROUP, "Gadget browser", "GadgetUnlock_manual");

				Button UnlockButton(&GROUP, "Unlock Gadget", "GadgetUnlock_manual");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Gadget)->ForEach([&](IL2CPP::String* key)
					{
						GadgetBrowser.list.push_back(key->ToString());
					});
				}

				void Update()
				{
					TagService::ToggleTagVisibility("GadgetUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("GadgetUnlock_manual", UnlockMode.index == 1);
				}
				#pragma endregion
			}

			namespace ArmorUnlocker
			{
				Group GROUP(&SUBTAB, "Armor Unlocker");

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual" });

				Browser ArmorBrowser(&GROUP, "Armor browser", "ArmorUnlock_manual");

				IntInput ArmorLevel(&GROUP, "Armor level", 65);
				Button UnlockAllArmors(&GROUP, "Unlock All armors", "ArmorUnlock_auto");
				Button UnlockArmors(&GROUP, "Unlock armor", "ArmorUnlock_manual");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Armor)->ForEach([&](IL2CPP::String* key)
					{
						ArmorBrowser.list.push_back(key->ToString());
					});
				}

				void Update()
				{
					TagService::ToggleTagVisibility("ArmorUnlock_manual", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("ArmorUnlock_auto", UnlockMode.index == 1);
				}
				#pragma endregion
			}

			namespace ModuleUnlocker
			{
				Group GROUP(&SUBTAB, "Module Unlocker");

				IntInput ModuleAmount(&GROUP, "Module Amount", 2500);
				IntInput UpgradeAmount(&GROUP, "Upgrade Amount", 1);

				Button UnlockModule(&GROUP, "Unlock modules");
				Button UpgradeModule(&GROUP, "Upgrade modules");
			}

			namespace PixelPassUnlocker
			{
				Group GROUP(&SUBTAB, "Pixel Pass Unlocker");

				IntInput PassXP(&GROUP, "Pass EXP Amount", 1000);
				Button AddPassXP(&GROUP, "Add Pass XP");

				Button UnlockPixelPass(&GROUP, "Unlock Pixel Pass");
			}

			namespace MiscUnlocker
			{
				Group GROUP(&SUBTAB, "Misc Unlocker");

				Button UnlockWears(&GROUP, "Unlock wears");
				Button UnlockPets(&GROUP, "Unlock pets");
				Button UnlockGraffiti(&GROUP, "Unlock graffitis");
				Button UnlockLobbyItems(&GROUP, "Unlock lobby items");
				Button GetAllParts(&GROUP, "Get all crafting parts");
				Button GetVeteranBadge(&GROUP, "Get Veteran Badge");
			}
		}

		namespace Adder
		{
			SubTab SUBTAB(&TAB, "Adder/Editor");
			namespace XpEditor
			{
				Group GROUP(&SUBTAB, "XP Adder");

				Text Note(&GROUP,
					"Adding too much XP is bannable. Don't overuse."
				);

				IntInput XpAmount(&GROUP, "XP amount", INT16_MAX, 0, INT16_MAX);
				Button AddXp(&GROUP, "Add XP");
			}

			namespace CurrencyAdder
			{
				Group GROUP(&SUBTAB, "Currency Adder");

				Browser CurrencyBrowser(&GROUP, "Currency browser");
				IntInput CurrencyAmount(&GROUP, "Currency amount");
				Button AddXp(&GROUP, "Add Currency");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Currency)->ForEach([&](IL2CPP::String* key)
					{
						CurrencyBrowser.list.push_back(key->ToString());
					});
				}
				#pragma endregion
			}

			namespace ChestAdder
			{
				Group GROUP(&SUBTAB, "Chest Adder");

				Browser ChestBrowser(&GROUP, "Chest browser");
				IntInput ChestAmount(&GROUP, "Chest amount");
				Button AddChest(&GROUP, "Add Chest");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Chest)->ForEach([&](IL2CPP::String* key)
					{
						ChestBrowser.list.push_back(key->ToString());
					});
				}
				#pragma endregion
			}

			namespace BuffAdder
			{
				Group GROUP(&SUBTAB, "Player Buff Editor");

				Browser BoosterBrowser(&GROUP, "Buff browser");
				IntInput Duration(&GROUP, "Duration (in hour)", 24, 0);
				Button AddBuff(&GROUP, "Add Buff");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::PlayerBuff)->ForEach([&](IL2CPP::String* key)
					{
						BoosterBrowser.list.push_back(key->ToString());
					});
				}
				#pragma endregion
			}
		}

		namespace Stats
		{
			SubTab SUBTAB(&TAB, "Account Stats");

			namespace GlobalWin
			{
				Group GROUP(&SUBTAB, "Global win");

				const std::vector<std::string> gamemodes =
				{
					"Deathmatch",
					"TimeBattle",
					"TeamFight",
					"DeadlyGames",
					"FlagCapture",
					"CapturePoints",
					"InFriendWindow",
					"InClanWindow",
					"Duel",
					"Dater",
					"DeathEscape",
					"Campaign",
					"Arena",
					"SpeedRun",
					"Spleef",
					"Squad",
					"TeamDuel",
					"Siege",
					"ClanSiege",
					"Sniper",
					"BattleRoyale",
					"Glider",
					"Dungeons",
					"Racing",
					"MiniGames",
					"ClassicArena",
					"FortDefence",
					"FortAttack",
					"MMTeamFight",
					"MonsterHunter",
					"Imposter",
					"FreePlay",
					"ArmRace",
					"HoldPositions",
					"Rotation",
					"Competition",
					"PlantTheBomb",
					"Mutation",
					"RedLight",
					"PixelLegends",
					"Domination",
					"DefendTheTower",
					"HideFromMonster",
					"Extraction",
					"PumpkinHunting",
					"Christmas2023"
				};

				Browser Gamemode(&GROUP, "Gamemode browser", gamemodes);
				Button AddStat(&GROUP, "Add win stat");
			}

			namespace MonthlyMatch
			{
				Group GROUP(&SUBTAB, "Montly match");

				Checkbox MatchIsWin(&GROUP, "Set as winning match");
				Button AddStat(&GROUP, "Add monthly match stat");
			}

			namespace Kill
			{
				Group GROUP(&SUBTAB, "Kill/Headshot");

				IntInput HeadshotAmount(&GROUP, "Headshot amount");
				Button AddStat(&GROUP, "Add Kill/Headshot stat");
			}

			namespace Winstreak
			{
				Group GROUP(&SUBTAB, "Killstreak/Winstreak");

				IntInput KillstreakAmount(&GROUP, "Killstreak amount");
				IntInput WinstreakAmount(&GROUP, "Winstreak amount");
				Button AddStat(&GROUP, "Add Killstreak/Winstreak stat");
			}
		}
	}

	namespace Misc
	{
		Tab TAB(&WINDOW, ICON_FA_LIST);

		namespace Bypass
		{
			SubTab SUBTAB(&TAB, "Bypasses");

			namespace Chat
			{
				Group GROUP(&SUBTAB, "Chat");

				Checkbox BypassChatFilter(&GROUP, "Bypass Chat filter", true);
			}

			namespace Armory
			{
				Group GROUP(&SUBTAB, "Armory");

				Checkbox AllowShovel(&GROUP, "Allow to use shovel");
			}

			namespace Misc
			{
				Group GROUP(&SUBTAB, "Misc");
				Button ForceReload(&GROUP, "Force reload");
				Button SkipTutorial(&GROUP, "Skip tutorial");
			}

			namespace Analytics
			{
				Group GROUP(&SUBTAB, "Analytics");

				Checkbox AnalyticsBypass(&GROUP, "Analytics bypass", true);
				Checkbox DeeperAnalyticsBypass(&GROUP, "Deeper Analytics bypass");
			}
		}

		namespace Skin
		{
			SubTab SUBTAB(&TAB, "Skin importer & stealer");

			namespace CustomSkinImporter
			{
				Group GROUP(&SUBTAB, "Custom Skin Importer");
				Text NOTE(&GROUP,
					"Supported Skin Resolution: 64x64, 64x32.\n\n"
					"Pixel Gun doesn't support slim mc skin. Don't import them\n"
					"or the skin will break.\n\n"
					"Custom Skin with 64x64 resolution will be converted\n"
					"into 64x32 automatically."
				);

				Button ImportSkin(&GROUP, "Import Skin");
			}

			namespace SkinStealer
			{
				Group GROUP(&SUBTAB, "Skin Stealer");
				Text NOTE(&GROUP,
					"Stolen skins will be saved on the selected folder."
				);
				IntInput TargetID(&GROUP, "Target ID");
				Button AddChest(&GROUP, "Steal skins from target");
			}

			namespace ClanIconStealer
			{
				Group GROUP(&SUBTAB, "Clan Icon Stealer");
				Text NOTE(&GROUP,
					"Stolen icon will be saved on the selected folder."
				);
				IntInput TargetID(&GROUP, "Target Clan ID");
				Button AddChest(&GROUP, "Steal clan icon");
			}
		}
	}

	namespace Settings
	{
		Tab TAB(&WINDOW, ICON_FA_WRENCH);

		namespace Menu
		{
			SubTab SUBTAB(&TAB, "Menu");

			namespace MenuCustomization
			{
				Group GROUP(&SUBTAB, "Menu Customization");

				const std::vector<MenuColorScheme> colorSchemeList = {
					Themes::darkBlue,
					Themes::darkGreen,
					Themes::darkLavenderBlue,
					{
						ImColor(0, 0, 0),
						ImColor(9, 0, 33),
						ImColor(12, 0, 50),

						ImColor(50, 0, 136),
						ImColor(228, 0, 124),
						ImColor(255, 189, 57),

						ImColor(215, 215, 215),
						ImColor(255, 255, 255)
					}
				};

				const std::vector<std::string> themseList = {
					"Dark Blue",
					"Dark Green",
					"Dark Purple",
					"Retrowave"
				};

				Browser Themes(&GROUP, "Themes", themseList);
				Button SetTheme(&GROUP, "Set theme");

				#pragma region MenuFunctions
				void HandleTheme()
				{
					UIFramework::Global::gMenuColorScheme = colorSchemeList[Themes.index];
				}

				void Load()
				{
					SetTheme.OnClick(HandleTheme);
				}
				#pragma endregion
			}

			namespace MouseFix
			{
				Group GROUP(&SUBTAB, "Mouse Fix");

				Checkbox DisableGameClickEvent(&GROUP, "Disable Game click event", true);
				Checkbox DisableCameraMovement(&GROUP, "Disable Camera movement", false);
			}

			namespace Config
			{
				Group GROUP(&SUBTAB, "Configuration Preset");

				std::string currentConfigPath = "C:\\dumps\\nigga.nazicfg";
				Checkbox DefaultConfig(&GROUP, "Set loaded config as default", false);
				Button SelectConfig(&GROUP, "Select config file");
			}
		}
	}
	#pragma endregion

	void OnUpdate()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_F1))
		{
			gMenuShown = !gMenuShown;
		}

		MouseFix::ShowMouse(gMenuShown);
		Gameplay::General::Aim::Update();
		Gameplay::ServerMods::PrefabSpawner::Update();
		Account::Unlocker::WeaponUnlocker::Update();
		Account::Unlocker::WeaponSkinUnlocker::Update();
		Account::Unlocker::RoyaleUnlocker::Update();
		Account::Unlocker::GadgetUnlocker::Update();
		Account::Unlocker::ArmorUnlocker::Update();

		if (gMenuShown)
		{
			WINDOW.Render();
		}
	}

	void INIT()
	{
		Account::Unlocker::WeaponSkinUnlocker::Load();
		Account::Unlocker::RoyaleUnlocker::Load();
		Account::Unlocker::WeaponUnlocker::Load();
		Account::Unlocker::GadgetUnlocker::Load();
		Account::Unlocker::ArmorUnlocker::Load();
		Account::Adder::CurrencyAdder::Load();
		Account::Adder::BuffAdder::Load();
		Account::Adder::ChestAdder::Load();
		Settings::Menu::MenuCustomization::Load();

		#undef ERROR // NIGGERED WINDOWS MACRO
		if (gTotalFailedPointerDef > 0)
		{
			UIFramework::QueueNotification(NotificationType::ERROR,
				"Nazi Mod didn't initialize properly due to pg update."
				"Although still usable, some features may be unusable."
				"Please wait for an update for fixes."
			);
		}

		UIFramework::INIT(
			Convert::ToString(GetLoaderPath()).append("\\assets"),
			Themes::darkLavenderBlue,
			OnUpdate
		);
	}
}