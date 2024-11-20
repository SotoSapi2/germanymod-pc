#include "HookTracer.hpp"
#include <Logger.hpp>

#include "../util/MemoryUtil.hpp"

#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")

namespace HookTracer
{
	constexpr BYTE JMP = 0xE9;

	const BYTE gapBytes[3] = { 0xCC, 0xCC, 0xCC };

	void LogOffsetFromTrampoline(BYTE* trampolineAddreas)
	{
		MEMORY_BASIC_INFORMATION memInfo;
		if (VirtualQuery(trampolineAddreas, &memInfo, sizeof(memInfo)))
		{
			BYTE* base = (byte*) memInfo.AllocationBase;
			uintptr_t offset = trampolineAddreas - base;

			LOG_NOTAG("[HOOK DETECTED]");
			LOG_NOTAG("method offset: 0x%x", (uintptr_t) offset);
			LOG_NOTAG("sus jmp address: %p\n", (void*)trampolineAddreas);
		}
	}

	PIMAGE_SECTION_HEADER FindIl2CppSection(const std::unique_ptr<BYTE[]>& moduleBytes)
	{
		constexpr const char* TARGET_SECTION = "il2cpp";

		PIMAGE_NT_HEADERS ntHeader = ImageNtHeader(moduleBytes.get());
		int sectionCount = ntHeader->FileHeader.NumberOfSections;
		PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeader);

		for (int i = 0; i < sectionCount; ++i, ++section)
		{
			if (strcmp((const char*)section->Name, TARGET_SECTION) == 0)
			{
				return section;
			}
		}
	}

	void FindTrampolinesAndLog(bool compareGameAssembly)
	{
		std::optional<Memory::ModuleInfo> moduleInfoOpt = Memory::FindModule("GameAssembly.dll");

		if (!moduleInfoOpt.has_value())
		{
			LOG_ERROR("Couldn't find GameAssembly.dll module.");
			return;
		}

		Memory::ModuleInfo gameAssembly = moduleInfoOpt.value();
		std::unique_ptr<BYTE[]> gameAssemblyBytes = Memory::ReadBytes(gameAssembly.BaseAddress(), gameAssembly.imageSize);
		std::ifstream gameAssemblyFilestream(gameAssembly.filepath, std::ios::binary);

		PIMAGE_SECTION_HEADER il2CppSection = FindIl2CppSection(gameAssemblyBytes);
		std::string_view sectionView(
			(char*)gameAssemblyBytes.get() + il2CppSection->VirtualAddress, 
			il2CppSection->SizeOfRawData
		);

		std::vector<uint8_t*> jmps;
		for (size_t it = sectionView.find(JMP, 0); it != sectionView.npos; it = sectionView.find(JMP, ++it))
		{
			jmps.push_back(gameAssembly.BaseAddress() + il2CppSection->VirtualAddress + it);
		}

		BYTE filebytes[5] = { 0 };

		for (BYTE* jmp : jmps)
		{
			if (compareGameAssembly)
			{
				DWORD offset = jmp - gameAssembly.BaseAddress();
				DWORD sectionOffset = offset - il2CppSection->VirtualAddress;
				DWORD fileOffset = il2CppSection->PointerToRawData + sectionOffset;

				gameAssemblyFilestream.seekg(fileOffset, std::ios::beg);
				gameAssemblyFilestream.read((char*)filebytes, 5);

				if (!memcmp(jmp, filebytes, 5)) continue;

				LogOffsetFromTrampoline(jmp);
			}
			else
			{
				int jmpOffset;
				memcpy(&jmpOffset, jmp + 1, 4);
				uint8_t* jmpDest = jmp + (jmpOffset + 5);

				if (jmpDest < gameAssembly.BaseAddress() || jmpDest > gameAssembly.EndAddreas())
				{
					MEMORY_BASIC_INFORMATION memInfo;

					if (memcmp(jmp - 3, gapBytes, 3))
					{
						continue;
					}

					if (!VirtualQuery(jmp, &memInfo, sizeof(memInfo)))
					{
						continue;
					}

					if (memInfo.Protect != PAGE_EXECUTE_READ)
					{
						continue;
					}

					if (!VirtualQuery(jmpDest, &memInfo, sizeof(memInfo)))
					{
						continue;
					}

					if (memInfo.Protect != PAGE_EXECUTE_READWRITE)
					{
						continue;
					}

					LogOffsetFromTrampoline(jmp);
				}
			}
		}
	}
}