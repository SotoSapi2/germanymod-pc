#pragma once
#include <imgui.h>
#include <string>
#include <vector>
#include <json.hpp>

namespace UIComponents
{
	enum class GroupSplitType : uint8_t
	{
		NO_SPLIT,
		HALF
	};

	enum class GroupPlacementType
	{
		AUTO,
		LEFT,
		RIGHT
	};

	enum class ButtonSizeType
	{
		WINDOW_SIZE,
		FLEXIBLE,
	};

	class IRender
	{
	public:
		bool visible = true;

		virtual void Render();
	};

	class IComponent : public IRender
	{
	public:
		std::vector<IComponent*> children;

		virtual void RenderChildren();

		virtual void AddChild(IComponent* child);

		virtual void AddTag(const char* tagname);

		virtual ~IComponent();
	};

	class ILabel
	{
	public:
		std::string label;
	};

	class IConfigurable : public ILabel
	{
		public:
		IConfigurable();

		virtual void HandleConfigLoad(const nlohmann::json& data);

		virtual nlohmann::json::object_t HandleConfigSave();
	};

	class MainWindow : public IComponent
	{
	public:
		ImVec2 windowSize;

		MainWindow(ImVec2 windowSize);

		void Render() override;
	};

	class Section : public IComponent
	{
	public:
		const char* icon;

		Section(MainWindow* parentWindow, const char* icon);

		void Render() override;
	};

	class Tab : public IComponent, public ILabel
	{
	public:
		GroupSplitType splitType;

		Tab(Section* parentTab, const char* label, GroupSplitType splitType = GroupSplitType::HALF);

		void Render() override;
	};

	class Group : public IComponent, public ILabel
	{
		public:
		GroupPlacementType placementType;
		const ImVec2 intialSize;

		Group(Tab* parentTab, const char* label, GroupPlacementType placementType = GroupPlacementType::AUTO);

		Group(Tab* parentTab, const char* label, ImVec2 intialSize, GroupPlacementType placementType = GroupPlacementType::AUTO);

		void Render() override;
	};

	class Text : public IComponent
	{
	public:
		std::string text;

		Text(Group* parentGroup, const char* text);

		Text(Group* parentGroup, const char* text, const char* tag);

		void Render() override;
	};

	class Checkbox : public IComponent, public IConfigurable
	{
	public:
		bool value;
		std::vector<std::function<void(bool value)>> onToggleEventEntry;

		Checkbox(Group* parentGroup, const char* label, bool defaultValue = false);

		Checkbox(Group* parentGroup, const char* label, const char* tag, bool defaultValue = false);

		void Render() override;

		virtual bool IsActive() const;

		virtual void OnToggle(const std::function<void(bool value)>& callback);

		void HandleConfigLoad(const nlohmann::json& data) override;

		nlohmann::json::object_t HandleConfigSave() override;
	};

	class Button : public IComponent, public ILabel
	{
	public:
		ButtonSizeType buttonSizeType;
		std::vector<std::function<void()>> onClickEventEntry;
		std::vector<std::function<void()>> onClickAsyncEventEntry;

		Button(Group* parentGroup, const char* label, ButtonSizeType buttonSizeType = ButtonSizeType::FLEXIBLE);

		Button(Group* parentGroup, const char* label, const char* tag, ButtonSizeType buttonSizeType = ButtonSizeType::FLEXIBLE);

		void Render() override;

		virtual void OnClick(const std::function<void()>& callback);
		virtual void OnClickAsync(const std::function<void()>& callback);
	};

	class IntSlider : public IComponent, public IConfigurable
	{
	public:
		int value = 0;
		int min = 0;
		int max = 0;

		IntSlider(Group* parentGroup, const char* label, int min, int max, int defaultValue = 0);

		IntSlider(Group* parentGroup, const char* label, const char* tag, int min, int max, int defaultValue = 0);

		void Render() override;

		void HandleConfigLoad(const nlohmann::json& data) override;

		nlohmann::json::object_t HandleConfigSave() override;
	};

	class FloatSlider : public IComponent, public IConfigurable
	{
	public:
		float value = 0;
		float min = 0;
		float max = 0;

		FloatSlider(Group* parentGroup, const char* label, float min, float max, float defaultValue = 0);

		FloatSlider(Group* parentGroup, const char* label, const char* tag, float min, float max, float defaultValue = 0);

		void Render() override;

		void HandleConfigLoad(const nlohmann::json& data) override;

		nlohmann::json::object_t HandleConfigSave() override;
	};

	class StringInput : public IComponent, public IConfigurable
	{
		std::unique_ptr<char[]> value;
		size_t maxLength;

	public:
		std::string hint;
		ImVec2 sizeArg;

		StringInput(Group* parentGroup, const char* label, const char* defaultValue,
			size_t maxLength = 255, const char* hint = "input", ImVec2 sizeArg = { -1, 0 });

		StringInput(Group* parentGroup, const char* label, const char* tag, const char* defaultValue,
			size_t maxLength, const char* hint = "input", ImVec2 sizeArg = { -1, 0 });

		void Render() override;

		std::string GetValue();

		void SetValue(const std::string& value);

		void HandleConfigLoad(const nlohmann::json& data) override;

		nlohmann::json::object_t HandleConfigSave() override;
	};

	class IntInput : public IComponent, public IConfigurable
	{
	public:
		std::string hint;
		int value;
		int min;
		int max;
		ImVec2 sizeArg;

		IntInput(Group* parentGroup, const char* label, int defaultValue = 0, int min = INT_MIN, int max = INT_MAX,
			const char* hint = "input", ImVec2 sizeArg = { -1, 0 });

		IntInput(Group* parentGroup, const char* label, const char* tag,
			int defaultValue = 0, int min = INT_MIN, int max = INT_MAX, const char* hint = "input", ImVec2 sizeArg = { -1, 0 });

		void Render() override;

		void HandleConfigLoad(const nlohmann::json& data) override;

		nlohmann::json::object_t HandleConfigSave() override;
	};

	class Browser : public IComponent, public IConfigurable
	{
	public:
		std::vector<std::string> list;
		int index = 0;

		Browser(Group* parentGroup, const char* label, const std::vector<std::string>& list = {}, int defaultIndex = 0);

		Browser(Group* parentGroup, const char* label, const char* tag, const std::vector<std::string>& list = {}, int defaultIndex = 0);

		void Render() override;

		void HandleConfigLoad(const nlohmann::json& data) override;

		nlohmann::json::object_t HandleConfigSave() override;
	};

	class Mode : public IComponent, public IConfigurable
	{
	public:
		std::vector<std::string> modes;
		int index = 0;

		Mode(Group* parentGroup, const char* label, const std::vector<std::string>& modes, int defaultIndex = 0);

		Mode(Group* parentGroup, const char* label, const char* tag, const std::vector<std::string>& modes, int defaultIndex = 0);

		void Render() override;

		void HandleConfigLoad(const nlohmann::json& data) override;

		nlohmann::json::object_t HandleConfigSave() override;
	};

	class ColorPicker : public IComponent, public IConfigurable
	{
	public:
		float colorBuffer[4];

		ColorPicker(Group* parentGroup, const char* label, const ImColor& defaultColor = ImColor(1, 0, 0, 1));

		ImColor GetColor();

		void SetColor(const ImColor& color);

		void Render() override;
	};
}
