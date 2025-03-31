#include <IL2CPP.hpp>
#include "../util/HookingUtil.hpp"
#include "Global.hpp"
#include "GameplayMain.hpp"
#include "import/PointerFunctions.hpp"
#include "utils/MemPatcher.hpp"

namespace AntiAnalytics
{
	IL2CPP::Class* inGameConnection_class = nullptr;
	IL2CPP::Class* idk_class = nullptr;
	IL2CPP::Class* analytics_class = nullptr;

	bool anlayticsNopped = false;

	void NopAnalyticsMethods()
	{
		MemPatcher::Nop(inGameConnection_class->GetMethod(0x5)->GetPointer());
		MemPatcher::Nop(idk_class->GetMethod(0x6)->GetPointer());

		for (size_t i = 0; i < analytics_class->MethodCount(); i++)
		{
			IL2CPP::MethodInfo* info = analytics_class->GetMethod(i);
			if (info->GetReturnClass() == IL2CPP::DefaultTypeClass::Void)
			{
				MemPatcher::Nop(info->GetPointer());
			}
		}

		anlayticsNopped = true;
	}

	void RestoreAnalyticsMethods()
	{
		MemPatcher::Restore(inGameConnection_class->GetMethod(0x5)->GetPointer());
		MemPatcher::Restore(idk_class->GetMethod(0x6)->GetPointer());

		for (size_t i = 0; i < analytics_class->MethodCount(); i++)
		{
			IL2CPP::MethodInfo* info = analytics_class->GetMethod(i);
			if (info->GetReturnClass() == IL2CPP::DefaultTypeClass::Void)
			{
				MemPatcher::Restore(info->GetPointer());
			}
		}

		anlayticsNopped = false;
	}

	$Hook(void, ExperienceController_AddExp, (IL2CPP::Object* _this, int value, int reason, IL2CPP::Object* dic, IL2CPP::Object* sex))
	{
		bool prevAnlayticsNopped = anlayticsNopped;

		if (anlayticsNopped)
		{
			RestoreAnalyticsMethods();
		}

		$CallOrig(ExperienceController_AddExp, _this, value, reason, dic, sex);

		if (prevAnlayticsNopped)
		{
			NopAnalyticsMethods();
		}
	}

	void INIT()
	{
		using namespace IL2CPP::ClassMapping;

		inGameConnection_class = GetClass("InGameConnection");
		idk_class = GetClass("SexClass0");
		analytics_class = GetClass("AnalyticsClass");

		$RegisterHook(
			ExperienceController_AddExp,
			ExperienceController::AddExperience.GetPointer()
		);
	}
}