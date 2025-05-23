#pragma once
#include "TileBehavior.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class SolidTileBehavior : private TileBehavior
	{
	public:
		virtual bool TryUpdate(
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			TilemapContainer::TILE_INFO tileInfo,
			Time time,
			TileActionTypes action) override;
	};
}

// GOOD FOR NEW IMPLEMENTATION