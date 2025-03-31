#pragma once
#include <string>
#include <vector>
#include <windows.h>
#include <gdiplus.h>
namespace GdiplusManager
{
	extern ULONG_PTR gdiplusToken;
	extern CLSID pngClsid;

	bool ReadBitmapBytes(Gdiplus::Bitmap* bitmap, CLSID clsid, std::vector<BYTE>* memoryBuffer);

	bool CropPng(Gdiplus::Bitmap* ogBitmap, Gdiplus::Rect cropArea, Gdiplus::Size resizeResolution, std::vector<BYTE>* memoryBuffer);

	bool CropPng(const std::wstring& filepath, Gdiplus::Rect cropArea, Gdiplus::Size resizeResolution, std::vector<BYTE>* memoryBuffer);

	void INIT();

	void Shutdown();
}