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

		inline Vector2 ViewportToWorld(Vector2 viewport, bool applyOffsets = true)
		{
			if (applyOffsets)
				viewport -= Vector2(.5, .5); // Center
			viewport *= Scale * WORLD_UNITS; // Scale
			if (applyOffsets)
				viewport += Origin;			 // Camera origin

			return viewport;
		}
		inline Vector2 WorldToViewport(Vector2 worldPosition, bool applyOffsets = true)
		{
			if (applyOffsets)
				worldPosition -= Origin;					// Camera origin
			worldPosition /= (Scale * double(WORLD_UNITS)); // Scale
			if(applyOffsets)
				worldPosition += Vector2(.5, .5);			// Center
			
			return worldPosition;
		}

		inline Vector2 ScreenToViewport(Vector2Int screen)
		{
			return (Vector2)screen / (Vector2)ScreenSize;
		}
		inline Vector2Int ViewportToScreen(Vector2 viewport)
		{
			return viewport *ScreenSize;
		}
	};
}