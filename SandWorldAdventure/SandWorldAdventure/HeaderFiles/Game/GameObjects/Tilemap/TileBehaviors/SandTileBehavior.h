#pragma once
#include "TileBehavior.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class SandTileBehavior : private TileBehavior
	{
	private:
		const int GRAVITY = 25; // use G/10.0

	public:
		virtual bool TryUpdate(
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			TilemapContainer::TILE_INFO tileInfo,
			Time time,
			TileActionTypes action) override;
	};
}