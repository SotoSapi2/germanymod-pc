#include "Menu.hpp"
#include "../entrypoint.hpp"
#include "../util/Converter.hpp"
#include "../framework/FileDialogService.hpp"
#include "../framework/GdiPlusManager.hpp"
#include "MouseFix.hpp"
#include "Functions.hpp"
#include "AntiAnalytics.hpp"
#include "websocket/AccountCommands.hpp"
#include "websocket/ClanCommands.hpp"
#include "websocket/SquadCommands.hpp"
#include "websocket/WebsocketCore.hpp"
#include "utils/MemPatcher.hpp"
#include "Global.hpp"

#include <base64.h>

#undef ERROR // DUMB WINDOWS MACRO :middle_finger:
namespace Menu
{
	using namespace UIComponents;
	using namespace UIFramework;
	bool gMenuShown = false;

	MainWindow WINDOW(ImVec2(900, 600));

	#pragma region MenuComponent
	namespace Gameplay
	{
		Section SECTION(&WINDOW, ICON_FA_GAMEPAD);

		namespace General
		{
			Tab TAB(&SECTION, "Genaral");

			namespace Player
			{
				Group GROUP(&TAB, "Player");

				Checkbox InfAmmo(&GROUP, "Infinite Ammo");
				Checkbox Godmode(&GROUP, "Godmode (client-sided)");
				Checkbox FirerateHack(&GROUP, "Hack Firerate (kickable)");
				FloatSlider Firerate(&GROUP, "Firerate", "Player_firerate", 0, 50);
				Checkbox NoFixedDelay(&GROUP, "No switch delay (must be enabled in lobby)");

				#pragma region MenuFunctions
				void Update()
				{
					TagService::ToggleTagVisibility("Player_firerate", FirerateHack.value);
				}
				#pragma endregion
			}

			namespace Bullet
			{
				Group GROUP(&TAB, "Bullet");

				Checkbox ForceShotgun(&GROUP, "Force Shotgun");
				Checkbox BulletExplode(&GROUP, "Force Explosive Bullet");
				Checkbox Wallbreak(&GROUP, "Force Railgun Bullet");
			}

			namespace Effects
			{
				Group GROUP(&TAB, "Effects");

				Checkbox NoCharge(&GROUP, "Instant Charge");
				Checkbox EnemyMarker(&GROUP, "Enemy Marker");
				Checkbox KillSpeedboost(&GROUP, "Kill Speedboost");
				Checkbox Lifesteal(&GROUP, "Lifesteal");
				Checkbox HeadMagnifier(&GROUP, "Head Magnifier");
			}

			namespace Movement
			{
				Group GROUP(&TAB, "Movement");

				Checkbox Speedhack(&GROUP, "Speedhack");
				Checkbox AirJump(&GROUP, "Air jump (Double jump boots needed)");
				FloatSlider GravityPower(&GROUP, "Gravity power", 0.0f, 2.0f, 1.0f);
				Checkbox GravityToggle(&GROUP, "Modify gravity (must be enabled in lobby)");
			}

			namespace Aim
			{
				Group GROUP(&TAB, "Aim");

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
				Group GROUP(&TAB, "Rocket");

				Mode RocketMode(&GROUP, "Rocket mode", { "None", "Homing", "Follow Crosshair" });

				Checkbox RocketTower(&GROUP, "Rocket tower");
				Checkbox LongLifetime(&GROUP, "Long-lifetime rocket");
				Checkbox NuclearExplosion(&GROUP, "Nuclear explosion");
				Checkbox Gravity(&GROUP, "Gravity");
				Checkbox Ricochet(&GROUP, "Ricochet");
			}

			namespace Visual
			{
				Group GROUP(&TAB, "Visual", GroupPlacementType::LEFT);

				Checkbox Xray(&GROUP, "X-Ray vision");
				Checkbox TPS(&GROUP, "Third-person view (must be enabled in lobby)");
			}
		}

		namespace ServerMods
		{
			Tab TAB(&SECTION, "Server Mods");

			namespace Modifier
			{
				Group GROUP(&TAB, "Modifier");

				Checkbox FriendlyFire(&GROUP, "Friendly Fire");
				Checkbox TargetFloatHit(&GROUP, "Target float on hit");
				Checkbox ElectricShock(&GROUP, "Electric Shock (Sensivity Troll)");
				Checkbox Polymorpher(&GROUP, "Polymorpher");
			}

			namespace ChatSpam
			{
				Group GROUP(&TAB, "Chat Spam");
				StringInput message(&GROUP, "Spam message", "Nazi Mod - discord.gg/Y3gj2Rszq6", 255);
				Checkbox SpamChat(&GROUP, "Spam chat");
			}

			namespace RPC
			{
				Group GROUP(&TAB, "RPC");

				Checkbox DisableJumpAll(&GROUP, "Disable jump all");
				Checkbox SpeedupAll(&GROUP, "Speedup all");
				Checkbox SlowdownAll(&GROUP, "Slowdown all");

				Button AttractEveryone(&GROUP, "Attract everyone");
				Button NoClipEveryone(&GROUP, "No-clip everyone");
				Button CrashEveryone(&GROUP, "Crash everyone (spam to trigger)");
			}

			namespace PrefabSpawner
			{
				Group GROUP(&TAB, "Prefab Spawner");
				Mode PrefabType(&GROUP, "Prefab type", { "Projectile", "Bot" });

				const std::vector<std::string> projectilePrefabs = {
					"gadget_dragonwhistle",
					"Weapon1466",
					"gadget_fakebonus",
					"gadget_nutcracker"
				};

				Browser ProjectileBrowser(&GROUP, "Projectile Prefab", "Prefab_proj", projectilePrefabs);
				Checkbox AutoSpawn(&GROUP, "Auto-spawn", "Prefab_proj", false);
				Checkbox LongLifetime(&GROUP, "Long-lifetime", "Prefab_proj", true);
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
		Section SECTION(&WINDOW, ICON_FA_USER);

		namespace Unlocker
		{
			Tab TAB(&SECTION, "Content Unlocker");

			namespace WeaponUnlocker
			{
				Group GROUP(&TAB, "Weapon Unlocker");

				Text NOTE(&GROUP,
					"Weapon level can't go higher than your current \n"
					"account level.\n"
					"Otherwise the game will refuse to add weapons."
				);

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual", "Misc" });

				IntSlider FromIndexInput(&GROUP, "From index", "WepUnlock_auto", 0, 0);
				IntSlider ToIndexInput(&GROUP, "To index", "WepUnlock_auto", 1, 1);

