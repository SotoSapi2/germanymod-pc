#pragma once
namespace AntiAnalytics
{
	extern bool anlayticsNopped;

	void NopAnalyticsMethods();

	void RestoreAnalyticsMethods();

	void INIT();
}