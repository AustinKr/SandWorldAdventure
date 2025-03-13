#pragma once
#include <Windows.h>

namespace SandboxEngine::Render
{
	struct ScreenState
	{
		BITMAPINFO BitmapInfo;
		void* Data;

		ScreenState();
		ScreenState(int width, int height);

		void Resize(int newWidth, int newHeight);
		void Release();
	};
}