#pragma once
#include "HeaderFiles/Vector2.h"
#include <Windows.h>
#include <algorithm>
#include "ScreenState.h"

namespace SandboxEngine::Render
{
	class Renderer
	{
	public:
		static const double GAME_UNIT_SIZE;

		static Vector2 FromWorldToScreen(ScreenState* pScreen, Vector2 world, bool center = true);
		static Vector2 FromScreenToWorld(ScreenState* pScreen, Vector2 screen, bool center = true);

		static UINT* GetPixel(ScreenState* pScreen, int i);
		static UINT* GetPixel(UINT** ppData, int i);
		static void Background(ScreenState* pScreen, UINT color);

		static void DrawRectPixels(ScreenState* pScreen, int x, int y, int width, int height, UINT color);
		static void DrawRect(ScreenState* pScreen, double x, double y, double width, double height, UINT color);

		static void CopyState(ScreenState* pScreenSource, int sourceX, int sourceY, ScreenState* pScreenDestination, int destinationX, int destinationY, int nullColor = -1);

	};
}