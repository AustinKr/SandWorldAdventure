#pragma once
#include "ScreenState.h"
#include "HeaderFiles/Vector2.h"

namespace SandboxEngine::Render
{
	const unsigned int NULL_CAMERA_COLOR = 0xff00ff;

	class Camera
	{
	public:
		Vector2 Position;
		Vector2 Scale;

		ScreenState MainScreen;

		inline Camera() { Position = {}; Scale = {}; MainScreen = {}; }
		inline Camera(Vector2 pos, Vector2 scale, Vector2 resize) { Position = pos; Scale = scale; MainScreen = ScreenState(resize.X, resize.Y); }

		// Draws to the camera screen from world coordinates
		void DrawRect(ScreenState* pScreen, double x, double y, double width, double height, UINT color);


		Vector2 FromWorldToViewport(Vector2 world, bool center = true);
		Vector2 FromViewportToWorld(Vector2 screen, bool center = true);

		Vector2 FromViewportToScreen(Vector2 viewport, bool center = true);
		Vector2 FromScreenToViewport(Vector2 screen, bool center = true);
	};
}