#pragma once
#include "../PointerWrapper.hpp"

namespace ExperienceController
{
	extern Pointer<int(Il2CppObject* experienceController)> GetCurrentLevel;

	extern Pointer<void(Il2CppObject* experienceController, int value, int reason, Il2CppObject* dictionary, Il2CppObject* sex)> AddExperience;
}