/*
████████╗██╗░░██╗██╗░██████╗  ░██████╗░█████╗░██╗░░░██╗██████╗░░█████╗░███████╗
╚══██╔══╝██║░░██║██║██╔════╝  ██╔════╝██╔══██╗██║░░░██║██╔══██╗██╔══██╗██╔════╝
░░░██║░░░███████║██║╚█████╗░  ╚█████╗░██║░░██║██║░░░██║██████╔╝██║░░╚═╝█████╗░░
░░░██║░░░██╔══██║██║░╚═══██╗  ░╚═══██╗██║░░██║██║░░░██║██╔══██╗██║░░██╗██╔══╝░░
░░░██║░░░██║░░██║██║██████╔╝  ██████╔╝╚█████╔╝╚██████╔╝██║░░██║╚█████╔╝███████╗
░░░╚═╝░░░╚═╝░░╚═╝╚═╝╚═════╝░  ╚═════╝░░╚════╝░░╚═════╝░╚═╝░░╚═╝░╚════╝░╚══════╝

███████╗██╗██╗░░░░░███████╗  ██╗░██████╗
██╔════╝██║██║░░░░░██╔════╝  ██║██╔════╝
█████╗░░██║██║░░░░░█████╗░░  ██║╚█████╗░
██╔══╝░░██║██║░░░░░██╔══╝░░  ██║░╚═══██╗
██║░░░░░██║███████╗███████╗  ██║██████╔╝
╚═╝░░░░░╚═╝╚══════╝╚══════╝  ╚═╝╚═════╝░

██╗░░░██╗███╗░░██╗██████╗░███████╗░█████╗░██████╗░░█████╗░██████╗░██╗░░░░░███████╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗
██║░░░██║████╗░██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔══██╗██╔══██╗██║░░░░░██╔════╝██║██║██║██║██║██║██║██║██║██║██║██║██║██║
██║░░░██║██╔██╗██║██████╔╝█████╗░░███████║██║░░██║███████║██████╦╝██║░░░░░█████╗░░██║██║██║██║██║██║██║██║██║██║██║██║██║██║
██║░░░██║██║╚████║██╔══██╗██╔══╝░░██╔══██║██║░░██║██╔══██║██╔══██╗██║░░░░░██╔══╝░░╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝
╚██████╔╝██║░╚███║██║░░██║███████╗██║░░██║██████╔╝██║░░██║██████╦╝███████╗███████╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗██╗
░╚═════╝░╚═╝░░╚══╝╚═╝░░╚═╝╚══════╝╚═╝░░╚═╝╚═════╝░╚═╝░░╚═╝╚═════╝░╚══════╝╚══════╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝╚═╝

TODO:
- Fix flickering components when you load subtab for the first time. This bug most likey a ImGui bug.
- Put more effort for flexible Button type.
- Rewrite and finish notification system. (imgui_notify.h suck ass)
- Clean up and refactor this choatic mess.
- Hanging every Zack Zuckerberg slaves' on a rope for who contributing inventing React.
*/

#include "UIFramework.hpp"
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <optional>
#include <imgui_internal.h>
#include <sstream>
#include <regex>
//#include <imgui_notify.hpp>
#include "backend/UIBackend.hpp"
#include <Logger.hpp>
#include <json.hpp>
#include <windows.h>
#include <obfuscator.h>
#include "TaskScheduler.hpp"

#define IS_ODD(x) (x % 2 == 1)

static inline ImColor ImLerp(const ImColor& a, const ImColor& b, float t)
{
	return ImColor(
		a.Value.x + (b.Value.x - a.Value.x) * t,
		a.Value.y + (b.Value.y - a.Value.y) * t,
		a.Value.z + (b.Value.z - a.Value.z) * t,
		a.Value.w + (b.Value.w - a.Value.w) * t
	);
}

template<typename T>
class DataContainer
{
	std::unordered_map<ImGuiID, std::unordered_map<uint8_t, T*>> container;

public:
	T* Get(const ImGuiID& id, uint8_t slot, T defaultValue)
	{
		if (container.find(id) == container.end())
		{
			container[id] = std::unordered_map<uint8_t, T*>{};
		}

		if (container.at(id).find(slot) == container.at(id).end())
		{
			auto data = new T;
			*data = defaultValue;
			container.at(id).insert({ slot, data });
			return data;
		}

		return container.at(id).at(slot);
	}

	T* Get(const ImGuiID& id, uint8_t slot)
	{
		if (container.find(id) == container.end())
		{
			container[id] = std::unordered_map<uint8_t, T*>{};
		}

		if (container.at(id).find(slot) == container.at(id).end())
		{
			auto data = new T;
			container.at(id).insert({ slot, data });
			return data;
		}

		return container.at(id).at(slot);
	}
};

namespace InterpHelper
{
	float Lerp(float x, float min = 0.0f, float max = 1.0f)
	{
		return std::clamp(x, min, max);
	}
}

namespace UIFramework
{
	//struct GroupQueue
	//{
	//	const char* label;
	//	GroupPlacementType placement;
	//	std::function<void()> drawer;
	//};

	namespace Vars
	{
		// PUBLIC FIELDS //
		ImFont* gRegularFont = nullptr;
		ImFont* gMediumFont = nullptr;
		ImFont* gLargeFont = nullptr;
		ImFont* gExtraLargeFont = nullptr;
		ImFont* gIconFont = nullptr;

		MenuColorScheme gMenuColorScheme;
		//

		std::string assetsPath;
		ImGuiID globalSelectedSectionId = 0;
		ImGuiID globalSelectedTabId = 0;

		UIComponents::Section* currentSelectedSection = nullptr;

		UIComponents::Tab* currentSelectedTab = nullptr;

		DataContainer<float> floatContainer;
		DataContainer<ImColor> colorContainer;
		DataContainer<ImRect> rectContainer;

		std::function<void()> onUpdateCallback;
	};

	char AciiToLower(char in)
	{
		if (in <= 'Z' && in >= 'A')
			return in - ('Z' - 'z');
		return in;
	}

	std::vector<std::pair<int, const std::string&>> SearchSubstring(const std::vector<std::string>* vec, const std::string& target, bool caseSenstive = false, int maxResult = 0)
	{
		std::vector<std::pair<int, const std::string&>> results;

		int resultFound = 0;
		for (int i = 0; i < vec->size(); i++)
		{
			if (maxResult != 0 && resultFound > maxResult)
			{
				break;
			}
			const std::string& origStr = vec->at(i);

			if (caseSenstive)
			{
				if (origStr.find(target) != std::string::npos)
				{
					resultFound++;
					results.push_back({ i, origStr });
				}
			}
			else
			{
				std::string str = origStr;
				std::transform(str.begin(), str.end(), str.begin(), AciiToLower);

				if (str.find(target) != std::string::npos)
				{
					resultFound++;
					results.push_back({ i, origStr });
				}
			}
		}

		return results;
	}

	namespace Widgets
	{
		void RenderTextDisabled(bool isDisabled, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end,
			const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
		{
			const char* text_display_end = ImGui::FindRenderedTextEnd(text, text_end);
			const int text_len = (int)(text_display_end - text);
			if (text_len == 0)
				return;

			ImGuiContext& g = *GImGui;
			ImGuiWindow* window = g.CurrentWindow;
			ImDrawList* draw_list = window->DrawList;

			ImVec2 pos = pos_min;
			const ImVec2 text_size = text_size_if_known ? *text_size_if_known : ImGui::CalcTextSize(text, text_display_end, false, 0.0f);

			const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
			const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;
			bool need_clipping = (pos.x + text_size.x >= clip_max->x) || (pos.y + text_size.y >= clip_max->y);
			if (clip_rect) // If we had no explicit clipping rectangle then pos==clip_min
				need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

			// Align whole block. We should defer that to the better rendering function when we'll have support for individual line alignment.
			if (align.x > 0.0f) pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
			if (align.y > 0.0f) pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

			// Render
			if (need_clipping)
			{
				ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
				draw_list->AddText(NULL, 0.0f, pos, ImGui::GetColorU32(isDisabled ? ImGuiCol_TextDisabled : ImGuiCol_Text), text, text_display_end, 0.0f, &fine_clip_rect);
			}
			else
			{
				draw_list->AddText(NULL, 0.0f, pos, ImGui::GetColorU32(isDisabled ? ImGuiCol_TextDisabled : ImGuiCol_Text), text, text_display_end, 0.0f, NULL);
			}
		}

		void Frame(const char* str_id, const ImVec2& size = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0)
		{
			ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4());
			ImGui::BeginChild(str_id, size, child_flags, window_flags);
			ImGui::PopStyleColor();
		}

