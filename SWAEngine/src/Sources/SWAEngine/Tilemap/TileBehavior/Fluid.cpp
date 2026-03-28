#include "SWAEngine/Tilemap/TileBehavior/Fluid.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"

namespace SWAEngine::Tilemap::TileBehavior
{
	void Fluid::OnCreate(Tile tile, Math::Vector2Int pos)
	{

	}
	void Fluid::OnRemove(Tile tile, Math::Vector2Int pos)
	{

	}
	void Fluid::OnClone(Tile original, Math::Vector2Int originalPos, Math::Vector2Int newPos)
	{

	}

	void Fluid::Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time)
	{
		// Move down if empty space
		auto pair = pTilemap->TryStepMoveTile(pos, Math::Vector2Int(0, -1));
		if (pair.first != pos)
		{
			pTilemap->SwapTiles(pair.first, pos);
			return;
		}

		// Try move sideways, favoring empty space
		int dir = signbit((float)rand() / (float)RAND_MAX - .5f) * 2 - 1;
		// Try move one way
		Tile otherTile1 = TryMoveSideways(pos, dir, pTilemap);
		if (otherTile1.HasValue)
		{
			// Not empty space, so try move other way
			Tile otherTile2 = TryMoveSideways(pos, -dir, pTilemap);
			// Still not empty space so just try swap whichever
			if (otherTile2.BehaviorUID == FLUID)
				pTilemap->SwapTiles(pos + Math::Vector2Int(-dir, 0), pos); 
			else if(otherTile1.BehaviorUID == FLUID)
				pTilemap->SwapTiles(pos + Math::Vector2Int(dir, 0), pos);
		}
	}

	Tile Fluid::TryMoveSideways(Math::Vector2Int pos, int dir, Tilemap* const pTilemap)
	{
		Math::Vector2Int otherPos = pos + Math::Vector2Int(dir, 0);
		if (!pTilemap->IsInBounds(otherPos))
			return {};

		Tile otherTile = pTilemap->GetTile(otherPos);

		if (!otherTile.HasValue)
		{
			pTilemap->SwapTiles(otherPos, pos);
			return {};
		}

		return otherTile;
	}
}