#include "SWAEngine/Tilemap/Tilemap.h"
#include "SWAEngine/Tilemap/TilemapContainer.h"
#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"
#include <algorithm>

namespace SWAEngine::Tilemap
{
	const unsigned int Tilemap::MAX_MOVE_STEPS = 50;

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

	Tile Tilemap::GetActiveTile(Math::Vector2Int position)
	{
		if (!mp_ActiveTilesContainer->Contains(position))
			return {};

		return mp_ActiveTilesContainer->Get(position);
	}
	Tile Tilemap::GetTile(Math::Vector2Int position)
	{
		// Try get active
		if (!mp_PendingTilesContainer->Contains(position))
			return GetActiveTile(position);

		// Get pending
		return mp_PendingTilesContainer->Get(position);
	}

	Tile& Tilemap::SetTile(Math::Vector2Int position, Tile tile)
	{
		if (position.X < 0 || position.Y < 0)
			throw std::exception(); // TODO: Allow expansion of tilemap

		return mp_PendingTilesContainer->Set(position, tile);
	}
	void Tilemap::SwapTiles(Math::Vector2Int a, Math::Vector2Int b, Tile tileA, Tile tileB)
	{
		if(!tileA.HasValue)
			tileA = GetActiveTile(a);
		if(!tileB.HasValue)
			tileB = GetActiveTile(b);

		if (!tileA.HasValue && !tileB.HasValue)
			return;

		SetTile(a, tileB);
		SetTile(b, tileA);
	}
	bool Tilemap::TryStepMoveTile(Math::Vector2Int start, Math::Vector2 movement, int maxSteps)
	{
		Math::Vector2Int current = start;
		Math::Vector2 direction = movement.Normalize();
		Tile hitTile = {};
		int step = 0, endSqrd = std::min(maxSteps * maxSteps, (int)floor(movement.GetMagnitudeSqrd()));
		for (step = 0; step*step < endSqrd; step++)
		{
			current = start + direction * step;
			if (!IsInBounds(current))
				break;
			// Check whether hit a tile
			hitTile = GetTile(current);
			if (hitTile.HasValue)
			{
				step--;
				break; 
			}
			// Else, hit nothing
		}
		// Make sure we didn't run out of steps or didn't actually move
		if (step == maxSteps || current == start)
			return false;

		// Successfully moved
		SwapTiles(current, start, hitTile);
		return true;
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
	bool Tilemap::IsInBounds(Math::Vector2Int tile)
	{
		return tile.X >= 0 && tile.Y >= 0 && tile.X < GetBounds().X && tile.Y < GetBounds().Y;
	}

	Tilemap::TILES Tilemap::ApplyPendingTiles(Time time)
	{
		TILES tilesToUpdate = {};

		mp_PendingTilesContainer->Iterate([&](Math::Vector2Int pos, Tile& rPendingTile)
		{
			if (rPendingTile.HasValue) // Add
			{
				// Try call old tile ::OnRemove()
				if (mp_ActiveTilesContainer->Contains(pos))
				{
					Tile& rOldTile = mp_ActiveTilesContainer->Get(pos);
					if (rOldTile.BehaviorUID != NULL)
						TileBehavior::IBehavior::s_Behaviors.at(rOldTile.BehaviorUID)
						->OnRemove(rOldTile, pos);
				}

				// Add tile
				mp_ActiveTilesContainer->Set(pos, rPendingTile, true);
				if (rPendingTile.BehaviorUID != NULL)
				{
					// Queue update
					tilesToUpdate.insert(std::make_pair(pos, rPendingTile));
					// Call current tile ::OnCreate()
					TileBehavior::IBehavior::s_Behaviors.at(rPendingTile.BehaviorUID)
						->OnCreate(rPendingTile, pos);
				}
			}
			else if (mp_ActiveTilesContainer->Contains(pos)) // Remove
			{
				// Call old tile ::OnRemove()
				Tile& rOldTile = mp_ActiveTilesContainer->Get(pos);
				if (rOldTile.BehaviorUID != NULL)
					TileBehavior::IBehavior::s_Behaviors.at(rOldTile.BehaviorUID)
					->OnRemove(rOldTile, pos);

				mp_ActiveTilesContainer->Erase(pos); // Remove tile from memory
			}

			// Queue update surrounding
			for (auto& off : SURROUNDING_TILES)
			{
				Tile tile = GetActiveTile(off + pos);
				if (tile.HasValue && tile.BehaviorUID != NULL)
					tilesToUpdate.insert(std::make_pair(off + pos, tile));
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
			TileBehavior::IBehavior::s_Behaviors.at(pair.second.BehaviorUID)->Update(pair.second, pair.first, this, time);
		}
	}
}