		void EndFrame()
		{
			ImGui::EndChild(); // crazy rename
		}

		void Group(UIComponents::Group* group, const ImVec2& size = { 0, 0 })
		{
			std::string strId = group->label + "-group";

			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);

			if (size.x == 0 && size.y == 0)
			{
				ImGui::BeginChild(strId.c_str(), { -1.0, 0.0 }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding);
			}
			else
			{
				ImGui::BeginChild(strId.c_str(), size, ImGuiChildFlags_AlwaysUseWindowPadding);
			}

			ImGui::PopStyleVar();
			ImGui::PushFont(Vars::gLargeFont);
			ImGui::TextUnformatted(group->label.c_str());
			ImGui::PopFont();
			ImGui::Separator();

			group->RenderChildren();

			ImGui::EndChild();
		}

		void Tab(UIComponents::Tab* tab)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
			{
				return;
			}
			ImGui::SameLine();
			ImGui::PushFont(Vars::gLargeFont);

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(tab->label.c_str());
			const ImVec2 label_size = ImGui::CalcTextSize(tab->label.c_str(), nullptr, true);
			ImVec2 pos = window->DC.CursorPos;

			ImVec2 itemSize(0, 50);

			if (style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
				pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
			ImVec2 size = ImGui::CalcItemSize(itemSize, label_size.x + 25 * 2.0f, label_size.y + 25 * 2.0f);

			const ImRect rect(pos, pos + size);
			ImGui::ItemSize(size, style.FramePadding.y);
			if (!ImGui::ItemAdd(rect, id))
			{
				ImGui::PopFont();
				return;
			}

			if (Vars::globalSelectedTabId == 0)
			{
				Vars::globalSelectedTabId = id;
			}

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, 0);

			if (pressed)
			{
				Vars::globalSelectedTabId = id;
			}

			bool selected = Vars::globalSelectedTabId == id;

			float* selectLerp = Vars::floatContainer.Get(id, 0, 0);
			*selectLerp = InterpHelper::Lerp(*selectLerp + g.IO.DeltaTime * 2 * (selected ? 1.0f : -1.0f));

			static ImRect subTabSelectionRect;

			if (selected)
			{
				Vars::currentSelectedTab = tab;

				constexpr float lineHeight = 5;
				subTabSelectionRect.Min.x = ImLerp(
					subTabSelectionRect.Min.x,
					rect.Min.x,
					*selectLerp
				);

				subTabSelectionRect.Max.x = ImLerp(
					subTabSelectionRect.Max.x,
					rect.Max.x,
					*selectLerp
				);

				subTabSelectionRect.Min.y = rect.Min.y;
				subTabSelectionRect.Max.y = rect.Max.y - (size.y - lineHeight);

				window->DrawList->AddRectFilled(
					subTabSelectionRect.Min,
					subTabSelectionRect.Max,
					Vars::gMenuColorScheme.primaryMed,
					0
				);
			}

