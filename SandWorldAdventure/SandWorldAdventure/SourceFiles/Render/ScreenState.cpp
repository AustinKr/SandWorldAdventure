#include "HeaderFiles/Render/ScreenState.h"

namespace SandboxEngine::Render
{
	ScreenState::ScreenState() { Data = nullptr; BitmapInfo = {}; };
	ScreenState::ScreenState(int width, int height)
	{
		Data = nullptr;
		BitmapInfo = {};
		Resize(width, height);
	}

	void ScreenState::Resize(int newWidth, int newHeight)
	{
		Release();
		// Assign new data

		// Create info
		BitmapInfo = {};
		BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BitmapInfo.bmiHeader.biSizeImage = sizeof(unsigned int) * newWidth * newHeight;
		BitmapInfo.bmiHeader.biWidth = newWidth;
		BitmapInfo.bmiHeader.biHeight = newHeight;
		BitmapInfo.bmiHeader.biBitCount = 32;
		BitmapInfo.bmiHeader.biPlanes = 1;
		BitmapInfo.bmiHeader.biCompression = BI_RGB;

		// Create memory
		Data = VirtualAlloc(nullptr, BitmapInfo.bmiHeader.biSizeImage, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}

	void ScreenState::Release()
	{
		if (Data != nullptr)
		{
			// Clean up old data
			VirtualFree(Data, NULL, MEM_RELEASE);
			BitmapInfo.bmiHeader.biWidth = 0;
			BitmapInfo.bmiHeader.biHeight = 0;
			Data = nullptr;
		}
	}
}