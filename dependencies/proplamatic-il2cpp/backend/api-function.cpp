#include <Windows.h>

#define DO_API_DEFINE
#include "api-function.hpp"
#undef DO_API_DEFINE

namespace IL2CPP
{
    // pasted from https://github.com/Sxitxma/Windows-IL2CPP-Runtime-Dumper/blob/master/src/il2cpp_dump.cpp
	std::string GetProtectedExportName(HMODULE pe_base)
	{
		/* SXITXMA
		const std::string suffix = "_wasting_your_life";
		std::string fullName;
		std::ifstream file("GameAssembly.dll", std::ios::binary);
		assert(!file && "Error Occured when trying to open the GameAssembly dll file!");
		std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		size_t pos = fileContent.find(suffix);
		if (pos != std::string::npos) {
			size_t start = pos;
			while (start > 0 && (isalnum(fileContent[start - 1]) || fileContent[start - 1] == '_')) { --start; }
			fullName = fileContent.substr(start, pos - start + suffix.length());
		}
		return fullName.empty() ? "il2cpp_domain_get_assemblies" : fullName;
		*/
		// NULLBIT
		PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)pe_base;
		PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS)((BYTE*)dos_header + dos_header->e_lfanew);

		PIMAGE_OPTIONAL_HEADER optional_header = (PIMAGE_OPTIONAL_HEADER)&nt_headers->OptionalHeader;
		PIMAGE_DATA_DIRECTORY export_data_directory = &(optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
		PIMAGE_EXPORT_DIRECTORY export_directory = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)pe_base + export_data_directory->VirtualAddress);

		DWORD name_count = export_directory->NumberOfNames;
		PDWORD export_name_table = (PDWORD)((BYTE*)pe_base + export_directory->AddressOfNames);

		std::string protected_export = "_wasting_your_life";

		for (DWORD i = 0; i < export_directory->NumberOfNames; i++)
		{
			char* name = (char*)((BYTE*)pe_base + export_name_table[i]);
			std::string name_buf = std::string(name);
			if (name_buf.find(protected_export) != std::string::npos)
			{
				return name_buf;
			}
		}

		return "il2cpp_domain_get_assemblies";
	}

	void import_api(HMODULE gameAssemblyHandle)
	{
		#define DO_API_DECLARE
		#include "api-function.hpp"
		#undef DO_API_DECLARE
	}

	void INIT_API_FUNCTIONS(HMODULE gameAssemblyHandle)
	{
		import_api(gameAssemblyHandle);

		if (IMPORT::il2cpp_domain_get_assemblies == nullptr)
		{
			IMPORT::il2cpp_domain_get_assemblies = (const IMPORT::Il2CppAssembly * *(*)(const IMPORT::Il2CppDomain*, size_t*)) GetProcAddress(
				gameAssemblyHandle,
				GetProtectedExportName(gameAssemblyHandle).c_str()
			);
		}

		while (!IMPORT::il2cpp_is_vm_thread(nullptr))
		{
			Sleep(1);
		}
	}
}