			RenderTextDisabled(
				!selected,
				rect.Min + style.FramePadding - ImVec2(2.5, 0),
				rect.Max + style.FramePadding,
				tab->label.c_str(),
				nullptr,
				&label_size,
				style.ButtonTextAlign,
				&rect
			);
			ImGui::PopFont();
		}

		bool Section(UIComponents::Section* section)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
			{
				return false;
			}

			ImGuiContext& g = *GImGui;

			ImGuiID id = window->GetID(section->icon);
			ImGuiStyle& style = g.Style;

			ImVec2 pos = window->DC.CursorPos;
			ImVec2 size(window->Size.x, window->Size.x);
			ImRect rect(pos, pos + size);

			ImGui::ItemSize(size, style.FramePadding.y);
			if (!ImGui::ItemAdd(rect, id))
				return false;

			if (Vars::globalSelectedSectionId == 0)
			{
				Vars::globalSelectedSectionId = id;
			}

			ImDrawList* draw = window->DrawList;

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, 0);

			bool selected = Vars::globalSelectedSectionId == id;

			if (pressed)
			{
				Vars::globalSelectedSectionId = id;
				Vars::globalSelectedTabId = 0;
				Vars::currentSelectedSection = section;
			}

			if (selected)
			{
				Vars::currentSelectedSection = section;
			}

			float* selectLerp = Vars::floatContainer.Get(id, 0, 0);
			*selectLerp = InterpHelper::Lerp(*selectLerp + g.IO.DeltaTime * 5.0f * (selected ? 1.0f : -1.0f));

			ImU32 rectColor = ImLerp(
				Vars::gMenuColorScheme.bgLow,
				Vars::gMenuColorScheme.bgMed,
				*selectLerp
			);

			ImU32 lineColor = ImLerp(
				Vars::gMenuColorScheme.bgMed,
				Vars::gMenuColorScheme.primaryMed,
				*selectLerp
			);

			draw->AddRectFilled(
				rect.Min,
				rect.Max,
				rectColor,
				0
			);

			draw->AddRectFilled(
				ImLerp(
					rect.Min + ImVec2(size.x - 2, 0),
					rect.Min + ImVec2(size.x - 3, 0),
					*selectLerp
				),
				rect.Max,
				lineColor,
				0
			);

			ImGui::PushFont(Vars::gIconFont);
			RenderTextDisabled(
				!selected,
				rect.Min,
				rect.Max,
				section->icon,
				nullptr,
				nullptr,
				style.ButtonTextAlign,
				&rect
			);
			ImGui::PopFont();

			return selected;
		}

		bool Checkbox(const char* label, bool* val)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGui::PushFont(Vars::gMediumFont);

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

			const float square_sz = ImGui::GetFrameHeight();
			const ImVec2 pos = window->DC.CursorPos;
			const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
			ImGui::ItemSize(total_bb, style.FramePadding.y);

			if (!ImGui::ItemAdd(total_bb, id))
			{
				ImGui::PopFont();
				return false;
			}

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
			if (pressed)
			{
				*val = !(*val);
				ImGui::MarkItemEdited(id);
			}
			const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));

			ImColor checkmarkTargetColor = Vars::gMenuColorScheme.bgLow;
			ImColor borderTargetColor = Vars::gMenuColorScheme.bgLow;

			if (!(*val) && hovered)
			{
				checkmarkTargetColor = Vars::gMenuColorScheme.primaryMed;
			}
			else if (*val && hovered)
			{
				checkmarkTargetColor = Vars::gMenuColorScheme.primaryHigh;
				borderTargetColor = Vars::gMenuColorScheme.primaryHigh;
			}
			else if (*val)
			{
				checkmarkTargetColor = Vars::gMenuColorScheme.primaryMed;
				borderTargetColor = Vars::gMenuColorScheme.primaryMed;
			}

			ImColor* checkmarkColor = Vars::colorContainer.Get(id, 0, checkmarkTargetColor);
			ImColor* borderColor = Vars::colorContainer.Get(id, 1, borderTargetColor);
			*checkmarkColor = ImLerp(
				*checkmarkColor,
				checkmarkTargetColor,
				g.IO.DeltaTime * 10
			);

			*borderColor = ImLerp(
				*borderColor,
				borderTargetColor,
				g.IO.DeltaTime * 10
			);

			window->DrawList->AddRectFilled(
				check_bb.Min,
				check_bb.Max,
				ImGui::GetColorU32(ImGuiCol_WindowBg),
				style.FrameRounding
			);

			window->DrawList->AddRect(
				check_bb.Min,
				check_bb.Max,
				*borderColor,
				style.FrameRounding,
				0,
				1.5f
			);

			const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 6.0f));
			ImGui::RenderCheckMark(
				window->DrawList,
				check_bb.Min + ImVec2(pad, pad),
				*checkmarkColor,
				square_sz - pad * 2.0f
			);

			ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
			if (label_size.x > 0.0f)
				ImGui::RenderText(label_pos, label);

			ImGui::PopFont();
			return true;
		}

		bool Button(const char* label, UIComponents::ButtonSizeType sizeType, ImGuiButtonFlags flags = 0)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGui::PushFont(Vars::gMediumFont);
			ImGui::PushStyleColor(ImGuiCol_Text, Vars::gMenuColorScheme.textHigh.Value);

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

			ImVec2 pos = window->DC.CursorPos;
			if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
				pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
			ImVec2 size_arg = sizeType == UIComponents::ButtonSizeType::FLEXIBLE ? ImVec2() : ImVec2(-1, 0);
			ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 4.0f, label_size.y + style.FramePadding.y * 4.0f);

			const ImRect bb(pos, pos + size);
			auto window_dc_backup = window->DC;

			ImGui::ItemSize(size, style.FramePadding.y);
			if (sizeType == UIComponents::ButtonSizeType::FLEXIBLE &&
				bb.Max.x + size.x + style.ItemSpacing.x * 2.0f < window->DC.CursorStartPos.x + window->Size.x)
			{
				ImGui::SameLine();
			}

			//if (strcmp(label, "Unlock RGB Pioneer set") == 0)
			//{
			//	//if(window->DC.CursorPosPrevLine.x + size.x + style.ItemSpacing.x > window->DC.CursorStartPos.x + window->Size.x)
			//	IM_DEBUG_BREAK();
			//}

			if (!ImGui::ItemAdd(bb, id))
			{
				ImGui::PopFont();
				ImGui::PopStyleColor();
				return false;
			}

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

			// Render
			ImColor targetColor = Vars::gMenuColorScheme.primaryMed;
			if (held)
			{
				targetColor = Vars::gMenuColorScheme.primaryHigh;
			}
			else if (hovered)
			{
				targetColor = Vars::gMenuColorScheme.primaryLow;
			}

			ImColor* lerpColor = Vars::colorContainer.Get(id, 0, targetColor);
			*lerpColor = ImLerp(
				*lerpColor,
				targetColor,
				g.IO.DeltaTime * 10
			);

			ImGui::RenderFrame(bb.Min, bb.Max, *lerpColor, true, style.FrameRounding);
			ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

			ImGui::PopFont();
			ImGui::PopStyleColor();

			return pressed;
		}

		bool ToggableButton(const char* label, bool disabled, const ImVec2& size_arg = {}, ImGuiButtonFlags flags = 0)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGui::PushFont(Vars::gMediumFont);
			ImGui::PushStyleColor(ImGuiCol_Text, Vars::gMenuColorScheme.textHigh.Value);

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

			ImVec2 pos = window->DC.CursorPos;
			if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
				pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
			ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

			const ImRect bb(pos, pos + size);
			ImGui::ItemSize(size, style.FramePadding.y);
			if (!ImGui::ItemAdd(bb, id))
			{
				ImGui::PopFont();
				ImGui::PopStyleColor();
				return false;
			}

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

			// Render
			ImColor targetColor = Vars::gMenuColorScheme.primaryMed;

			if (disabled)
			{
				targetColor = Vars::gMenuColorScheme.primaryLow;
			}

			ImColor* lerpColor = Vars::colorContainer.Get(id, 0, targetColor);
			*lerpColor = ImLerp(
				*lerpColor,
				targetColor,
				g.IO.DeltaTime * 10
			);

			ImGui::RenderFrame(bb.Min, bb.Max, *lerpColor, true, style.FrameRounding);
			ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

			ImGui::PopFont();
			ImGui::PopStyleColor();

			return pressed && !disabled;
		}

		bool SliderScalarBar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;
			const ImGuiID id = window->GetID(label);
			const float w = window->Size.x;

			char value_buf[64];
			const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, 5));
			ImGui::PushFont(Vars::gMediumFont);
			ImGui::TextUnformatted(label);
			ImGui::PopFont();

			const ImVec2 label_size = ImGui::CalcTextSize(value_buf, value_buf_end, true);

			window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + (w - style.WindowPadding.x - label_size.x);
			window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y + g.FontSize * 0.2f;
			window->DC.CurrLineSize = window->DC.PrevLineSize;
			window->DC.CurrLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
			window->DC.IsSameLine = true;

			ImGui::PushFont(Vars::gRegularFont);
			ImGui::TextUnformatted(value_buf, value_buf_end);
			ImGui::PopFont();

			const ImVec2 size(w - style.WindowPadding.x * 2.0f, 25);

			const ImRect frame_bb(
				window->DC.CursorPos,
				window->DC.CursorPos + size
			);

			const ImRect total_bb(
				frame_bb.Min,
				frame_bb.Max + ImVec2(style.ItemInnerSpacing.x + label_size.x, 0.0f)
			);

			const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
			ImGui::ItemSize(total_bb, style.FramePadding.y);

			if (!ImGui::ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
			{
				ImGui::PopStyleVar();
				return false;
			}

			ImGui::PopStyleVar();

			if (format == NULL)
				format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

			//const bool hovered = ImGui::ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
			bool hovered, held = false;
			bool pressed = ImGui::ButtonBehavior(frame_bb, id, &hovered, &held, 0);
			//const bool clicked = hovered && ImGui::IsMouseClicked(0, id);

			if (pressed)
			{
				ImGui::SetKeyOwner(ImGuiKey_MouseLeft, id);

				ImGui::SetActiveID(id, window);
				ImGui::SetFocusID(id, window);
				ImGui::FocusWindow(window);
			}

			ImRect grab_bb;
			ImRect bgRect(
				frame_bb.Min - ImVec2(style.ItemSpacing.x / 2, 0),
				frame_bb.Max - ImVec2()
			);

			const bool value_changed = ImGui::SliderBehavior(
				bgRect,
				id,
				data_type,
				p_data,
				p_min,
				p_max,
				format,
				flags,
				&grab_bb
			);

			if (value_changed)
			{
				ImGui::MarkItemEdited(id);
			}

			ImColor targetFillerColor = Vars::gMenuColorScheme.primaryMed;
			ImColor targetGrabColor = Vars::gMenuColorScheme.primaryHigh;

			if (held)
			{
				targetFillerColor = Vars::gMenuColorScheme.primaryLow;
				targetGrabColor = Vars::gMenuColorScheme.primaryMed;
			}
			else if (hovered)
			{
				targetFillerColor = Vars::gMenuColorScheme.primaryHigh;
			}


			ImColor* holdFillerColor = Vars::colorContainer.Get(id, 0, Vars::gMenuColorScheme.primaryMed);
			ImColor* holdGrabColor = Vars::colorContainer.Get(id, 1, Vars::gMenuColorScheme.primaryHigh);
			float* sliderInterp = Vars::floatContainer.Get(id, 0, (frame_bb.Max.x - grab_bb.Max.x) / (frame_bb.Max.x - frame_bb.Min.x));

			*holdFillerColor = ImLerp(
				*holdFillerColor,
				targetFillerColor,
				g.IO.DeltaTime * 10
			);

			*holdGrabColor = ImLerp(
				*holdGrabColor,
				targetGrabColor,
				g.IO.DeltaTime * 10
			);

			*sliderInterp = ImLerp(
				*sliderInterp,
				(frame_bb.Max.x - grab_bb.Max.x) / (frame_bb.Max.x - frame_bb.Min.x),
				0.25f
			);

			constexpr float sliderFillerThinness = 8.0f;
			window->DrawList->AddRectFilled(
				ImVec2(
					frame_bb.Min.x,
					frame_bb.Min.y + sliderFillerThinness
				),
				ImVec2(
					frame_bb.Max.x,
					frame_bb.Max.y - sliderFillerThinness
				),
				ImGui::GetColorU32(ImGuiCol_WindowBg),
				g.Style.FrameRounding
			);

			window->DrawList->AddRectFilled(
				ImVec2(
					frame_bb.Min.x,
					frame_bb.Min.y + sliderFillerThinness
				),
				ImVec2(
					frame_bb.Max.x - size.x * (*sliderInterp),
					frame_bb.Max.y - sliderFillerThinness
				),
				*holdFillerColor,
				g.Style.FrameRounding
			);

			float sliderGrabThinnessMul = 0.5f;
			float sliderGrabSize = (frame_bb.Max.y - sliderFillerThinness * sliderGrabThinnessMul) - (frame_bb.Min.y + sliderFillerThinness * sliderGrabThinnessMul);
			window->DrawList->AddRectFilled(
				ImVec2(
					(frame_bb.Max.x - sliderGrabSize - size.x * (*sliderInterp)) + sliderGrabSize * 0.5f - style.WindowPadding.x * 0.5f,
					frame_bb.Min.y + sliderFillerThinness * sliderGrabThinnessMul
				),
				ImVec2(
					(frame_bb.Max.x - size.x * (*sliderInterp)) + sliderGrabSize * 0.5f - style.WindowPadding.x * 0.5f,
					frame_bb.Max.y - sliderFillerThinness * sliderGrabThinnessMul
				),
				*holdGrabColor,
				360
			);


			return value_changed;
		}

		bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0)
		{
			return SliderScalarBar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
		}

		bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
		{
			return SliderScalarBar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
		}

		bool InputString(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg = { -1, 0 })
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiIO& io = ImGui::GetIO();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, 5));
			ImGui::PushFont(Vars::gMediumFont);
			ImGui::Text(label);
			ImGui::PopFont();

			ImGui::PushStyleColor(ImGuiCol_FrameBg, Vars::gMenuColorScheme.bgLow.Value);

			// ImGui::InputTextEx function was modified to disable label render cause you cant
			// just paste the body here due to hardcoded used function accessibility.
			bool value_changed = ImGui::InputTextEx(label, hint, buf, buf_size, size_arg, ImGuiInputTextFlags_NoLabel);
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();

			return value_changed;
		}

		bool InputScalar(const char* label, ImGuiDataType data_type, void* p_data, const char* hint = NULL, const char* format = NULL,
			const ImVec2& size_arg = { -1, 0 })
		{
			ImGuiContext& g = *GImGui;
			ImGuiStyle& style = ImGui::GetStyle();
			ImGuiWindow* window = ImGui::GetCurrentWindow();

			if (window->SkipItems)
				return false;

			ImGuiIO& io = ImGui::GetIO();

			if (format == NULL)
				format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

			char buf[64];
			ImGui::DataTypeFormatString(buf, IM_ARRAYSIZE(buf), data_type, p_data, format);

			bool value_changed = false;
			if (InputString(label, hint, buf, IM_ARRAYSIZE(buf)))
			{
				value_changed = ImGui::DataTypeApplyFromText(buf, data_type, p_data, format);
			}

			if (value_changed)
				ImGui::MarkItemEdited(g.LastItemData.ID);

			return value_changed;
		}

		bool InputInt(const char* label, const char* hint, int* v)
		{
			return InputScalar(label, ImGuiDataType_S32, v, hint, "%d");
		}

		bool BrowserElement(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			const ImGuiStyle& style = g.Style;

			// Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
			ImGuiID id = window->GetID(label);
			ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
			ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
			ImVec2 pos = window->DC.CursorPos;
			pos.y += window->DC.CurrLineTextBaseOffset;
			ImGui::ItemSize(size, 0.0f);

			// Fill horizontal space
			// We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
			const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
			const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
			const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
			if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
				size.x = ImMax(label_size.x, max_x - min_x);

			// Text stays at the submission position, but bounding box may be extended on both sides
			const ImVec2 text_min = pos + ImVec2(style.FramePadding.x + 3, 0);
			const ImVec2 text_max(min_x + size.x, pos.y + size.y);

			// Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
			ImRect bb(min_x, pos.y, text_max.x, text_max.y);
			if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
			{
				const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
				const float spacing_y = style.ItemSpacing.y;
				const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
				const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
				bb.Min.x -= spacing_L;
				bb.Min.y -= spacing_U;
				bb.Max.x += (spacing_x - spacing_L);
				bb.Max.y += (spacing_y - spacing_U);
			}
			//if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

			// Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackgroundChannel for every Selectable..
			const float backup_clip_rect_min_x = window->ClipRect.Min.x;
			const float backup_clip_rect_max_x = window->ClipRect.Max.x;
			if (span_all_columns)
			{
				window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
				window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
			}

			const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
			const bool item_add = ImGui::ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
			if (span_all_columns)
			{
				window->ClipRect.Min.x = backup_clip_rect_min_x;
				window->ClipRect.Max.x = backup_clip_rect_max_x;
			}

			if (!item_add)
				return false;

			const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
			if (disabled_item && !disabled_global) // Only testing this as an optimization
				ImGui::BeginDisabled();

			// FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
			// which would be advantageous since most selectable are not selected.
			if (span_all_columns)
			{
				if (g.CurrentTable)
					ImGui::TablePushBackgroundChannel();
				else if (window->DC.CurrentColumns)
					ImGui::PushColumnsBackground();
				g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
				g.LastItemData.ClipRect = window->ClipRect;
			}

			// We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
			ImGuiButtonFlags button_flags = 0;
			if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
			if (flags & ImGuiSelectableFlags_NoSetKeyOwner) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
			if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
			if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
			if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
			//if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)) { button_flags |= ImGuiButtonFlags_AllowOverlap; }

			const bool was_selected = selected;
			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);

			// Auto-select when moved into
			// - This will be more fully fleshed in the range-select branch
			// - This is not exposed as it won't nicely work with some user side handling of shift/control
			// - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
			//   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
			//   - (2) usage will fail with clipped items
			//   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
			if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
				if (g.NavJustMovedToId == id)
					selected = pressed = true;

			// Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard

			if (pressed)
				ImGui::MarkItemEdited(id);

			// In this branch, Selectable() cannot toggle the selection so this will never trigger.
			if (selected != was_selected) //-V547
				g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

			// Render
			if (hovered)
			{
				const ImU32 col = ImGui::GetColorU32(ImGuiCol_ChildBg);
				ImGui::RenderFrame(bb.Min, bb.Max, col, false, g.Style.FrameRounding);
			}


			ImU32 col = Vars::gMenuColorScheme.primaryLow;

			if (selected)
			{
				col = Vars::gMenuColorScheme.primaryHigh;
			}
			if (hovered)
			{
				col = Vars::gMenuColorScheme.primaryMed;
			}

			ImGui::RenderFrame(
				bb.Min,
				ImVec2(bb.Min.x + 7, bb.Max.y),
				col,
				false,
				0
			);

			if (g.NavId == id)
				ImGui::RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_Compact | ImGuiNavHighlightFlags_NoRounding);

			if (span_all_columns)
			{
				if (g.CurrentTable)
					ImGui::TablePopBackgroundChannel();
				else if (window->DC.CurrentColumns)
					ImGui::PopColumnsBackground();
			}

			ImGui::RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);

			// Automatically close popups
			if (pressed)
				ImGui::CloseCurrentPopup();

			if (disabled_item && !disabled_global)
				ImGui::EndDisabled();

			IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
			return pressed; //-V1020
		}

		bool Browser(const char* label, int* index, const std::vector<std::string>* list)
		{
			ImGuiContext& g = *GImGui;
			ImGuiStyle& style = ImGui::GetStyle();
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			ImGuiID id = ImGui::GetID(label);

			if (window->SkipItems)
				return false;

			const std::string& previewValue = list->empty() ? "empty list" : list->at(*index);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, 5));
			ImGui::PushFont(Vars::gMediumFont);
			ImGui::TextUnformatted(label);
			ImGui::PopFont();

			float squareRatio = g.FontSize + g.Style.FramePadding.y * 4.0f;
			ImVec2 pos = window->DC.CursorPos;
			ImVec2 size(
				window->Size.x - style.WindowPadding.x * 2.0,
				squareRatio
			);
			ImRect rect(pos, pos + size);

			ImGui::ItemSize(size, style.FramePadding.y);
			if (!ImGui::ItemAdd(rect, id))
			{
				ImGui::PopStyleVar();
				return false;
			}

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held);
			ImGuiID popup_id = ImHashStr(label);
			bool popup_open = ImGui::IsPopupOpen(popup_id, ImGuiPopupFlags_None);
			if (pressed && !popup_open)
			{
				//ImGui::OpenPopupEx(popup_id, ImGuiPopupFlags_None);
				ImGui::OpenPopupEx(popup_id, ImGuiPopupFlags_None);
				popup_open = true;
			}

			ImColor targetButtonColor = Vars::gMenuColorScheme.primaryMed;
			if (popup_open)
			{
				targetButtonColor = Vars::gMenuColorScheme.primaryHigh;
			}
			else if (hovered)
			{
				targetButtonColor = Vars::gMenuColorScheme.primaryLow;
			}

			ImColor* buttonColor = Vars::colorContainer.Get(id, 0);
			*buttonColor = ImLerp(
				*buttonColor,
				targetButtonColor,
				g.IO.DeltaTime * 10
			);

			window->DrawList->AddRectFilled(
				rect.Min,
				rect.Max,
				Vars::gMenuColorScheme.bgLow,
				style.FrameRounding
			);

			ImGui::RenderTextClipped(
				rect.Min + style.FramePadding,
				rect.Max,
				previewValue.c_str(),
				NULL,
				NULL
			);

			window->DrawList->AddRectFilled(
				ImVec2(rect.Max.x - squareRatio, rect.Min.y),
				rect.Max,
				*buttonColor,
				style.FrameRounding
			);

			ImGui::PushStyleColor(ImGuiCol_Text, ImColor(255, 255, 255).Value);
			ImGui::SetWindowFontScale(0.75f);
			ImGui::PushFont(Vars::gIconFont);
			ImGui::RenderTextClipped(
				ImVec2(rect.Max.x - squareRatio + style.FramePadding.y * 2.0, rect.Min.y - style.FramePadding.y),
				ImVec2(rect.Max.x - style.FramePadding.y * 2.0, rect.Max.y - style.FramePadding.y * 2.0),
				ICON_FA_LIST,
				nullptr,
				nullptr,
				ImVec2(0, style.FramePadding.y / 2)
			);
			ImGui::PopFont();
			ImGui::SetWindowFontScale(1.0f);
			ImGui::PopStyleColor();

			if (!popup_open)
			{
				ImGui::PopStyleVar();
				window->DC.CursorPos.y += style.ItemSpacing.y / 2.0f;
				return false;
			}

			std::string searchFrameId = std::string(label).append("-searchFrame");
			std::string listId = std::string(label).append("-listId");

			const auto popupFlags = ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
				ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoResize | ImGuiChildFlags_AutoResizeY;

			char name[16];
			ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginComboDepth); // Recycle windows based on depth

			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, style.WindowRounding);
			if (ImGui::Begin(name, NULL, popupFlags))
			{
				static char searchBuffer[255];
				static int totalResultFound = 0;

				if (pressed)
				{
					strcpy(searchBuffer, "");
				}

				ImGui::SetWindowSize(ImVec2(300, 500));

				ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
				ImGui::BeginChild(searchFrameId.c_str(), { -1.0, 0.0 }, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding);
				{
					ImGui::PopStyleVar();

					ImGui::PushFont(Vars::gLargeFont);
					ImGui::TextUnformatted(label);
					ImGui::PopFont();
					ImGui::Separator();
					InputString("Search", nullptr, searchBuffer, 255);
					ImGui::Text("Result found: %i", totalResultFound);
					ImGui::EndChild();

				}
				ImGui::PushFont(Vars::gMediumFont);
				Frame(listId.c_str(), { -1, -1 });
				{
					bool valueChanged = false;
					if (strlen(searchBuffer) == 0)
					{
						int iterateCount = list->size();// > maxResult ? maxResult : list->size();
						totalResultFound = iterateCount;

						for (int i = 0; i < iterateCount; i++)
						{
							const std::string& val = list->at(i);
							ImGui::PushID(i);
							const bool itemSelected = (i == *index);
							if (BrowserElement(val.c_str(), itemSelected, 0, ImVec2()))
							{
								*index = i;
								valueChanged = true;
							}

							if (itemSelected)
							{
								ImGui::SetItemDefaultFocus();
							}

							ImGui::PopID();
						}
					}
					else
					{
						std::vector<std::pair<int, const std::string&>> queryList = SearchSubstring(list, searchBuffer, 0);
						totalResultFound = queryList.size();

						for (int i = 0; i < queryList.size(); i++)
						{
							std::pair<int, const std::string&> it = queryList.at(i);
							const std::string& val = it.second;

							ImGui::PushID(i);
							const bool itemSelected = (it.first == *index);
							if (BrowserElement(val.c_str(), itemSelected, 0, ImVec2()))
							{
								*index = it.first;
								valueChanged = true;
							}

							if (itemSelected)
							{
								ImGui::SetItemDefaultFocus();
							}

							ImGui::PopID();
						}
					}

					if (valueChanged)
					{
						ImGui::CloseCurrentPopup();
					}

					if (ImGui::IsKeyPressed(ImGuiKey_Escape))
					{
						ImGui::CloseCurrentPopup();
					}

					EndFrame();
				}
				ImGui::PopFont();
				ImGui::EndPopup();
			}

			ImGui::PopStyleVar(2);
			window->DC.CursorPos.y += style.ItemSpacing.y / 2.0f;
			return true;
		}

		bool Mode(const char* label, int* index, const std::vector<std::string>* modes)
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			ImGuiIO& io = ImGui::GetIO();

			if (window->SkipItems)
				return false;

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(style.ItemSpacing.x, 5));
			ImGui::PushFont(Vars::gMediumFont);
			ImGui::Text(label);
			ImGui::PopFont();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, style.FramePadding.y));
			for (int i = 0; i < modes->size(); i++)
			{
				const std::string& modeLabel = modes->at(i);

				bool disable = i == *index;
				if (ToggableButton(modeLabel.c_str(), disable))
				{
					*index = i;
				}

				if (i < modes->size() - 1)
				{
					ImGui::SameLine();
				}
			}

			ImGui::PopStyleVar(2);

			return true;
		}
	}

	namespace TagService
	{
		std::unordered_map<std::string, std::vector<UIComponents::IComponent*>*> taggedComponents;

		std::optional<std::vector<UIComponents::IComponent*>*> GetTaggedComponents(const char* tag)
		{
			auto it = taggedComponents.find(std::string(tag));

			if (it == taggedComponents.end())
			{
				return std::nullopt;
			}

			return it->second;
		}

		void TagComponent(const char* tag, UIComponents::IComponent* component)
		{
			auto key = std::string(tag);
			auto it = taggedComponents.find(key);

			if (it == taggedComponents.end())
			{
				auto alllocatedList = new std::vector<UIComponents::IComponent*>;
				alllocatedList->push_back(component);
				taggedComponents.insert({ key, alllocatedList });
			}
			else
			{
				it->second->push_back(component);
			}
		}

		void IterateTaggedComponents(const char* tag, std::function<void(UIComponents::IComponent*)> func)
		{
			auto comps = TagService::GetTaggedComponents(tag);
			if (comps.has_value())
			{
				for (auto& v : *comps.value())
				{
					func(v);
				}
			}
		}

		void ToggleTagVisibility(const char* tag, bool toggle)
		{
			IterateTaggedComponents(tag, [&](UIComponents::IComponent* comp)
			{
				comp->visible = toggle;
			});
		}
	}

	void RenderSubTabBar()
	{
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGui::GetWindowDrawList()->AddRectFilled(
			windowPos + ImVec2(0, windowSize.y - 2.0f),
			windowPos + windowSize,
			ImGui::GetColorU32(ImGuiCol_ChildBg),
			0
		);

		if (Vars::currentSelectedSection != nullptr)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2());
			Vars::currentSelectedSection->RenderChildren();
			ImGui::PopStyleVar();
		}
	}

	namespace ConfigManager
	{
		std::vector<UIComponents::IConfigurable*> configurableCompsList;
		using nlohmann::json;

		bool LoadConfig(const std::wstring& path)
		{
			std::ifstream fileStream(path);
			assert(fileStream.is_open());

			if (!fileStream.is_open())
			{
				return false;
			}

			try
			{
				auto fileSize = std::filesystem::file_size(path);
				std::string fileData;
				fileData.resize(fileSize);
				fileStream.read(fileData.data(), fileSize);
				json configData = json::parse(fileData);

				for (UIComponents::IConfigurable* component : configurableCompsList)
				{
					component->HandleConfigLoad(configData[component->label]);

				}
			}
			catch (std::exception& err)
			{
				return false;
			}

			return true;
		}

		bool SaveConfig(const std::wstring& path)
		{
			bool hasExtension = path.find(L".nazicfg") != std::wstring::npos;
			std::ofstream fileStream(!hasExtension ? path + L".nazicfg" : path);
			try
			{
				json configData = json::object({});

				for (UIComponents::IConfigurable* component : configurableCompsList)
				{
					auto config = component->HandleConfigSave();
					configData[component->label] = config;
				}

				fileStream << configData.dump();
				fileStream.flush();
			}

			catch (std::exception& err)
			{
				return false;
			}

			return true;
		}
	}

	void RenderGroups(UIComponents::Tab* tab)
	{
		if (tab == nullptr) return;

		std::string leftBodyId = tab->label + "-body-left";
		std::string rightBodyId = tab->label + "-body-right";

		if (tab->splitType == UIComponents::GroupSplitType::NO_SPLIT)
		{
			tab->RenderChildren();
		}
		else if (tab->splitType == UIComponents::GroupSplitType::HALF)
		{
			using UIComponents::GroupPlacementType;

			ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
			ImVec2 containerSize = ImGui::GetWindowSize();

			ImVec2 leftBodySize((containerSize.x / 2) - itemSpacing.x * 2, 0.0);
			ImVec2 rightBodySize(-1.0, 0.0);

			std::vector<UIComponents::Group*> leftGroupsList;
			std::vector<UIComponents::Group*> rightGroupsList;

			UIComponents::GroupPlacementType autoPlacementType = GroupPlacementType::LEFT;

			for (size_t i = 0; i < tab->children.size(); i++)
			{
				UIComponents::Group* group = (UIComponents::Group*)tab->children.at(i);

				switch (group->placementType)
				{
					case GroupPlacementType::LEFT:
					{
						leftGroupsList.push_back(group);
						autoPlacementType = GroupPlacementType::RIGHT;
						break;
					}
					case GroupPlacementType::RIGHT:
					{
						rightGroupsList.push_back(group);
						autoPlacementType = GroupPlacementType::LEFT;
						break;
					}
					case GroupPlacementType::AUTO:
					{
						if (autoPlacementType == GroupPlacementType::LEFT)
						{
							leftGroupsList.push_back(group);
							autoPlacementType = GroupPlacementType::RIGHT;
						}
						else if (autoPlacementType == GroupPlacementType::RIGHT)
						{
							rightGroupsList.push_back(group);
							autoPlacementType = GroupPlacementType::LEFT;
						}
						break;
					}
				}
			}

			Widgets::Frame(leftBodyId.c_str(), leftBodySize, ImGuiChildFlags_AutoResizeY);
			{
				for (UIComponents::Group* queue : leftGroupsList)
				{
					Widgets::Group(queue);
				}

				ImGui::EndChild();
			}
			ImGui::SameLine();

			Widgets::Frame(rightBodyId.c_str(), rightBodySize, ImGuiChildFlags_AutoResizeY);
			{
				for (UIComponents::Group* queue : rightGroupsList)
				{
					Widgets::Group(queue);
				}

				Widgets::EndFrame();
			}
		}
	}

	void RenderFeatureBody(const std::string& strId)
	{
		std::string sectionId = strId + "-section";
		std::string tabRowId = strId + "-tabRow";
		std::string groupContainerId = strId + "-container";

		Widgets::Frame(sectionId.c_str(), { -1, -1 }, 0, ImGuiWindowFlags_NoScrollbar);
		{
			Widgets::Frame(tabRowId.c_str(), { -1, 50 }, 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollbar);
			{
				RenderSubTabBar();
				Widgets::EndFrame();
			}

			Widgets::Frame(groupContainerId.c_str(), { -1, 0.0 });
			{
				RenderGroups(Vars::currentSelectedTab);
				Widgets::EndFrame();
			}
			Widgets::EndFrame();
		}
	}

	void MainWindow(const ImVec2& size, UIComponents::MainWindow* mainWindow)
	{
		auto WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
		if (ImGui::Begin("#MainWindow", nullptr, WindowFlags))
		{
			ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
			ImGui::SetWindowSize(size);

			Widgets::Frame("#Selection", { 50, -1 }, 0);
			{
				ImVec2 parentSize = ImGui::GetWindowSize();

				ImVec2 logoSectionSize(parentSize.x, parentSize.x);
				ImVec2 tabColumnSize(logoSectionSize.x, parentSize.y - logoSectionSize.x - itemSpacing.y);

				Widgets::Frame("#Logo", logoSectionSize);
				{
					//ImGui::Image(Global::gMenuLogo, logoSectionSize);
					//ImGui::SetWindowFontScale(2.0f);
					ImGui::SetCursorPosY(ImGui::GetStyle().FramePadding.y * 2.0f);
					ImGui::PushFont(Vars::gExtraLargeFont);
					ImGui::TextUnformatted("Nzi");
					ImGui::PopFont();
					//ImGui::SetWindowFontScale(1.0f);
					Widgets::EndFrame();
				}

				Widgets::Frame("#TabColumn", tabColumnSize);
				{
					ImVec2 columnPos = ImGui::GetWindowPos();
					ImVec2 columnSize = ImGui::GetWindowSize();

					ImGui::GetWindowDrawList()->AddRectFilled(
						columnPos + ImVec2(columnSize.x - 2, 0),
						columnPos + ImVec2(columnSize.x, columnSize.y),
						ImGui::GetColorU32(ImGuiCol_ChildBg),
						0
					);

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);

					mainWindow->RenderChildren();

					ImGui::PopStyleVar(2);
					Widgets::EndFrame();
				}

				Widgets::EndFrame();
			}

			ImGui::SameLine();
			RenderFeatureBody("#feature");
			ImGui::End();
		}
	}

	void DisclaimerWindow(const ImVec2& size, const std::function<void()>& onClose)
	{
		auto WindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
		if (ImGui::Begin("#MainWindow", nullptr, WindowFlags))
		{
			ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
			ImGui::SetWindowSize(size);
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);

			ImGui::BeginChild("credit-group", {-1.0, -1.0}, ImGuiChildFlags_AlwaysUseWindowPadding);
			{
				ImGui::PopStyleVar();
				ImGui::PushFont(Vars::gLargeFont);
				ImGui::TextUnformatted("Disclaimer");
				ImGui::PopFont();
				ImGui::Separator();

				ImGui::TextUnformatted(
					"Please, read carefuly!\n\n"

					"Our cheat (Nazi Mod) is provided to you completely free of charge.\n"
					"We do not engage in any transactions involving the sale of our cheat.\n"
					"If you encounter any individual or entity claiming to sell our cheat, please be aware that it is unequivocally a scam.\n"
					"We do not endorse or participate in any form of commercialization of our cheat software.\n\n"

					"Additionally, we emphasize that our cheat is exclusively distributed through our official Discord server.\n"
					"Any download links or sources claiming to offer our cheat outside of our Discord server could potentially contain malware or other\n"
					"malicious software. We do not recommend obtaining our cheat from any other source apart from our official Discord server.\n\n"

					"tdlr: Nazi Mod is free. If you bought this cheat from someone, you're cooked.\n"
					"If you find Nazi Mod from any source other than our Discord server, it might be a malware or virus.\n\n"

					"Thank you for your understanding!"
				);

				if (Widgets::Button("Nazi Mod Discord server (.gg/GCphUP98Xh)", UIComponents::ButtonSizeType::FLEXIBLE)
				{
					ShellExecuteA(0, 0, "https://discord.gg/GCphUP98Xh", 0, 0, SW_SHOW);
				}
				if (Widgets::Button("I understand.", UIComponents::ButtonSizeType::FLEXIBLE))
				{
					onClose();
				}
				ImGui::EndChild();
			}
		}
		ImGui::End();
	}

	void SaveColorScheme(const std::string& path)
	{
		char colorSchemeBuffer[sizeof(MenuColorScheme)];
		memcpy(colorSchemeBuffer, &Vars::gMenuColorScheme, sizeof(MenuColorScheme));

		std::ofstream wStream(path, std::ios::binary);
		if (!wStream.is_open())
		{
			throw std::runtime_error("No such file.");
		}

		wStream.write(colorSchemeBuffer, sizeof(MenuColorScheme));
		wStream.close();
	}

	MenuColorScheme ReadColorScheme(const std::string& path)
	{
		char colorSchemeBuffer[sizeof(MenuColorScheme)];
		memcpy(colorSchemeBuffer, &Vars::gMenuColorScheme, sizeof(MenuColorScheme));

		std::ifstream rStream(path, std::ios::binary);
		if (!rStream.is_open())
		{
			throw std::runtime_error("No such file.");
		}

		rStream.read(colorSchemeBuffer, sizeof(MenuColorScheme));
		rStream.close();

		return *reinterpret_cast<MenuColorScheme*>(colorSchemeBuffer);
	}

	void ManageDefaultTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_TableBorderLight] = ImColor();
		style.Colors[ImGuiCol_TableBorderStrong] = ImColor();
		style.Colors[ImGuiCol_WindowBg] = Vars::gMenuColorScheme.bgLow;
		style.Colors[ImGuiCol_ChildBg] = Vars::gMenuColorScheme.bgMed;
		style.Colors[ImGuiCol_PopupBg] = Vars::gMenuColorScheme.bgLow;
		style.Colors[ImGuiCol_TitleBgCollapsed] = Vars::gMenuColorScheme.bgLow;
		style.Colors[ImGuiCol_TitleBgActive] = Vars::gMenuColorScheme.bgLow;
		style.Colors[ImGuiCol_Header] = Vars::gMenuColorScheme.bgLow;
		style.Colors[ImGuiCol_HeaderActive] = Vars::gMenuColorScheme.bgLow;
		style.Colors[ImGuiCol_HeaderHovered] = Vars::gMenuColorScheme.bgLow;
		style.Colors[ImGuiCol_ScrollbarBg] = ImColor();
		style.Colors[ImGuiCol_ScrollbarGrab] = Vars::gMenuColorScheme.bgHigh;
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = Vars::gMenuColorScheme.primaryMed;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = Vars::gMenuColorScheme.primaryMed;
		style.Colors[ImGuiCol_Button] = Vars::gMenuColorScheme.primaryMed;
		style.Colors[ImGuiCol_ButtonHovered] = Vars::gMenuColorScheme.primaryHigh;
		style.Colors[ImGuiCol_ButtonActive] = Vars::gMenuColorScheme.primaryLow;
		style.Colors[ImGuiCol_Text] = Vars::gMenuColorScheme.textMed;
	}

	ImFont* LoadFont(ImGuiIO& io, const std::string& filepath, float fontSize, const ImFontConfig* fontCfg = NULL, const ImWchar* glyphRanges = NULL)
	{
		return io.Fonts->AddFontFromFileTTF(filepath.c_str(), fontSize, fontCfg, glyphRanges);
	}

	void QueueNotification(NotificationType notifType, const std::string& message)
	{
		switch (notifType)
		{
			//case INFO:
			//{
			//	ImGui::InsertNotification({ ImGuiToastType::Info, 3000, message.c_str()});
			//	break;
			//}
			//case WARN:
			//{
			//	ImGui::InsertNotification({ ImGuiToastType::Warning, 3000, message.c_str() });
			//	break;
			//}
			//case ERROR:
			//{
			//	ImGui::InsertNotification({ ImGuiToastType::Error, 3000, message.c_str() });
			//	break;
			//}
			//default:
			//{
			//	break;
			//}
		}
	}

	void OnLoad()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 10;
		style.ChildRounding = 10;
		style.FrameRounding = 8;
		style.TabRounding = 10;

		style.WindowPadding = ImVec2(10.0, 10.0);
		style.ItemSpacing = ImVec2(10.0, 10.0);

		ImFontConfig iconConfig;
		iconConfig.MergeMode = true;
		iconConfig.PixelSnapH = true;
		iconConfig.OversampleH = 3;
		iconConfig.OversampleV = 3;

		static const ImWchar iconRanges[] = {
			ICON_MIN_FA,
			ICON_MAX_FA,
			0
		};

		Vars::gRegularFont = LoadFont(io, Vars::assetsPath + "\\fonts\\BuilderSans-Regular.ttf", 18.0f);
		Vars::gMediumFont = LoadFont(io, Vars::assetsPath + "\\fonts\\BuilderSans-Bold.ttf", 20.0f);
		Vars::gLargeFont = LoadFont(io, Vars::assetsPath + "\\fonts\\BuilderSans-Bold.ttf", 22.0f);
		Vars::gExtraLargeFont = LoadFont(io, Vars::assetsPath + "\\fonts\\BuilderSans-ExtraBold.ttf", 40.0f);
		Vars::gIconFont = LoadFont(io, Vars::assetsPath + "\\fonts\\FontAwesome.ttf", 23.0f, &iconConfig, iconRanges);

		ManageDefaultTheme();
	}

	void OnUpdate()
	{
		Vars::onUpdateCallback();
		//ImGui::RenderNotifications();
	}

	void INIT(const std::string& assetsPath, MenuColorScheme colorScheme, const std::function<void()>& onUpdateCallback)
	{
		Vars::onUpdateCallback = onUpdateCallback;
		Vars::assetsPath = assetsPath;
		Vars::gMenuColorScheme = colorScheme;

		UIBackend::START(OnLoad, OnUpdate);
	}
}


