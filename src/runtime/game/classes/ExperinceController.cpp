#include "ExperinceController.hpp"

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