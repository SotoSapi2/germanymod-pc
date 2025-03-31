#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>
namespace GdiplusManager
{
	extern ULONG_PTR gdiplusToken;
	extern CLSID pngClsid;

	bool CropPng(const std::wstring& filepath, Gdiplus::Rect cropArea, Gdiplus::Size resizeResolution, std::vector<BYTE>* memoryBuffer);

	void INIT();

	void Shutdown();
}