#pragma once
#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"

namespace SWAEngine::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API Fluid : IBehavior
	{
	public:
		virtual void Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time) override;
		virtual Tile CreateNew() override;

	private:
		Tile TryMoveSideways(Math::Vector2Int pos, int dir, Tilemap* const pTilemap);
	};
}