				Browser WeaponBrowser(&GROUP, "Weapon browser", "WepUnlock_manual");

				IntInput WeaponLevel(&GROUP, "Weapon level", 0, 0, 65);
				Browser WeaponRarity(&GROUP, "Weapon rarity", { "Common", "Uncommon", "Rare", "Epic", "Legendary", "Mythical" }, 5);

				Button UnlockButton(&GROUP, "Unlock Weapon", "WepUnlock_unlockButton");
				//Button RemoveButton(&GROUP, "Remove Weapon", "WepUnlock_unlockButton");

				Button UnlockRGBSet(&GROUP, " Unlock RGB Pioneer set ", "WepUnlock_misc", ButtonSizeType::WINDOW_SIZE);
				Button UnlockSecret(&GROUP, "Unlock Secret Weapons", "WepUnlock_misc", ButtonSizeType::WINDOW_SIZE);

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Weapon)->ForEach([&](IL2CPP::String* key)
					{
						WeaponBrowser.list.push_back(key->ToString());
					});

					FromIndexInput.max = WeaponBrowser.list.size() - 2;
					ToIndexInput.max = WeaponBrowser.list.size() - 1;

					UnlockButton.OnClick([&]
					{
						if (UnlockMode.index == 0)
						{
							AccountCommands::UnlockWeapon(
								WeaponLevel.value,
								WeaponRarity.index,
								FromIndexInput.value,
								ToIndexInput.value
							);
						}
						else if (UnlockMode.index == 1)
						{
							AccountCommands::UnlockWeapon(
								WeaponLevel.value,
								WeaponRarity.index,
								WeaponBrowser.list[WeaponBrowser.index]
							);
						}

						WebsocketCore::Reload();
					});

					UnlockRGBSet.OnClick([&]
					{
						AccountCommands::UnlockRGBWeaponSet(WeaponLevel.value, WeaponRarity.index);

						WebsocketCore::Reload();
					});

					UnlockSecret.OnClick([&]
					{
						AccountCommands::UnlockGoofyWeapons(WeaponLevel.value, WeaponRarity.index);

						WebsocketCore::Reload();
					});

					//RemoveButton.OnClick([&]
					//{
					//	if (UnlockMode.index == 0)
					//	{
					//		AccountCommands::RemoveWeapon(
					//			FromIndexInput.value,
					//			ToIndexInput.value
					//		);
					//	}
					//	else if (UnlockMode.index == 1)
					//	{
					//		AccountCommands::RemoveWeapon(
					//			WeaponBrowser.list[WeaponBrowser.index]
					//		);
					//	}

					//	WebsocketCore::Reload();
					//});
				}

				void Update()
				{
					TagService::ToggleTagVisibility("WepUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("WepUnlock_manual", UnlockMode.index == 1);
					TagService::ToggleTagVisibility("WepUnlock_misc", UnlockMode.index == 2);
					TagService::ToggleTagVisibility("WepUnlock_unlockButton", UnlockMode.index != 2);

					FromIndexInput.value = std::clamp(FromIndexInput.value, 0, ToIndexInput.value);
					ToIndexInput.value = std::clamp(ToIndexInput.value, FromIndexInput.value, (int)WeaponBrowser.list.size());
				}
				#pragma endregion
			}

			namespace WeaponSkinUnlocker
			{
				Group GROUP(&TAB, "Weapon Skin Unlocker");

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual", "Misc"});

				IntSlider FromIndexInput(&GROUP, "From index ", "WepSkinUnlock_auto", 0, 1);
				IntSlider ToIndexInput(&GROUP, "To index ", "WepSkinUnlock_auto", 0, 1);

				Browser WeaponSkinBrowser(&GROUP, "Weapon Skin browser", "WepSkinUnlock_manual");

				Button UnlockDlcButton(&GROUP, "Unlock DLC Skins", "WepSkinUnlock_misc");
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

					UnlockButton.OnClick([&]
					{
						if (UnlockMode.index == 0)
						{
							AccountCommands::UnlockWeaponSkin(
								FromIndexInput.value,
								ToIndexInput.value
							);
						}
						else if (UnlockMode.index == 1)
						{
							AccountCommands::UnlockWeaponSkin(
								WeaponSkinBrowser.list[WeaponSkinBrowser.index]
							);
						}

						WebsocketCore::Reload();
					});

					UnlockDlcButton.OnClick([&]
					{
						AccountCommands::UnlockDlcWeaponSkin(); 
						WebsocketCore::Reload();
					});
				}

				void Update()
				{
					TagService::ToggleTagVisibility("WepSkinUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("WepSkinUnlock_manual", UnlockMode.index == 1);
					TagService::ToggleTagVisibility("WepSkinUnlock_misc", UnlockMode.index == 2);

					if (UnlockMode.index == 2)
					{
						UnlockButton.visible = false;
					}
					else
					{
						UnlockButton.visible = true;
					}

					FromIndexInput.value = std::clamp(FromIndexInput.value, 0, ToIndexInput.value);
					ToIndexInput.value = std::clamp(FromIndexInput.value, ToIndexInput.value, ToIndexInput.max);
				}
				#pragma endregion
			}

			namespace GadgetUnlocker
			{
				Group GROUP(&TAB, "Gadget Unlocker");

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual" });

				Browser GadgetBrowser(&GROUP, "Gadget browser", "GadgetUnlock_manual");

				IntInput GadgetLevel(&GROUP, "Gadget level", 0);

				Button UnlockButton(&GROUP, "Unlock Gadget", "GadgetUnlock_manual");

				Button UnlockAllButton(&GROUP, "Unlock all Gadget", "GadgetUnlock_auto");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Gadget)->ForEach([&](IL2CPP::String* key)
					{
						GadgetBrowser.list.push_back(key->ToString());
					});

					UnlockButton.OnClick([&]
					{
						GadgetManager::ProvideGadget(
							IL2CPP::String::Create(GadgetBrowser.list[GadgetBrowser.index]),
							GadgetLevel.value
						);
					});

					UnlockAllButton.OnClick([&]
					{
						ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Gadget)->ForEach([&](IL2CPP::String* key)
						{
							GadgetManager::ProvideGadget(
								key,
								GadgetLevel.value
							);
						});
					});
				}

				void Update()
				{
					TagService::ToggleTagVisibility("GadgetUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("GadgetUnlock_manual", UnlockMode.index == 1);
				}
				#pragma endregion
			}

