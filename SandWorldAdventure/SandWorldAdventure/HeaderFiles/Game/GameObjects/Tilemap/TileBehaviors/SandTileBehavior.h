#pragma once
#include "TileBehavior.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class SandTileBehavior : private TileBehavior
	{
	private:
		const int GRAVITY = 25; // use G/10.0

		// Returns whether it moved
		bool TryApplyMotion(
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			TilemapContainer::TILE_INFO tileInfo,
			Time time);
	public:
		virtual bool TryUpdate(
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			TilemapContainer::TILE_INFO tileInfo,
			Time time) override;
	};
}