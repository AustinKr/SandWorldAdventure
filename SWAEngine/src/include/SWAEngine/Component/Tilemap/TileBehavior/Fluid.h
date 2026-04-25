#pragma once
#include "IBehavior.h"

namespace SWAEngine::Component::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API Fluid : IBehavior
	{
	public:
		virtual void Update(Tile tile, Math::Vector2Int pos, TilemapComponent* const pTilemap, Math::Time time) override;
		virtual Tile CreateNew(Math::Time time) override;

	private:
		Tile TryMoveSideways(Math::Vector2Int pos, int dir, TilemapComponent* const pTilemap);
	};
}