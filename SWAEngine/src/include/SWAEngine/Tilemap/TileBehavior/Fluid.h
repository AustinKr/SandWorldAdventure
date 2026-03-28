#pragma once
#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"

namespace SWAEngine::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API Fluid : IBehavior
	{
	public:
		virtual void OnCreate(Tile tile, Math::Vector2Int pos) override;
		virtual void OnRemove(Tile tile, Math::Vector2Int pos) override;
		virtual void OnClone(Tile original, Math::Vector2Int originalPos, Math::Vector2Int newPos) override;

		virtual void Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time) override;

	private:
		Tile TryMoveSideways(Math::Vector2Int pos, int dir, Tilemap* const pTilemap);
	};
}