namespace UIComponents
{
	void IRender::Render()
	{

	}

	void IComponent::RenderChildren()
	{
		if (!this->visible)
		{
			return;
		}

		for (IRender* child : children)
		{
			if (child->visible)
			{
				child->Render();
			}
		}
	}

	void IComponent::AddChild(IComponent* child)
	{
		// you shouldn't be adding yourself bruh
		if (this == child)
		{
			return;
		}

		children.push_back(child);
	}

	void IComponent::AddTag(const char* tagname)
	{
		if (tagname == nullptr) return;
		UIFramework::TagService::TagComponent(tagname, this);
	}

	IComponent::~IComponent()
	{
		// TODO: Handle children deconstruction
	}

	IConfigurable::IConfigurable()
	{
		UIFramework::ConfigManager::configurableCompsList.push_back(this);
	}

	void IConfigurable::HandleConfigLoad(const nlohmann::json& data)
	{

	}

	nlohmann::json::object_t IConfigurable::HandleConfigSave()
	{
		return {};
	}

	MainWindow::MainWindow(ImVec2 windowSize)
	{
		this->windowSize = windowSize;
	}

	void MainWindow::Render()
	{
		if (this->visible)
		{
			UIFramework::MainWindow(windowSize, this);
		}
	}

	Section::Section(MainWindow* parentWindow, const char* icon)
	{
		this->icon = icon;
		parentWindow->AddChild(this);
	}

