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
		auto pair = pTilemap->TryStepMoveTile(pos, Math::Vector2Int(0, -1));
		if (pair.first != pos)
		{
			pTilemap->SwapTiles(pair.first, pos);
			return;
		}

		int dir = signbit((float)rand() / (float)RAND_MAX - .5f) * 2 - 1;
		Math::Vector2Int otherPos = pos + Math::Vector2Int(1, 0) * dir;
		if (!pTilemap->IsInBounds(otherPos))
			return;

		Tile belowTile = pTilemap->GetTile(otherPos);
		
		if (belowTile.HasValue && belowTile.BehaviorUID != FLUID)
			return;

		pTilemap->SwapTiles(otherPos, pos);
	}
}