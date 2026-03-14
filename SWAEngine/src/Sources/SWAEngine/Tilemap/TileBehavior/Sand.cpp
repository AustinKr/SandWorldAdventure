#include "SWAEngine/Tilemap/TileBehavior/Sand.h"

namespace SWAEngine::Tilemap::TileBehavior
{
	void Sand::Update(Tilemap* const pTilemap, Time time, Math::Vector2Int pos, Tile tile)
	{
		/*if ((int)time.CurrentTime % 2 != 0)
		{
			pTilemap->SetTile(pos, tile);
			return;
		}*/

		auto belowPos = pos - Math::Vector2Int(0, 1);
		if (belowPos.Y < 0)
			return;

		Tile below = pTilemap->GetTile(belowPos);
		if (below.HasValue)
			return;

		pTilemap->SetTile(belowPos, tile);
		pTilemap->SetTile(pos, below);
	}
}