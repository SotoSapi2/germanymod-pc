#include "DumperMenu.hpp"

#include <Windows.h>
#include <Logger.hpp>
#include <imgui.h>
#include <FileUtils.h>

#include "../algorithm/Dumper.hpp"
#include "../unity/Unity.hpp"
#include "../framework/UIManager.hpp"
#include "../framework/FileDialogService.hpp"
#include "../util/Converter.hpp"
#include "../entrypoint.hpp"

#include "../unity/mono-wrapper/MonoString.hpp"
#include "../unity/mono-wrapper/MonoArray.hpp"

namespace DumperMenu
{
	char obfuscationRegex[1000] = "^[a-zA-Z0-9\\s.,;:!?()'\"-]+$";
	std::string savepath = ".";
	bool generatePattern = true;

	Il2CppDomain* il2cppDomain = nullptr;
	const Il2CppImage* corlib = nullptr;

	Il2CppObject* (*GetCurrentDomain)();
	Il2CppArray* (*GetAssemblies)(Il2CppObject* domain);
	Il2CppObject* (*GetAssemblyName)(Il2CppObject* assembly);
	MonoString* (*AssemblyName_getName)(Il2CppObject* assemblyName);

	void DumpIl2CppAssemblies()
	{
		il2cpp_thread_attach(il2cppDomain);

		if (il2cpp_domain_get_assemblies != nullptr) // idfk for some unity game this export doesnt exist.
		{
			size_t assembliesCount = 0;
			const Il2CppAssembly** assemblies = il2cpp_domain_get_assemblies(il2cppDomain, &assembliesCount);

			for (size_t i = 0; i < assembliesCount; i++)
			{
				const Il2CppAssembly* assembly = assemblies[i];

				Dumper::DumpImage(
					assembly->aname.name, 
					savepath.c_str(),
					generatePattern
				);
			}
		}
		else // if il2cpp_domain_get_assemblies is null, then use reflection to Assembly names.
		{
			MonoArray<Il2CppObject*>* assemblies = (MonoArray<Il2CppObject*>*) GetAssemblies(GetCurrentDomain());

			assemblies->foreach([&](Il2CppObject* assembly)
			{
				MonoString* monoAssemblyName = AssemblyName_getName(GetAssemblyName(assembly));
				std::string assemblyName = std::string(monoAssemblyName->ToUtf8()) + ".dll";

				Dumper::DumpImage(
					assemblyName.c_str(),
					savepath.c_str(),
					generatePattern
				);
			});
		}
	}

	void MenuUpdate()
	{
		if (ImGui::BeginTabItem("Dumper"))
		{
			ImGui::InputText("Obfuscation Regex", obfuscationRegex, std::size(obfuscationRegex));
			ImGui::Checkbox("Generate Pattern", &generatePattern);

			if (ImGui::Button("Select dump folder"))
			{
				std::optional<std::string> newSavepathOpt = FileDialogService::SelectFolder();

				if (newSavepathOpt.has_value())
				{
					savepath = newSavepathOpt.value();
				}
				else
				{
					LOG_ERROR("Folder selection failure.");
				}
			}
			ImGui::SameLine();
			ImGui::Text("Dump folder: %s", savepath.c_str());

			if (ImGui::Button("Dump"))
			{
				DumpIl2CppAssemblies();
			}

			ImGui::EndTabItem();
		}
	}

	void INIT()
	{
		il2cppDomain = il2cpp_domain_get();
		corlib = il2cpp_get_corlib();

		Il2CppClass* AppDomainClass = il2cpp_class_from_name(corlib, "System", "AppDomain");
		Il2CppClass* Assembly = il2cpp_class_from_name(corlib, "System.Reflection", "Assembly");
		Il2CppClass* AssemblyName = il2cpp_class_from_name(corlib, "System.Reflection", "AssemblyName");

		GetCurrentDomain = (Il2CppObject*(*)()) Il2CppUtils::GetMethodPointerByName(
			AppDomainClass, 
			"get_CurrentDomain"
		);

		GetAssemblies = (Il2CppArray * (*)(Il2CppObject*)) Il2CppUtils::GetMethodPointerByName(
			AppDomainClass,
			"GetAssemblies"
		);

		GetAssemblyName = (Il2CppObject * (*)(Il2CppObject*)) Il2CppUtils::GetMethodPointerByIndex(
			Assembly,
			0x19 // Assembly.GetName()
		);

		AssemblyName_getName = (MonoString * (*)(Il2CppObject*)) Il2CppUtils::GetMethodPointerByIndex(
			AssemblyName,
			0x4 // AssemblyName.get_FullName()
		);

		savepath = Convert::ToString(GetLoaderPath()) + "\\dumps";

		if (!std::filesystem::exists(savepath) || !std::filesystem::is_directory(savepath))
		{
			std::filesystem::create_directory(savepath);
		}

		UIManager::RegisterUIUpdate(MenuUpdate);
	}
}