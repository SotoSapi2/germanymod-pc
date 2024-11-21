#include "ReverseToolMenu.hpp"

#include <Windows.h>
#include <Logger.hpp>
#include <imgui.h>
#include <FileUtils.h>

#include "../algorithm/HookTracer.hpp"
#include "../unity/Unity.hpp"
#include "../framework/UIManager.hpp"
#include "../framework/FileDialogService.hpp"
#include "../util/Converter.hpp"
#include "../entrypoint.hpp"

namespace ReverseToolMenu
{
	bool compareGameAssembly = true;

	void MenuUpdate()
	{
		if (ImGui::BeginTabItem("Reversing Tool"))
		{
			if (ImGui::CollapsingHeader("Trampoline Tracer (might be inaccurate)"))
			{
				ImGui::Checkbox("Compare with GameAssembly.dll file", &compareGameAssembly);
				if (ImGui::Button("Trace Trampolines"))
				{
					HookTracer::FindTrampolinesAndLog(compareGameAssembly);
				}
			}
				
			ImGui::EndTabItem();
		}
	}

	void INIT()
	{
		UIManager::RegisterUIUpdate(MenuUpdate);
	}
}