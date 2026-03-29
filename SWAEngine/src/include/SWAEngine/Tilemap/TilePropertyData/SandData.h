#pragma once
#include "SWAEngine/Math/vector2.h"

namespace SWAEngine::Tilemap::TilePropertyData
{
	// Defines extra properties to assocaite with sand Tiles
	struct SandData
	{
		Math::Vector2 Velocity;
		float LastUpdateTime;
	};
}