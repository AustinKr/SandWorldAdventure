#include "SWAEngine/Tilemap/Tilemap.h"
#include "SWAEngine/Tilemap/TilemapContainer.h"

namespace SWAEngine::Tilemap
{
	Tilemap::Tilemap()
	{
		mp_ActiveTilesContainer = new TilemapContainer();
		mp_PendingTilesContainer = new TilemapContainer();
	}
	void Tilemap::Release()
	{
		delete(mp_ActiveTilesContainer);
		delete(mp_PendingTilesContainer);
		delete(this);
	}

	Tile Tilemap::GetTile(Vector2Int position)
	{
		return mp_ActiveTilesContainer->Get(position);
	}
	Tile& Tilemap::SetTile(Vector2Int position, Tile tile)
	{
		return mp_PendingTilesContainer->Set(position, tile);
	}

	void Tilemap::Update() 
	{
		ApplyPendingTiles();

		// TODO: Update behaviors
	/* mp_ActiveTilesContainer->Iterate([]()
		{

		});*/
	}

	void Tilemap::ApplyPendingTiles()
	{
		mp_PendingTilesContainer->Iterate([&](Vector2Int pos, Tile& rTile)
		{
			if (rTile.HasValue)
				mp_ActiveTilesContainer->Set(pos, rTile, true);
			else
				mp_ActiveTilesContainer->Erase(pos); // Remove tile from memory

			// TODO: update graphics

			return true; // continue iteration
		});
	}
}