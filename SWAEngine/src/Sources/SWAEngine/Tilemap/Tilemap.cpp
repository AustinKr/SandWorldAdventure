#include "SWAEngine/Tilemap/Tilemap.h"
#include "SWAEngine/Tilemap/TilemapContainer.h"

namespace SWAEngine::Tilemap
{
	Tilemap::Tilemap(Math::Vector2 origin, Math::Vector2 scale)
	{
		Origin = origin;
		TileScale = scale;

		mp_ActiveTilesContainer = new TilemapContainer();
		mp_PendingTilesContainer = new TilemapContainer();
	}
	void Tilemap::Release()
	{
		delete(mp_ActiveTilesContainer);
		delete(mp_PendingTilesContainer);
		delete(this);
	}
	bool Tilemap::IsEmpty()
	{
		return mp_ActiveTilesContainer->Size() == 0;
	}

	Tile Tilemap::GetTile(Math::Vector2Int position)
	{
		if (!mp_ActiveTilesContainer->Contains(position))
			return {};

		return mp_ActiveTilesContainer->Get(position);
	}
	Tile& Tilemap::SetTile(Math::Vector2Int position, Tile tile)
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

	Math::Vector2 Tilemap::TileToWorld(Math::Vector2Int tile, bool applyOffsets)
	{
		Math::Vector2 world = (Math::Vector2)tile * TileScale;
		if (applyOffsets)
			world += Origin;
		return world;
	}
	Math::Vector2Int Tilemap::WorldToTile(Math::Vector2 world, bool applyOffsets)
	{
		if (applyOffsets)
			world -= Origin;
		return world / TileScale;
	}

	Math::Vector2Int Tilemap::GetBounds()
	{
		return mp_ActiveTilesContainer->GetBounds();
	}

	void Tilemap::ApplyPendingTiles()
	{
		mp_PendingTilesContainer->Iterate([&](Math::Vector2Int pos, Tile& rTile)
		{
			if (rTile.HasValue)
				mp_ActiveTilesContainer->Set(pos, rTile, true);
			else
				mp_ActiveTilesContainer->Erase(pos); // Remove tile from memory

			return true; // continue iteration
		});

		mp_PendingTilesContainer->Clear();
	}
}