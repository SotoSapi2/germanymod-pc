#include "ClassFinder.hpp"

#include <Logger.hpp>
#include <iostream>
#include <unordered_map>
#include <functional>
#include "FieldPatterns.hpp"

namespace ClassManager
{
	std::unordered_map<std::string, Il2CppClass*> classMap{};

	void RegisterClass(Il2CppClass* klass, const char* name)
	{
		if (klass == nullptr)
		{
			LOG_ERROR("First Arg (Il2CppClass*) is null");
			return;
		}

		classMap.insert({ std::string(name), klass });
	}
	Il2CppClass* GetRegisteredClass(const char* name)
	{
		auto it = classMap.find(std::string(name));

		if (it == classMap.end())
		{
			LOG_ERROR("Could not find %s.", name);
			return nullptr;
		}

		if (it->second == nullptr)
		{
			LOG_ERROR("%s is null", name);
			return nullptr;
		}

		return it->second;
	}

	void ScanImage(const Il2CppImage* image, std::function<void(Il2CppClass*)> iterator)
	{
		for (int i = 0; i < il2cpp_image_get_class_count(image); i++)
		{
			auto klass = il2cpp_image_get_class(image, i);
			auto type = il2cpp_class_get_type(const_cast<Il2CppClass*>(klass));

			iterator(il2cpp_class_from_type(type));
		}
	}
}

Il2CppClass* GetClass(const char* name)
{
	return ClassManager::GetRegisteredClass(name);
}

static bool NAME_CHECK(Il2CppClass* klass, const char* name)
{
	return strcmp(il2cpp_class_get_name(klass), name) == 0;
}

static bool NAMESPACE_CHECK(Il2CppClass* klass, const char* namespaze)
{
	return strcmp(il2cpp_class_get_namespace(klass), namespaze) == 0;
}

static void REG_CLASS_NAMECHECK(Il2CppClass* klass, const char* name)
{
	if (NAME_CHECK(klass, name))
	{
		ClassManager::RegisterClass(klass, name);
	}
}

static void REG_CLASS_PATTERNCHECK(Il2CppClass* klass, size_t fieldCount, size_t methodCount, const FieldPattern& pattern, const char* className)
{
	if (Il2CppUtils::CheckFieldPattern(klass, fieldCount, methodCount, pattern))																
	{
		ClassManager::RegisterClass(klass, className);
	}
}

namespace ClassFinder
{
	void UnityCoreLoad(Il2CppClass* klass)
	{
		if (NAMESPACE_CHECK(klass, "UnityEngine"))
		{
			REG_CLASS_NAMECHECK(klass, "Screen");
			REG_CLASS_NAMECHECK(klass, "Time");
			REG_CLASS_NAMECHECK(klass, "Camera");
			REG_CLASS_NAMECHECK(klass, "Cursor");
			REG_CLASS_NAMECHECK(klass, "Component");
			REG_CLASS_NAMECHECK(klass, "Transform");
			REG_CLASS_NAMECHECK(klass, "Object");
			REG_CLASS_NAMECHECK(klass, "GameObject");
			REG_CLASS_NAMECHECK(klass, "SystemInfo");
			REG_CLASS_NAMECHECK(klass, "Application");
			REG_CLASS_NAMECHECK(klass, "TouchScreenKeyboard");
			REG_CLASS_NAMECHECK(klass, "MonoBehaviour");
			REG_CLASS_NAMECHECK(klass, "Resources");
			REG_CLASS_NAMECHECK(klass, "Debug");
			REG_CLASS_NAMECHECK(klass, "Quaternion");
			REG_CLASS_NAMECHECK(klass, "Physics");
		}
	}

	void AssemblyCSharpCheck(Il2CppClass* klass)
	{
		if (NAMESPACE_CHECK(klass, ""))
		{
			REG_CLASS_NAMECHECK(klass, "PixelTime");
			REG_CLASS_NAMECHECK(klass, "WeaponSounds");
			REG_CLASS_NAMECHECK(klass, "WeaponManager");
			REG_CLASS_NAMECHECK(klass, "UICamera");
		}

		if (NAMESPACE_CHECK(klass, "Progress"))
		{
			REG_CLASS_PATTERNCHECK(klass, 72, 471, FieldPatterns::ProgressUpdater, "ProgressUpdater");
		}

		if (NAMESPACE_CHECK(klass, "Rilisoft"))
		{
			REG_CLASS_PATTERNCHECK(klass, 1, 3, FieldPatterns::WebsocketSex0, "WebsocketSex0");
		}

		if (NAMESPACE_CHECK(klass, "Rilisoft.WebSocket"))
		{
			REG_CLASS_NAMECHECK(klass, "WebSocketManager");
		}
	}

	void INIT()
	{
		ClassManager::ScanImage(CommonLibImages::Corlib, UnityCoreLoad);
		ClassManager::ScanImage(CommonLibImages::AssemblyCSharp, AssemblyCSharpCheck);
	}
}