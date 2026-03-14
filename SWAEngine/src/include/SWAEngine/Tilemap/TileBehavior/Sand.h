#pragma once
#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"

namespace SWAEngine::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API Sand : IBehavior
	{
		virtual void Update(Tilemap* const pTilemap, Time time, Math::Vector2Int pos, Tile tile) override;
	};
}