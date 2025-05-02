#pragma once
#include "HeaderFiles/Math.h"

namespace SandboxEngine::GraphicsPipeline
{
	struct Camera
	{
	public:
		const int WORLD_UNITS = 1;

		//TODO: Maybe give camera width and height of screen
		/*
		
		inline Vector2 ScreenToViewport(Vector2 screen)
		{
			int screenW, screenH;
			glfwGetWindowSize(window, &screenW, &screenH);
			return screen / Vector2(screenW, screenH);
		}
		*/

		Vector2 Origin;
		Vector2 Scale;

		inline Camera() : Origin({0,0}), Scale({1,1})
		{
			/*nothing*/
		}

		inline Vector2 ViewportToWorld(Vector2 viewport)
		{
			viewport *= Scale * WORLD_UNITS;
			return viewport + Origin;
		}
		inline Vector2 WorldToViewport(Vector2 worldPosition)
		{
			worldPosition -= Origin;
			return worldPosition / (Scale * WORLD_UNITS);
		}
	};
}