			namespace ModuleUnlocker
			{
				Group GROUP(&TAB, "Module Unlocker");

				IntInput ModuleAmount(&GROUP, "Module Amount", 2500);
				IntInput UpgradeAmount(&GROUP, "Upgrade Amount", 1);

				Button UnlockModule(&GROUP, "Unlock modules");
				Button UpgradeModule(&GROUP, "Upgrade modules");

				#pragma region MenuFunctions
				void Load()
				{
					UnlockModule.OnClick([&]
					{
						AccountCommands::UnlockModules(ModuleAmount.value);
						WebsocketCore::Reload();
					});

					UpgradeModule.OnClick([&]
					{
						for (size_t i = 0; i < UpgradeAmount.value; i++)
						{
							AccountCommands::UpgradeModules();
						}

						WebsocketCore::Reload();
					});
				}

				#pragma endregion
			}

			namespace RoyaleUnlocker
			{	
				Group GROUP(&TAB, "Royale Cosmetic Unlocker");

				Text NOTE(&GROUP,
					"Unlocking too much cosmetic might cause the game\n"
					"to reload and no cosmetic will be added.\n"
				);

				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual" });

				IntSlider FromIndexInput(&GROUP, "From index  ", "RoyaleUnlock_auto", 0, 1);
				IntSlider ToIndexInput(&GROUP, "To index  ", "RoyaleUnlock_auto", 0, 1);

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

					UnlockButton.OnClick([&]
					{
						if (UnlockMode.index == 0)
						{
							ContentKeyRegister::IterateKeyRegister(OfferItemType::Royale, [&](IL2CPP::String* key)
							{
								if (ContentKeyRegister::IsKeyBannable(key)) return;
								ProgressUpdater::AddRoyale(key, true, Global::offerwallParam);
							});
						}
						else if (UnlockMode.index == 1)
						{
							ProgressUpdater::AddRoyale(
								IL2CPP::String::Create(RoyalesBrowser.list[RoyalesBrowser.index]), 
								true, 
								Global::offerwallParam
							);
						}
					});
				}

				void Update()
				{
					TagService::ToggleTagVisibility("RoyaleUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("RoyaleUnlock_manual", UnlockMode.index == 1);

					FromIndexInput.value = std::clamp(FromIndexInput.value, 0, ToIndexInput.value);
					ToIndexInput.value = std::clamp(FromIndexInput.value, ToIndexInput.value, ToIndexInput.max);
				}
				#pragma endregion
			}

			namespace ArmorUnlocker
			{
				Group GROUP(&TAB, "Armor Unlocker");

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

					UnlockAllArmors.OnClick([&] 
					{
						for (auto v : ArmorBrowser.list)
						{
							ProgressUpdater::BuyArmor(
								ProgressUpdater::GetInstance(),
								WearClass::WearIndex(IL2CPP::String::Create(v)),
								ArmorLevel.value,
								IL2CPP::String::Create(""),
								Global::offerwallParam
							);
						}

					});

					UnlockArmors.OnClick([&]
					{
						for (auto v : ArmorBrowser.list)
						{
							if (ArmorBrowser.list[ArmorBrowser.index] != v) continue;

							ProgressUpdater::BuyArmor(
								ProgressUpdater::GetInstance(),
								WearClass::WearIndex(IL2CPP::String::Create(v)),
								ArmorLevel.value,
								IL2CPP::String::Create(""),
								Global::offerwallParam
							);

							break;
						}
					});
				}

				void Update()
				{
					TagService::ToggleTagVisibility("ArmorUnlock_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("ArmorUnlock_manual", UnlockMode.index == 1);
				}
				#pragma endregion
			}

			namespace PixelPassUnlocker
			{
				Group GROUP(&TAB, "Pixel Pass Unlocker", GroupPlacementType::RIGHT);

				IntInput PassXP(&GROUP, "Pass EXP Amount", 1000);
				Button AddPassXP(&GROUP, "Add Pass XP");

				Button UnlockPixelPass(&GROUP, "Unlock Pixel Pass");

				#pragma region MenuFunctions
				void Load()
				{
					UnlockPixelPass.OnClick([&]
					{
						ProgressUpdater::BuyPixelPass(ProgressUpdater::GetInstance());
					});

					AddPassXP.OnClick([&]
					{
						ProgressUpdater::AddPixelPassExp(ProgressUpdater::GetInstance(), PassXP.value);
					});
				}
				#pragma endregion
			}

			namespace MiscUnlocker
			{
				Group GROUP(&TAB, "Misc Unlocker", GroupPlacementType::RIGHT);

				Button UnlockWearButton(&GROUP, "Unlock wears");
				Button UnlockPets(&GROUP, "Unlock pets");
				Button UnlockGraffiti(&GROUP, "Unlock graffitis");
				Button UnlockLobbyItems(&GROUP, "Unlock lobby items");
				Button GetAllParts(&GROUP, "Get all crafting parts");
				Button GetVeteranBadge(&GROUP, "Get Veteran Badge");

				#pragma region MenuFunctions
				void Load()
				{
					auto UnlockWears = [](OfferItemType type, int wearId)
					{
						IL2CPP::Object* instance = ProgressUpdater::GetInstance();
						ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type)->ForEach([&](IL2CPP::String* x)
						{
							ProgressUpdater::BuyWear(instance, wearId, x);
						});
					};

					UnlockWearButton.OnClick([&]
					{
						UnlockWears(OfferItemType::Hat, 6);
						UnlockWears(OfferItemType::Mask, 12);
						UnlockWears(OfferItemType::Cape, 9);
						UnlockWears(OfferItemType::Boots, 10);
					});

					UnlockPets.OnClick([&]
					{
						ContentKeyRegister::IterateKeyRegister(OfferItemType::Pet, [&](IL2CPP::String* x)
						{
							ProgressUpdater::AddPet(x, 9999);
						});
					});

					UnlockGraffiti.OnClick([&]
					{
						AccountCommands::UnlockGraffities();
					});

					UnlockLobbyItems.OnClick([&]
					{
						ContentKeyRegister::IterateKeyRegister(OfferItemType::LobbyItem, [&](IL2CPP::String* x)
						{
							LobbyGiverClass::AddLobbyItem(x);
						});
					});

					GetAllParts.OnClick([&]
					{
						AccountCommands::AddParts();
						WebsocketCore::Reload();
					});

