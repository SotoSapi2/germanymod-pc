#pragma once
#include <IL2CPP.hpp>
#include <unordered_map>
#include <vector>

namespace DumpsterFire
{
	extern std::unordered_map<char, std::vector<std::pair<int, int>>> gTextVectorMap;
	extern std::vector<Vector3> gPenisVectorArray;
	extern std::vector<const char*> gNameSpoofList;
}