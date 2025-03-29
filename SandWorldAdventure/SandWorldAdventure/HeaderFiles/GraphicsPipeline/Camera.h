#pragma once
#include "HeaderFiles/Math.h"

namespace SandboxEngine::GraphicsPipeline
{
	struct Camera
	{
	public:
		const int WORLD_UNITS = 1;

		Vector2 Origin;
		Vector2 Scale;

		inline Camera() : Origin({0,0}), Scale({1,1})
		{
			/*nothing*/
		}

		inline Vector2 WorldToViewport(Vector2 worldPosition)
		{
			worldPosition -= Origin;
			return worldPosition / (Scale * WORLD_UNITS);
		}

		inline Vector2 ViewportToWorld(Vector2 viewport)
		{
			viewport* (Scale * WORLD_UNITS);
			return viewport + Origin;
		}
	};
}