					GetVeteranBadge.OnClick([&]
					{
						AccountCommands::GetVeteranBadge();
						WebsocketCore::Reload();
					});
				}
				#pragma endregion
			}
		}

		namespace Adder
		{
			Tab TAB(&SECTION, "Adder/Editor");
			namespace XpEditor
			{
				Group GROUP(&TAB, "XP Adder");

				Text Note(&GROUP,
					"Adding too much XP is bannable. Don't overuse."
				);

				IntInput XpAmount(&GROUP, "XP amount", INT16_MAX, 0, INT16_MAX);
				Button AddXp(&GROUP, "Add XP");

				#pragma region MenuFunctions
				void Load()
				{
					using namespace IL2CPP::ClassMapping;
					using namespace IL2CPP::Wrapper;

					AddXp.OnClick([&]
					{
						Global::ExecuteOnGameThread([]
						{
							auto expControllerInstance = GetClass("ExperienceController")->GetField(0x1b)->GetValue<IL2CPP::Object*>(nullptr);
							Method<void(IL2CPP::Object*)> ctor = GetClass("XPNiggaSex")->GetMethod(0x0);

							IL2CPP::Object* nigger = IL2CPP::Class::Create(GetClass("XPNiggaSex"));
							ctor(nigger);

							ExperienceController::AddExperience(
								expControllerInstance,
								XpAmount.value,
								6549877,
								nullptr,
								nigger
							);
						});
					});
				}
				#pragma endregion
			}

			namespace CurrencyAdder
			{
				Group GROUP(&TAB, "Currency Adder");

				Browser CurrencyBrowser(&GROUP, "Currency browser", { 
					"GemsCurrency_1", "Coins_1", "LotteryKey", "ClanSilver", "ClanLootBoxPoints",
					"Coupons", "PixelPassCurrency"
				});
				IntInput CurrencyAmount(&GROUP, "Currency amount");
				Button AddCurrency(&GROUP, "Add Currency");

				#pragma region MenuFunctions
				void Load()
				{
					//ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Currency)->ForEach([&](IL2CPP::String* key)
					//{
					//	CurrencyBrowser.list.push_back(key->ToString());
					//});

					// ProgressUpdater::AddCurrency doesnt add lottery key for idk what reason that is. 
					// So we use KeyClass::AddKey as the work around.
					CurrencyBrowser.list.emplace_back("LotteryKey");
					AddCurrency.OnClick([&]
					{
						std::string key = Convert::LowercaseString(CurrencyBrowser.list[CurrencyBrowser.index]);
						if (CurrencyBrowser.list[CurrencyBrowser.index] == "LotteryKey")
						{
							KeyClass::AddKey(CurrencyAmount.value, true, 0, IL2CPP::String::Create(""), false, 3);
						}
						else
						{
							ProgressUpdater::AddCurrency(
								ProgressUpdater::GetInstance(),
								IL2CPP::String::Create(key),
								CurrencyAmount.value,
								1,
								false,
								false,
								Global::offerwallParam
							);
						}
					});
				}
				#pragma endregion
			}

			namespace ChestAdder
			{
				Group GROUP(&TAB, "Chest Adder");

				Browser ChestBrowser(&GROUP, "Chest browser", {"novice chest", "medium chest", "winner chest", "war hero chest"});
				IntInput ChestAmount(&GROUP, "Chest amount");
				Button AddChest(&GROUP, "Add Chest");

				#pragma region MenuFunctions
				void Load()
				{
					using namespace IL2CPP::ClassMapping;
					//ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::Chest)->ForEach([&](IL2CPP::String* key)
					//{
					//	ChestBrowser.list.push_back(key->ToString());
					//});

					AddChest.OnClick([&]
					{
						IL2CPP::Object* instance = ProgressUpdater::GetInstance();;
						int amount = ChestAmount.value;

						switch (ChestBrowser.index)
						{
							case 0:
								ProgressUpdater::AddChest(instance, 1, 10, amount, 0, Global::offerwallParam);
								break;
							case 1:
								ProgressUpdater::AddChest(instance, 2, 10, amount, 0, Global::offerwallParam);
								break;
							case 2:
								ProgressUpdater::AddChest(instance, 3, 10, amount, 0, Global::offerwallParam);
								break;
							case 3:
							{
								IL2CPP::Object* sex = IL2CPP::Class::Create(GetClass("InventoryItemType"));
								IL2CPP::Object* type = InventoryItemType::GetType(
									sex,
									OfferItemType::ClanLootBox,
									IL2CPP::String::Create("wargift_small"),
									amount
								);

								ProgressUpdater::AddInventoryItemType(instance, type, Global::offerwallParam);
								break;
							}
							default:
								break;
						}
					});
				}
				#pragma endregion
			}

			namespace BuffAdder
			{
				Group GROUP(&TAB, "Player Buff Editor");

				Browser BoosterBrowser(&GROUP, "Buff browser");
				IntInput Duration(&GROUP, "Duration (in hour)", 24, 0);
				Button AddBuff(&GROUP, "Add Buff");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::IterateKeyRegister(OfferItemType::PlayerBuff, [&](IL2CPP::String* key)//ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), OfferItemType::PlayerBuff)->ForEach([&](IL2CPP::String* key)
					{
						BoosterBrowser.list.push_back(key->ToString());
					});

					AddBuff.OnClick([&]
					{
						AccountCommands::AddPlayerBuff(
							BoosterBrowser.list[BoosterBrowser.index],
							Duration.value * 3600
						);

						WebsocketCore::Reload();
					});
				}
				#pragma endregion
			}
		}

		namespace Stats
		{
			Tab TAB(&SECTION, "Account Stats");

			namespace ClanRank
			{
				Group GROUP(&TAB, "Clan Rank");
				Browser ClanRankBrowser(&GROUP, "Clan Rank", {
					"Newbie",
					"Recruit",
					"Fighter",
					"Warrior",
					"Lieutenant",
					"Captain",
					"Major",
					"Lt. Colonel",
					"Colonel",
					"General",
					"Emperor"
				}, 10);
				IntInput ClanRankXP(&GROUP, "Clan Rank XP", 130000);

				Button SetClanRank(&GROUP, "Set Clan Rank");

				#pragma region MenuFunctions
				void Load()
				{
					SetClanRank.OnClick([&]
					{
						ProgressUpdater::SetClanRank(
							ProgressUpdater::GetInstance(),
							ClanRankBrowser.index + 1,
							ClanRankXP.value
						);
					});
				}
				#pragma endregion
			}

			namespace GlobalWin
			{
				Group GROUP(&TAB, "Global win");

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
				IntInput Amount(&GROUP, "Amount");
				Button AddStat(&GROUP, "Add win stat");

				#pragma region MenuFunctions
				void Load()
				{
					AddStat.OnClick([&]
					{
						AccountCommands::AddWinStat(Gamemode.index, Amount.value);
					});
				}
				#pragma endregion
			}

			namespace MonthlyMatch
			{
				Group GROUP(&TAB, "Montly match");

				Checkbox MatchIsWin(&GROUP, "Set as winning match");
				IntInput Amount(&GROUP, "Amount");
				Button AddStat(&GROUP, "Add monthly match stat");

				#pragma region MenuFunctions
				void Load()
				{
					AddStat.OnClick([&]
					{
						for (size_t i = 0; i < Amount.value; i++)
						{
							AccountCommands::AddMatchStat(MatchIsWin.value);
						}

						WebsocketCore::Reload();
					});

				}
				#pragma endregion
			}

			namespace Kill
			{
				Group GROUP(&TAB, "Kill/Headshot");

				IntInput KillAmount(&GROUP, "Kill amount");
				IntInput DeathAmount(&GROUP, "Death amount");
				IntInput HeadshotAmount(&GROUP, "Headshot amount");
				Button AddStat(&GROUP, "Add Kill/Headshot stat");

				#pragma region MenuFunctions
				void Load()
				{
					AddStat.OnClick([&]
					{
						AccountCommands::UpdateMonthlyStats(
							KillAmount.value,
							DeathAmount.value,
							HeadshotAmount.value,
							0,
							0
						);

						WebsocketCore::Reload();
					});
				}
				#pragma endregion
			}

			namespace Winstreak
			{
				Group GROUP(&TAB, "Killstreak/Winstreak");

				IntInput KillstreakAmount(&GROUP, "Killstreak amount");
				IntInput WinstreakAmount(&GROUP, "Winstreak amount");
				Button AddStat(&GROUP, "Add Killstreak/Winstreak stat");

				#pragma region MenuFunctions
				void Load()
				{
					AddStat.OnClick([&]
					{
						AccountCommands::UpdateMonthlyStats(
							0,
							0,
							0,
							KillstreakAmount.value,
							WinstreakAmount.value
						);

						WebsocketCore::Reload();
					});
				}
				#pragma endregion
			}


		}

		namespace ClanStuff
		{
			Tab TAB(&SECTION, "Clan Stuff");

			namespace ClanID
			{
				Group GROUP(&TAB, "Clan ID");
				Text NOTE(&GROUP,
					"READ ME: Enter your clan ID here.\n"
					"Entering invalid clan ID will cause clan mods not to work."
				);

				IntInput Input(&GROUP, "Clan ID");
			}

			namespace PlaceableUnlocker
			{

				Group GROUP(&TAB, "Placeable Unlocker");
				Mode UnlockMode(&GROUP, "Unlock mode", { "Automatic", "Manual" });

				Browser PlaceableBrowser(&GROUP, "Placeable browser", "PlaceUnlocker_manual");
				IntInput PlaceableAmount(&GROUP, "Placeable part amount");

				Button UnlockPlaceable(&GROUP, "Unlock placeable", "PlaceUnlocker_manual");
				Button UnlockAllPlaceable(&GROUP, "Unlock all placeable", "PlaceUnlocker_auto");

				#pragma region MenuFunctions
				void Load()
				{
					ContentKeyRegister::IterateKeyRegister(OfferItemType::ClanPlaceable, [&](IL2CPP::String* key, int index)
					{
						PlaceableBrowser.list.push_back(key->ToString());
					});

					UnlockAllPlaceable.OnClick([&]
					{
						ClanCommands::UnlockAllClanPlaceable(std::to_string(ClanID::Input.value), PlaceableAmount.value);
						WebsocketCore::Reload();
					});

					UnlockPlaceable.OnClick([&]
					{
						const std::string& target = PlaceableBrowser.list[PlaceableBrowser.index];

						ClanCommands::UnlockClanPlaceable(std::to_string(ClanID::Input.value), target, PlaceableAmount.value);
						WebsocketCore::Reload();
					});
				}

				void Update()
				{
					TagService::ToggleTagVisibility("PlaceUnlocker_auto", UnlockMode.index == 0);
					TagService::ToggleTagVisibility("PlaceUnlocker_manual", UnlockMode.index == 1);
				}
				#pragma endregion
			}

			namespace Misc
			{
				Group GROUP(&TAB, "Misc");
				Button PromoteClan(&GROUP, "Promote Clan", ButtonSizeType::WINDOW_SIZE);

				#pragma region MenuFunctions
				void Load()
				{
					PromoteClan.OnClick([&]
					{
						ClanCommands::PromoteClan(std::to_string(ClanID::Input.value));
						WebsocketCore::Reload();
					});
				}
				#pragma endregion
			}
		}
	}

	namespace Misc
	{
		Section SECTION(&WINDOW, ICON_FA_LIST);

		namespace Bypass
		{
			Tab TAB(&SECTION, "Bypasses");

			namespace Chat
			{
				Group GROUP(&TAB, "Chat");

				Checkbox BypassChatFilter(&GROUP, "Bypass Chat filter", true);

				#pragma region MenuFunctions
				void Load()
				{
					using namespace IL2CPP::ClassMapping;
					static void* ScanMatInWold_ptr = GetClass("FilterBadWorld")->GetMethod(0x2)->GetPointer();

					BypassChatFilter.OnToggle([&](bool value)
					{
						if (value)
						{
							MemPatcher::ReturnFalse(ScanMatInWold_ptr);
						}
						else
						{

							MemPatcher::Restore(ScanMatInWold_ptr);
						}
					});

					if(BypassChatFilter.value)
						MemPatcher::ReturnFalse(ScanMatInWold_ptr);
				}
				#pragma endregion
			}

			namespace Armory
			{
				Group GROUP(&TAB, "Armory");

				Checkbox AllowShovel(&GROUP, "Allow to use shovel");

				#pragma region MenuFunctions
				void Load()
				{
					AllowShovel.OnToggle([&](bool value)
					{
						WeaponMenager::GetItemRecordList()->ForEach([&](IL2CPP::Object* obj)
						{
							obj->GetFieldRef<bool>("campaignOnly") = !value;
							obj->GetFieldRef<bool>("isRoyale") = !value;
						});
					});
				}
				#pragma endregion
			}

			namespace Misc
			{
				Group GROUP(&TAB, "Misc");

				Text NOTE(&GROUP,
					"Progress will not be saved and no analytic logs will be sent\n"
					"while Safe Mode is enabled.\n\n"

					"Safe Mode doesn't work with several account mods."
				);

				Checkbox SafeMode(&GROUP, "Safe Mode");
				Button ForceReload(&GROUP, "Force reload", ButtonSizeType::WINDOW_SIZE);
				Button SkipTutorial(&GROUP, "Skip tutorial", ButtonSizeType::WINDOW_SIZE);

				#pragma region MenuFunctions
				void Load()
				{
					ForceReload.OnClick(WebsocketCore::Reload);
					SkipTutorial.OnClick([&] 
					{
						TutorialClass::Fill(TutorialClass::GetInstance());
						TrainingController::FinishTraining();
					});

					SafeMode.OnToggle([&](bool value)
					{
						if (value)
						{
							MemPatcher::Nop(ProgressUpdater::SaveProgress.GetPointer());
						}
						else
						{
							MemPatcher::Restore(ProgressUpdater::SaveProgress.GetPointer());
						}
					});
				}
				#pragma endregion
			}

			namespace Analytics
			{
				Group GROUP(&TAB, "Analytics");

				//Checkbox AnalyticsBypass(&GROUP, "Analytics bypass", true);
				Checkbox DeeperAnalyticsBypass(&GROUP, "Analytics bypass", true);

				#pragma region MenuFunctions
				void Load()
				{
					if (DeeperAnalyticsBypass.value)
					{
						AntiAnalytics::NopAnalyticsMethods();
					}

					DeeperAnalyticsBypass.OnToggle([&](bool value)
					{
						if (value)
						{
							AntiAnalytics::NopAnalyticsMethods();
						}
						else
						{
							AntiAnalytics::RestoreAnalyticsMethods();
						}
					});
				}
				#pragma endregion
			}
		}

		namespace Skin
		{
			Tab TAB(&SECTION, "Skin importer & stealer");

			namespace CustomSkinImporter
			{
				Group GROUP(&TAB, "Custom Skin Importer");
				Text NOTE(&GROUP,
					"Supported Skin Resolution: 64x64, 64x32.\n\n"
					"Pixel Gun doesn't support slim mc skin. Don't import them\n"
					"or the skin will break.\n\n"
					"Custom Skin with 64x64 resolution will be converted\n"
					"into 64x32 automatically."
				);

				Button ImportSkin(&GROUP, "Import Skin");
				#pragma region MenuFunctions
				void ShowErrorMsgBox(const char* msg)
				{
					ShowWindow(GetActiveWindow(), SW_SHOWMINIMIZED);
					MessageBoxA(
						nullptr,
						msg,
						"Error!",
						MB_OK | MB_ICONERROR
					);

					return;
				}

				void Load()
				{
					ImportSkin.OnClick([&]
					{
						std::optional<std::wstring> pathOpt = FileDialogService::GetFilepathLoad(L"Skin Texture (.png)\0*.png");
						if (!pathOpt.has_value()) return;

						Gdiplus::Bitmap textureBitmap(pathOpt.value().c_str());
						std::vector<BYTE> outBuffer;

						bool isSizeValid = textureBitmap.GetWidth() == 64 && textureBitmap.GetHeight() == 32;
						bool isConvertable = textureBitmap.GetWidth() == 64 && textureBitmap.GetHeight() == 64;

						if (!isSizeValid && !isConvertable)
						{
							ShowErrorMsgBox(
								"You loaded texture with invalid size.\n"
								"Make sure to check the texture size is correct then try again."
							);
							return;
						}

						if (isConvertable)
						{
							bool success = GdiplusManager::CropPng(
								&textureBitmap,
								Gdiplus::Rect(0, 0, 64, 32),
								Gdiplus::Size(64, 32),
								&outBuffer
							);

							if (!success)
							{
								ShowErrorMsgBox("Unable to crop texture due to Gdiplus failure.");
								return;
							}
						}
						else
						{
							GdiplusManager::ReadBitmapBytes(&textureBitmap, GdiplusManager::pngClsid, &outBuffer);
						}

						std::wstring skinName = pathOpt.value().substr(pathOpt.value().find_last_of(L"/\\") + 1);
						ProgressUpdater::UpdateCustomSkin(
							ProgressUpdater::GetInstance(),
							GetTickCount64(),
							IL2CPP::String::Create(skinName),
							IL2CPP::String::Create(base64_encode(outBuffer.data(), outBuffer.size()))
						);

						WebsocketCore::Reload();
					});
				}
				#pragma endregion
			}

			namespace SkinStealer
			{
				Group GROUP(&TAB, "Skin Stealer");
				Text NOTE(&GROUP,
					"Stolen skins will be saved on the selected folder."
				);
				IntInput TargetID(&GROUP, "Target ID");
				Button Steal(&GROUP, "Steal skins from target");

				#pragma region MenuFunctions
				std::wstring currentSavePath;

				void ParseSlotData(nlohmann::json& json)
				{
					if (!json.contains("status") || json.at("status") != "ok") return;
					nlohmann::json skins = json["slots"]["12"];

					for (nlohmann::json& val : skins)
					{
						std::string skinName = val["n"];
						std::string skinData = val["c"];
						std::string base64Bytes = base64_decode(skinData);

						auto savepath = std::filesystem::path(currentSavePath).append(skinName + ".png");

						DWORD imageSize = base64Bytes.length();
						HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, imageSize);
						memcpy(hMem, base64Bytes.c_str(), imageSize);

						IStream* pStream = nullptr;
						CreateStreamOnHGlobal(hMem, false, &pStream);

						Gdiplus::Image image(pStream);
						image.Save(savepath.wstring().c_str(), &GdiplusManager::pngClsid);
						pStream->Release();
						GlobalFree(hMem);
					}
				}

				void Load()
				{
					Steal.OnClick([&]
					{
						std::optional<std::wstring> pathOpt = FileDialogService::SelectFolder();
						if (!pathOpt.has_value()) return;

						currentSavePath = pathOpt.value();
						auto data = nlohmann::json::object({ 
							{"player_id", std::to_string(TargetID.value)}
						});
						WebsocketCore::QueuePackage("get_progress", data, ParseSlotData);
					});
				}
				#pragma endregion
			}

			//namespace ClanIconStealer
			//{
			//	Group GROUP(&TAB, "Clan Icon Stealer");
			//	Text NOTE(&GROUP,
			//		"Stolen icon will be saved on the selected folder."
			//	);
			//	IntInput TargetID(&GROUP, "Target Clan ID");
			//	Button Steal(&GROUP, "Steal clan icon");
			//}
		}
	}

	namespace Settings
	{
		Section SECTION(&WINDOW, ICON_FA_WRENCH);

		namespace Menu
		{
			Tab TAB(&SECTION, "Menu", UIComponents::GroupSplitType::NO_SPLIT);

			namespace MenuCustomization
			{
				Group GROUP(&TAB, "Menu Customization");

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
				void Load()
				{
					SetTheme.OnClick([&] 
					{
						UIFramework::Vars::gMenuColorScheme = colorSchemeList[Themes.index];
					});
				}
				#pragma endregion
			}

			//namespace MouseFix
			//{
			//	Group GROUP(&TAB, "Mouse Fix");

			//	Checkbox DisableGameClickEvent(&GROUP, "Disable Game click event", true);
			//	//Checkbox DisableCameraMovement(&GROUP, "Disable Camera movement", false);
			//}

			namespace Config
			{
				Group GROUP(&TAB, "Configuration Preset");

				Button LoadConfig(&GROUP, "Load config");
				Button SaveConfig(&GROUP, "Save config");

				#pragma region MenuFunctions
				const wchar_t* fileConfigFilter = L"Nazi configuration (.nazicfg)\0*.nazicfg";

				void Load()
				{
					LoadConfig.OnClick([&]
					{
						std::optional<std::wstring> pathOpt = FileDialogService::GetFilepathLoad(fileConfigFilter);
						if (!pathOpt.has_value()) return;
						UIFramework::ConfigManager::LoadConfig(pathOpt.value());
					});

					SaveConfig.OnClick([&]
					{
						std::optional<std::wstring> pathOpt = FileDialogService::GetFilepathSave(fileConfigFilter);
						if (!pathOpt.has_value()) return;
						UIFramework::ConfigManager::SaveConfig(pathOpt.value());
					});
				}
				#pragma endregion
			}
		}

		namespace Credit
		{
			Tab TAB(&SECTION, "Credits & info", UIComponents::GroupSplitType::NO_SPLIT);

			Group GROUP(&TAB, "Credits & info", {-1, -1});
			Text NOTE(&GROUP,
				"Nazi Mod version: v5.0.0\n"
				"\n"

				"Cheat developers:\n"
				"- @soto_sapi1\n"
				"\n"

				"Special credits:\n"
				"- @BoredKarma for websocket exploit (i skidded his ws from stardust src)\n"
				"- @sxitxma for contributing adding several features.\n"
				"- @.xdCraze (yes, i pasted his imgui widgets)\n"
				"- @proplam (pasting)\n"
				"- @7ddf (leaking, pasting and being a nigger)\n"
				"\n"

				"Used libraries credits:\n"
				"- Dear ImGui: https://github.com/ocornut/imgui\n"
				"- Kiero: https://github.com/Rebzzel/kiero\n"
				"- TulipHook: https://github.com/geode-sdk/TulipHook\n"
				"- Proplamatic IL2CPP: https://github.com/sotoSapi1/proplamatic-il2cpp\n"
				"- Nlohmann's Json: https://github.com/nlohmann/json\n"
				"- WinReg: https://github.com/GiovanniDicanio/WinReg\n"
				"- Renenyffenegger's Base64:\n"
				"  https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp/\n"
				"- obfusheader.h: https://github.com/ac3ss0r/obfusheader.h\n"
				"\n"

				"Fonts credits:\n"
				"- Builder sans: https://online-fonts.com/fonts/builder-sans\n"
				"\n"

				"Paste credits:\n"
				"- ZygiskPG: https://github.com/fedes1to/ZygiskPG\n"
				"- Stardust: https://github.com/fedes1to/Stardust-PG3D-Menu\n"
			);
			Button OpenDC(&GROUP, "Nazi Mod Discord server");

			#pragma region MenuFunctions
			void Load()
			{
				OpenDC.OnClick([&]
				{
					ShellExecuteA(0, 0, "https://discord.gg/Y3gj2Rszq6", 0, 0, SW_SHOW);
				});
			}
			#pragma endregion
		}
	}
	#pragma endregion

	#ifdef _DEBUG
	namespace Debug
	{
		Section SECTION(&WINDOW, ICON_FA_DEBUG);

		namespace DebugTab
		{
			Tab TAB(&SECTION, "Debug");

			namespace Websocket
			{
				Group GROUP(&TAB, "Websocket");

				Checkbox LogWs(&GROUP, "Log Websocket", true);

				void Update()
				{
					WebsocketCore::logWebsocket = LogWs.value;
				}
			}

			namespace Dumper
			{
				Group GROUP(&TAB, "Dumper");

				Button RunDumper(&GROUP, "Run Dumper");

				void Load()
				{
					RunDumper.OnClick([&]
					{
						IL2CPP::Dumper::StartDumper(
							Convert::ToString(GetLoaderPath() + L"\\dumps")
						);
					});
				}
			}

			namespace Squad
			{
				Group GROUP(&TAB, "Squad Spam test");

				StringInput SquadHash1(&GROUP, "Squad Hash 1", "");
				StringInput SquadHash2(&GROUP, "Squad Hash 2", "");
				StringInput Name(&GROUP, "Name", "");
				StringInput Receiver(&GROUP, "Receiver", "");

				Button Test(&GROUP, "Test");

				void Load()
				{
					Test.OnClick([&]
					{
						SquadCommands::InSquad(SquadHash1.GetValue());
						SquadCommands::SendSquadInvite(
							SquadHash2.GetValue(),
							Name.GetValue(),
							Receiver.GetValue()
						);
					});
				}
			}

			namespace ItemRecord
			{
				Group GROUP(&TAB, "Item Record");
				Button DumpItemRecord(&GROUP, "Dump Item Records");

				std::map<std::string, int> offerItemType = {
					{"None", 0},
					{"Weapon", 10},
					{"Armor", 20},
					{"Mask", 30},
					{"Hat", 40},
					{"Boots", 50},
					{"Cape", 60},
					{"Skin", 65},
					{"Gadget", 70},
					{"Pet", 80},
					{"Egg", 83},
					{"LobbyItem", 85},
					{"FortItem", 90},
					{"Gems", 1000},
					{"Coins", 1010},
					{"Leprechaun", 1020},
					{"WeaponUpgrade", 1030},
					{"GachaFreeSpin", 1040},
					{"EventCurrency", 1050},
					{"VIP", 1060},
					{"Parts", 1070},
					{"Royale", 1080},
					{"BattlePassLevel", 1090},
					{"BattlePassExp", 1100},
					{"BattlePassCurrency", 1110},
					{"GoldenSkin", 1120},
					{"EventChest", 1130},
					{"CraftCurrency", 1140},
					{"Module", 1150},
					{"ModulePoint", 1155},
					{"ModuleChest", 1160},
					{"WeaponSkin", 1170},
					{"ClanCurrency", 1180},
					{"Coupons", 1190},
					{"Currency", 1200},
					{"Character", 1210},
					{"ClanShields", 1220},
					{"ClanLootBox", 1230},
					{"ClanPlaceable", 1240},
					{"ClanPlaceablePoint", 1250},
					{"Detail", 1300},
					{"WeaponLevelUpgrade", 1310},
					{"PlayerBuff", 1320},
					{"ClanBuff", 1330},
					{"WeaponQualityUpgrade", 1340},
					{"ArmorSkin", 1350},
					{"ClanBuilding", 1360},
					{"ClanBuildingPoint", 1370},
					{"FreeUpgrade", 1380},
					{"Chest", 1390},
					{"Exp", 1400},
					{"Stats", 1410},
					{"ModeSlots", 1420},
					{"Executable", 1430},
					{"Tank", 1440},
					{"VIP20", 1450},
					{"LootBox", 1460},
					{"Graffiti", 1470},
					{"PixelPassExp", 1490},
					{"ClanRankExperience", 1500},
					{"WearSkin", 1510},
					{"Applicable", 1520},
					{"CraftSet", 1530},
					{"FeatureExp", 1540},
					{"PackagedItem", 1550},
					{"Achievement", 1560},
					{"ExpirySimple", 1570},
					{"Static", 1580},
					{"GemsHarvester", 1590},
					{"TreasureMap", 1600}
				};

				void Load()
				{
					DumpItemRecord.OnClick([&]
					{
						std::optional<std::wstring> pathOpt = FileDialogService::GetFilepathSave(L"Select a file (.txt)\0*.txt");

						if (!pathOpt.has_value()) return;
						std::ofstream dumpStream(pathOpt.value(), std::ios_base::trunc);

						for (auto v : offerItemType)
						{
							dumpStream << "# " << v.first << " (" + std::to_string(v.second) + ") : " << std::endl;
							ContentKeyRegister::IterateKeyRegister((OfferItemType)v.second, [&](IL2CPP::String* x, int index)
							{
								dumpStream << x->ToString() << " (" << index << ")" << std::endl;
							});

							dumpStream << std::endl;
						}

						dumpStream.flush();
					});
				}
			}
		}
	}
	#endif

	void OnUpdate()
	{
		static bool iUnderstand = false;
		static bool errorShown = false;

		if (ImGui::IsKeyPressed(ImGuiKey_F1) || ImGui::IsKeyPressed(ImGuiKey_RightCtrl) || ImGui::IsKeyPressed(ImGuiKey_RightAlt))
		{
			Account::Unlocker::WeaponUnlocker::WeaponLevel.value = Global::gPlayerLevel;
			gMenuShown = !gMenuShown;
		}

		MouseFix::ShowMouse(gMenuShown);

		if (!iUnderstand && gMenuShown)
		{
			UIFramework::DisclaimerWindow(ImVec2(900, 600), [&] {iUnderstand = true; });
			return;
		}

		if (Gameplay::General::Aim::Aimbot.value && Gameplay::General::Aim::FOVCircle.value)
		{
			auto background = ImGui::GetBackgroundDrawList();
			auto screenCenter = Vector2(Screen::GetWidth() / 2, Screen::GetHeight() / 2);
			background->AddCircle(
				ImVec2(screenCenter.X, screenCenter.Y), 
				Gameplay::General::Aim::AimbotFOV.value, 
				ImColor(255, 0, 0), 
				64, 
				2.0f
			);
		}

		if (gMenuShown)
		{
			WINDOW.Render();
		}

		Gameplay::General::Player::Update();
		Gameplay::General::Aim::Update();
		Gameplay::ServerMods::PrefabSpawner::Update();
		Account::Unlocker::WeaponSkinUnlocker::Update();
		Account::Unlocker::WeaponUnlocker::Update();
		Account::Unlocker::RoyaleUnlocker::Update();
		Account::Unlocker::GadgetUnlocker::Update();
		Account::Unlocker::ArmorUnlocker::Update();
		Account::ClanStuff::PlaceableUnlocker::Update();

		#ifdef _DEBUG
		Debug::DebugTab::Websocket::Update();
		#endif 
	}

	void INIT()
	{
		Account::Unlocker::WeaponSkinUnlocker::Load();
		Account::Unlocker::WeaponUnlocker::Load();
		Account::Unlocker::RoyaleUnlocker::Load();
		Account::Unlocker::GadgetUnlocker::Load();
		Account::Unlocker::ArmorUnlocker::Load();
		Account::Unlocker::ModuleUnlocker::Load();
		Account::Unlocker::PixelPassUnlocker::Load();
		Account::Unlocker::MiscUnlocker::Load();
		Account::Adder::XpEditor::Load();
		Account::Adder::CurrencyAdder::Load();
		Account::Adder::BuffAdder::Load();
		Account::Adder::ChestAdder::Load();
		Account::Stats::ClanRank::Load();
		Account::Stats::GlobalWin::Load();
		Account::Stats::MonthlyMatch::Load();
		Account::Stats::Kill::Load();
		Account::Stats::Winstreak::Load();
		Account::ClanStuff::PlaceableUnlocker::Load();
		Account::ClanStuff::Misc::Load();
		Misc::Bypass::Chat::Load();
		Misc::Bypass::Armory::Load();
		Misc::Bypass::Misc::Load();
		Misc::Bypass::Analytics::Load();
		Misc::Skin::CustomSkinImporter::Load();
		Misc::Skin::SkinStealer::Load();
		Settings::Menu::MenuCustomization::Load();
		Settings::Menu::Config::Load();
		Settings::Credit::Load();

		#ifdef _DEBUG
		Debug::DebugTab::ItemRecord::Load();
		Debug::DebugTab::Squad::Load();
		Debug::DebugTab::Dumper::Load();
		#endif 

		UIFramework::INIT(
			Convert::ToString(GetLoaderPath()).append("\\assets"),
			Themes::darkLavenderBlue,
			OnUpdate
		);
	}
}