	void Section::Render()
	{
		UIFramework::Widgets::Section(this);
	}

	Tab::Tab(Section* parentTab, const char* label, GroupSplitType splitType)
	{
		this->splitType = splitType;
		this->label = label;
		parentTab->AddChild(this);
	}

	void Tab::Render()
	{
		UIFramework::Widgets::Tab(this);
	}

	Group::Group(Tab* parentTab, const char* label, GroupPlacementType placementType)
	{
		this->label = label;
		this->placementType = placementType;
		parentTab->AddChild(this);
	}

	Group::Group(Tab* parentTab, const char* label, ImVec2 intialSize, GroupPlacementType placementType):
		intialSize(intialSize)
	{
		this->label = label;
		this->placementType = placementType;
		parentTab->AddChild(this);
	}

	void Group::Render()
	{
		UIFramework::Widgets::Group(this, this->intialSize);
	}

	Text::Text(Group* parentGroup, const char* text)
	{
		this->text = text;
		parentGroup->AddChild(this);
	}

	Text::Text(Group* parentGroup, const char* text, const char* tag)
	{
		this->text = text;
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void Text::Render()
	{
		ImGui::PushTextWrapPos();
		ImGui::TextUnformatted(text.c_str());
		ImGui::PopTextWrapPos();
	}

	Checkbox::Checkbox(Group* parentGroup, const char* label, bool defaultValue) : IConfigurable()
	{
		this->label = label;
		this->value = defaultValue;
		parentGroup->AddChild(this);
	}

	Checkbox::Checkbox(Group* parentGroup, const char* label, const char* tag, bool defaultValue) : IConfigurable()
	{
		this->label = label;
		this->value = defaultValue;
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void Checkbox::Render()
	{
		bool previousValue = value;
		UIFramework::Widgets::Checkbox(label.c_str(), &value);

		if (previousValue != value)
		{
			for (auto& event : onToggleEventEntry)
			{
				event(value);
			}
		}
	}

	bool Checkbox::IsActive() const
	{
		return value;
	}

	void Checkbox::OnToggle(const std::function<void(bool value)>& callback)
	{
		onToggleEventEntry.push_back(callback);
	}

	void Checkbox::HandleConfigLoad(const nlohmann::json& data)
	{
		this->value = data["value"];
	}

	nlohmann::json::object_t Checkbox::HandleConfigSave()
	{
		return nlohmann::json::object({{"value", this->value}});
	}

	Button::Button(Group* parentGroup, const char* label, ButtonSizeType buttonSizeType)
	{
		this->label = label;
		this->buttonSizeType = buttonSizeType;
		parentGroup->AddChild(this);
	}

	Button::Button(Group* parentGroup, const char* label, const char* tag, ButtonSizeType buttonSizeType)
	{
		this->label = label;
		this->buttonSizeType = buttonSizeType;
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void Button::Render()
	{
		if (UIFramework::Widgets::Button(label.c_str(), this->buttonSizeType))
		{
			for (auto& event : onClickEventEntry)
			{
				event();
			}

			for (auto& event : onClickAsyncEventEntry)
			{
				TaskScheduler::RunAsync(event);
			}
		}
	}

	void Button::OnClick(const std::function<void()>& callback)
	{
		onClickEventEntry.push_back(callback);
	}

	void Button::OnClickAsync(const std::function<void()>& callback)
	{
		onClickAsyncEventEntry.push_back(callback);
	}

	IntSlider::IntSlider(Group* parentGroup, const char* label, int min, int max, int defaultValue) : IConfigurable()
	{
		this->label = label;
		this->min = min;
		this->max = max;
		this->value = defaultValue;
		parentGroup->AddChild(this);
	}

	IntSlider::IntSlider(Group* parentGroup, const char* label, const char* tag, int min, int max, int defaultValue) : IConfigurable()
	{
		this->label = label;
		this->min = min;
		this->max = max;
		this->value = defaultValue;
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void IntSlider::Render()
	{
		UIFramework::Widgets::SliderInt(label.c_str(), &value, min, max);
	}

	void IntSlider::HandleConfigLoad(const nlohmann::json& data)
	{
		this->value = data["value"];
	}

	nlohmann::json::object_t IntSlider::HandleConfigSave()
	{
		return nlohmann::json::object({{ "value", this->value }});
	}

	FloatSlider::FloatSlider(Group* parentGroup, const char* label, float min, float max, float defaultValue) : IConfigurable()
	{
		this->label = label;
		this->min = min;
		this->max = max;
		this->value = defaultValue;
		parentGroup->AddChild(this);
	}

	FloatSlider::FloatSlider(Group* parentGroup, const char* label, const char* tag, float min, float max, float defaultValue) : IConfigurable()
	{
		this->label = label;
		this->min = min;
		this->max = max;
		this->value = defaultValue;
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void FloatSlider::Render()
	{
		UIFramework::Widgets::SliderFloat(label.c_str(), &value, min, max);
	}

	void FloatSlider::HandleConfigLoad(const nlohmann::json& data)
	{
		this->value = data["value"];
	}

	nlohmann::json::object_t FloatSlider::HandleConfigSave()
	{
		return nlohmann::json::object({{ "value", this->value }});
	}

	StringInput::StringInput(Group* parentGroup, const char* label, const char* defaultValue, size_t maxLength,
		const char* hint, ImVec2 sizeArg) : IConfigurable()
	{
		this->label = label;
		this->maxLength = maxLength;
		this->sizeArg = sizeArg;
		this->value = std::unique_ptr<char[]>(new char[maxLength]);
		std::copy(defaultValue, defaultValue + maxLength, value.get());
		parentGroup->AddChild(this);
	}

	StringInput::StringInput(Group* parentGroup, const char* label, const char* tag, const char* defaultValue, size_t maxLength,
		const char* hint, ImVec2 sizeArg) : IConfigurable()
	{
		this->label = label;
		this->maxLength = maxLength;
		this->sizeArg = sizeArg;
		this->value = std::unique_ptr<char[]>(new char[maxLength]);
		std::copy(defaultValue, defaultValue + maxLength, value.get());
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void StringInput::Render()
	{
		UIFramework::Widgets::InputString(label.c_str(), hint.c_str(), value.get(), maxLength, sizeArg);
	}

	std::string StringInput::GetValue()
	{
		return { this->value.get()};
	}

	void StringInput::SetValue(const std::string& value)
	{
		memcpy(this->value.get(), value.c_str(), this->maxLength);
	}

	void StringInput::HandleConfigLoad(const nlohmann::json& data)
	{
		SetValue(data["value"]);
	}

	nlohmann::json::object_t StringInput::HandleConfigSave()
	{
		return nlohmann::json::object({{ "value", GetValue() }});
	}

	IntInput::IntInput(Group* parentGroup, const char* label, int defaultValue, int min, int max, const char* hint, ImVec2 sizeArg)
		: IConfigurable()
	{
		this->label = label;
		this->value = defaultValue;
		this->min = min;
		this->max = max;
		this->hint = hint;
		parentGroup->AddChild(this);
	}

	IntInput::IntInput(Group* parentGroup, const char* label, const char* tag, int defaultValue, int min, int max, const char* hint, ImVec2 sizeArg)
		: IConfigurable()
	{
		this->label = label;
		this->value = defaultValue;
		this->min = min;
		this->max = max;
		this->hint = hint;
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void IntInput::Render()
	{
		value = std::clamp(value, min, max);
		UIFramework::Widgets::InputInt(label.c_str(), hint.c_str(), &value);
	}

	void IntInput::HandleConfigLoad(const nlohmann::json& data)
	{
		this->value = data["value"];
	}

	nlohmann::json::object_t IntInput::HandleConfigSave()
	{
		return nlohmann::json::object({{ "value", this->value }});
	}

	Browser::Browser(Group* parentGroup, const char* label, const std::vector<std::string>& list, int defaultIndex) : IConfigurable()
	{
		this->label = label;
		this->list = list;
		this->index = defaultIndex;

		parentGroup->AddChild(this);
	}

	Browser::Browser(Group* parentGroup, const char* label, const char* tag, const std::vector<std::string>& list, int defaultIndex) : IConfigurable()
	{
		this->label = label;
		this->list = list;
		this->index = defaultIndex;
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void Browser::Render()
	{
		UIFramework::Widgets::Browser(label.c_str(), &index, &list);
	}

	void Browser::HandleConfigLoad(const nlohmann::json& data)
	{
		this->index = data["index"];
	}

	nlohmann::json::object_t Browser::HandleConfigSave()
	{
		return nlohmann::json::object({{ "index", this->index }});
	}

	Mode::Mode(Group* parentGroup, const char* label, const std::vector<std::string>& modes, int defaultIndex) : IConfigurable()
	{
		this->label = label;
		this->modes = modes;
		this->index = defaultIndex;

		parentGroup->AddChild(this);
	}

	Mode::Mode(Group* parentGroup, const char* label, const char* tag, const std::vector<std::string>& modes, int defaultIndex) : IConfigurable()
	{
		this->label = label;
		this->modes = modes;
		this->index = defaultIndex;
		this->AddTag(tag);
		parentGroup->AddChild(this);
	}

	void Mode::Render()
	{
		UIFramework::Widgets::Mode(label.c_str(), &index, &modes);
	}

	void Mode::HandleConfigLoad(const nlohmann::json& data)
	{
		this->index = data["index"];
	}

	nlohmann::json::object_t Mode::HandleConfigSave()
	{
		return nlohmann::json::object({{ "index", this->index }});
	}

	ColorPicker::ColorPicker(Group* parentGroup, const char* label, const ImColor& defaultColor)
	{
		this->label = label;
		this->SetColor(defaultColor);
		parentGroup->AddChild(this);
	}

	ImColor ColorPicker::GetColor()
	{
		return ImColor(colorBuffer[0], colorBuffer[1], colorBuffer[2], colorBuffer[3]);
	}

	void ColorPicker::SetColor(const ImColor& color)
	{
		this->colorBuffer[0] = color.Value.x;
		this->colorBuffer[1] = color.Value.y;
		this->colorBuffer[2] = color.Value.z;
		this->colorBuffer[3] = color.Value.w;
	}

	void ColorPicker::Render()
	{
		ImGui::ColorPicker4(label.c_str(), colorBuffer);
	}
}