#include "SWAEngine/Tilemap/Tilemap.h"
#include "SWAEngine/Tilemap/TilemapContainer.h"
#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"

namespace SWAEngine::Tilemap
{
	const Math::Vector2Int Tilemap::SURROUNDING_TILES[8] =
	{
		Math::Vector2Int(-1, 1),
		Math::Vector2Int(-1, 0),
		Math::Vector2Int(-1, -1),
		Math::Vector2Int(0, -1),
		Math::Vector2Int(1, -1),
		Math::Vector2Int(1, 0),
		Math::Vector2Int(1, 1),
		Math::Vector2Int(0, 1),
	};

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
		if (position.X < 0 || position.Y < 0)
			throw std::exception(); // TODO: Allow expansion of tilemap

		return mp_PendingTilesContainer->Set(position, tile);
	}

	bool Tilemap::DetectCollisionRect(Math::Vector2Int bottomLeft, Math::Vector2Int topRight)
	{
		return mp_ActiveTilesContainer->DetectCollisionRect(bottomLeft, topRight);
	}

	void Tilemap::Update(Time time)
	{
		UpdateTiles(time, ApplyPendingTiles(time));
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

	Tilemap::TILES Tilemap::ApplyPendingTiles(Time time)
	{
		TILES tilesToUpdate = {};

		mp_PendingTilesContainer->Iterate([&](Math::Vector2Int pos, Tile& rTile)
		{
			if (rTile.HasValue)
			{
				mp_ActiveTilesContainer->Set(pos, rTile, true); // Add tile
				tilesToUpdate.insert(std::make_pair(pos, rTile));
			}
			else
				mp_ActiveTilesContainer->Erase(pos); // Remove tile from memory
			
			// Update surrounding
			for (auto& off : SURROUNDING_TILES)
			{
				tilesToUpdate.insert(std::make_pair(off + pos, GetTile(off + pos)));
			}

			return true; // continue iteration
		});

		mp_PendingTilesContainer->Clear();
		return tilesToUpdate;
	}
	void Tilemap::UpdateTiles(Time time, TILES tiles)
	{
		for (auto& pair : tiles)
		{
			TryUpdateTile(time, pair.first, pair.second);
		}
	}

	void Tilemap::TryUpdateTile(Time time, Math::Vector2Int pos, Tile tile)
	{
		if (!tile.HasValue || tile.BehaviorUID == NULL)
			return;

		TileBehavior::IBehavior::s_Behaviors.at(tile.BehaviorUID)->Update(this, time, pos, tile);
	}
}