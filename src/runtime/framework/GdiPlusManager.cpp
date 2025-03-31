#include <string>
#include <cassert>
#include "GdiPlusManager.hpp"
#pragma comment (lib, "gdiplus.lib")

namespace GdiplusManager
{
	ULONG_PTR gdiplusToken;
	CLSID pngClsid;

	bool ReadBitmapBytes(Gdiplus::Bitmap* bitmap, CLSID clsid, std::vector<BYTE>* memoryBuffer)
	{
		assert(bitmap != nullptr);
		assert(memoryBuffer != nullptr);
		if (bitmap == nullptr || memoryBuffer == nullptr)
		{
			return false;
		}

		IStream* memStream = nullptr;
		if (CreateStreamOnHGlobal(nullptr, TRUE, &memStream) != S_OK)
		{
			return false;
		}

		if (bitmap->Save(memStream, &clsid, nullptr) != Gdiplus::Ok)
		{
			memStream->Release();
			return false;
		}

		STATSTG statstg;
		if (memStream->Stat(&statstg, STATFLAG_NONAME) != S_OK) {
			memStream->Release();
			return false;
		}

		ULONG imageSize = statstg.cbSize.LowPart;
		memoryBuffer->resize(imageSize);

		LARGE_INTEGER zero = {};
		memStream->Seek(zero, STREAM_SEEK_SET, nullptr);

		ULONG bytesRead;
		memStream->Read(memoryBuffer->data(), imageSize, &bytesRead);
		memStream->Release();

		return (bytesRead == imageSize);
	}

	bool CropPng(Gdiplus::Bitmap* ogBitmap, Gdiplus::Rect cropArea, Gdiplus::Size resizeResolution, std::vector<BYTE>* memoryBuffer)
	{
		assert(ogBitmap != nullptr);
		assert(memoryBuffer != nullptr);
		if (ogBitmap == nullptr || memoryBuffer == nullptr)
		{
			return false;
		}

		Gdiplus::Bitmap resizedImg(resizeResolution.Width, resizeResolution.Height);//(64, 32);
		Gdiplus::Graphics graphics(&resizedImg);
		graphics.DrawImage(
			ogBitmap,
			cropArea,
			cropArea.X,
			cropArea.Y,
			cropArea.Width,
			cropArea.Height,
			Gdiplus::UnitPixel
		);

		return ReadBitmapBytes(&resizedImg, pngClsid, memoryBuffer);
	}

	bool CropPng(const std::wstring& filepath, Gdiplus::Rect cropArea, Gdiplus::Size resizeResolution, std::vector<BYTE>* memoryBuffer)
	{
		assert(memoryBuffer != nullptr);
		if (memoryBuffer == nullptr)
		{
			return false;
		}

		Gdiplus::Bitmap originalImg(filepath.c_str());
		if (originalImg.GetLastStatus() != Gdiplus::Ok)
		{
			return false;
		}

		return CropPng(&originalImg, cropArea, resizeResolution, memoryBuffer);
	}

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
		using namespace Gdiplus;
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		ImageCodecInfo* pImageCodecInfo = NULL;

		GetImageEncodersSize(&num, &size);
		if (size == 0)
			return -1;  // Failure

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL)
			return -1;  // Failure

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}
		}

		free(pImageCodecInfo);
		return -1;  // Failure
	}

	void INIT()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

		GetEncoderClsid(L"image/png", &pngClsid);
	}

	void Shutdown()
	{
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}
}