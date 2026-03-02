#pragma once
#include "TileBehavior.h"

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class SandTileBehavior : private TileBehavior
	{
	private:
		const int GRAVITY = 5;

		// Returns whether it moved
		bool TryApplyMotion(
			Vector2 translation,
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			TilemapContainer::TILE_INFO tileInfo);
	public:
		virtual bool TryUpdate(
			Tilemap* pTilemap,
			Vector2Int tilePosition,
			TilemapContainer::TILE_INFO tileInfo,
			Time time) override;
	};
}