#pragma once
#include "HeaderFiles/Math.h"

namespace SandboxEngine::GraphicsPipeline
{
	struct Camera
	{
	public:
		const int WORLD_UNITS = 1;
		Vector2Int ScreenSize;

		// Middle of the camera
		Vector2 Origin;
		// Scaling factor(total width and height across)
		Vector2 Scale;

		inline Camera() : Origin({0,0}), Scale({1,1}), ScreenSize({0,0})
		{
			/*nothing*/
		}

		inline Vector2 ViewportToWorld(Vector2 viewport)
		{
			viewport -= Vector2(.5, .5);
			viewport *= Scale * WORLD_UNITS;
			return viewport + Origin;
		}
		// TODO: Add a boolean to only transform through scale
		inline Vector2 WorldToViewport(Vector2 worldPosition)
		{
			worldPosition -= Origin;
			return worldPosition / (Scale * double(WORLD_UNITS)) + Vector2(.5, .5);
		}

		inline Vector2 ScreenToViewport(Vector2Int screen)
		{
			return (Vector2)screen / (Vector2)ScreenSize;// (screen / ScreenSize + Vector2(1, 1)) / 2.0;
		}
		inline Vector2Int ViewportToScreen(Vector2 viewport)
		{
			return viewport *ScreenSize;// (viewport * 2.0 - Vector2(1, 1))* ScreenSize;
		}
	};
}