#pragma once
#include <Windows.h>

#include <unordered_map>
#include <vector>
#include <optional>
#include <string>
#include <memory>


#define AUTO_ASSEMBLE_TRAMPOLINE(ADDRESS, TRAMPOLINE_LENGTH, INSTRUCTIONS) \
do { \
auto allocMemory = Memory::AllocateNearbyMemory(ADDRESS, sizeof INSTRUCTIONS + 14); \
Memory::CreateTrampoline(ADDRESS, allocMemory, TRAMPOLINE_LENGTH); \
Memory::WriteInstructions(allocMemory, INSTRUCTIONS, sizeof INSTRUCTIONS, ADDRESS + TRAMPOLINE_LENGTH); \
} while (false)

namespace Memory
{
	struct ModuleInfo
	{
		std::string filename;
		std::string filepath;
		DWORD imageSize;
		HMODULE handle;

		constexpr BYTE* BaseAddress()
		{
			return (BYTE*)handle;
		}

		constexpr BYTE* EndAddreas()
		{
			return (BYTE*)handle + imageSize;
		}

		~ModuleInfo();
	};

	std::optional<ModuleInfo> FindModule(const char* moduleName);

	std::unique_ptr<BYTE[]> ReadBytes(void* address, size_t len);

	void PatchBytes(void* address, const char* bytes, size_t len);

	void PatchBytes(uintptr_t address, const char* bytes);

	template <size_t N>
	void PatchBytes(uintptr_t address, const BYTE(&bytes)[N]);

	void NopBytes(uintptr_t address, size_t len);

	void RestoreBytes(uintptr_t address);
	void RestoreBytes(std::initializer_list<uintptr_t> address);

	uintptr_t PatternScan(uintptr_t module, const char* signature);

	void* AllocateNearbyMemory(uintptr_t address, size_t size);

	void CreateTrampoline(uintptr_t address, void* destination, size_t length);

	void RemoveTrampoline(uintptr_t address);

	void WriteInstructions(void* destination, const BYTE instructions[], size_t instructionLen, uintptr_t retAddress);

	void RestoreAllPatches();
};