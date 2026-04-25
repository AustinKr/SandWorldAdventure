#pragma once
#include "IBehavior.h"
#include <SWAEngine/Math/vector2.h>

namespace SWAEngine::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API Sand : IBehavior
	{
	public:
		virtual void Update(Tile tile, Math::Vector2Int pos, Component::TilemapComponent* const pTilemap, Math::Time time) override;

		virtual Tile CreateNew(Math::Time time) override;
	//private:
		//void TryMove(Math::Vector2 movement, Math::Vector2Int pos, Tilemap* const pTilemap);
	};
}