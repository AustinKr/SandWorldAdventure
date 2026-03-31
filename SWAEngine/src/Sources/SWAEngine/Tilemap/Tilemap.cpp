#include "SWAEngine/Tilemap/Tilemap.h"
#include "SWAEngine/Tilemap/TilemapContainer.h"
#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"
#include <algorithm>
#include <string> // TODO: Remove include

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

	const int Tilemap::ACTIVE_TILES_ID = 1;
	const int Tilemap::PENDING_TILES_ID = 2;

	Tilemap::Tilemap(const char* name, Math::Vector2 origin, Math::Vector2 scale) : BaseGameObject(name)
	{
		PropertyManager = {};

		Origin = origin;
		TileScale = scale;

		mp_ActiveTilesContainer = new TilemapContainer();
		mp_PendingTilesContainer = new TilemapContainer();
	}
	void Tilemap::Update(Time time)
	{
		UpdateTiles(time, ApplyPendingTiles(time));
	}
	void Tilemap::Release()
	{
		delete(mp_ActiveTilesContainer);
		delete(mp_PendingTilesContainer);
		
		BaseGameObject::Release();
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
	Tile Tilemap::GetTile(Math::Vector2Int position, __out int* containerID)
	{
		// Try get pending
		if (mp_PendingTilesContainer->Contains(position))
		{
			if (containerID != nullptr)
				*containerID = PENDING_TILES_ID;
			return mp_PendingTilesContainer->Get(position);
		}

		// Get active
		if (containerID != nullptr)
			*containerID = ACTIVE_TILES_ID;
		return GetActiveTile(position);
	}

	Tile Tilemap::SetTile(Math::Vector2Int position, Tile tile)
	{
		if (position.X < 0 || position.Y < 0)
			throw std::exception(); // TODO: Allow expansion of tilemap

		return mp_PendingTilesContainer->Set(PropertyManager, { position.X, position.Y, PENDING_TILES_ID}, tile, true);
	}
	void Tilemap::SwapTiles(Math::Vector2Int a, Math::Vector2Int b)
	{
		Tile tileA = GetTile(a);
		Tile tileB = GetTile(b);

		if (!tileA.HasValue && !tileB.HasValue)
			return;

		// Queue set and register to properties' shared tiles

		// Make cross references
		if (tileB.p_Properties != nullptr)
			PropertyManager.DataContainer[tileB.p_Properties].insert({ a.X, a.Y, PENDING_TILES_ID });
		if(tileA.p_Properties != nullptr)
			PropertyManager.DataContainer[tileA.p_Properties].insert({ b.X, b.Y, PENDING_TILES_ID });

		// Set a to b
		mp_PendingTilesContainer->Set(PropertyManager, { a.X, a.Y, PENDING_TILES_ID }, tileB, true);
		// Set b to a
		mp_PendingTilesContainer->Set(PropertyManager, { b.X, b.Y, PENDING_TILES_ID }, tileA, true);
	}
	std::pair<Math::Vector2Int, Tile> Tilemap::TryStepMoveTile(Math::Vector2Int origin, Math::Vector2 movement, int maxSteps)
	{
		float magnitude = sqrt(movement.GetMagnitudeSqrd());
		Math::Vector2 direction = movement / magnitude;

		Tile hitTile = {}; // Default: hit nothing
		Math::Vector2Int start = origin + direction, current = start; // Default: starts at the start which is one step ahead of the original tile
		
		int step = 0, end = std::min(maxSteps, (int)floor(magnitude));
		
		// Iterate
		for (step; step < end;) 
		{
			// Check if not in bounds
			if (!IsInBounds(current)) // TODO: !this means that tiles cant move to expand- need to add option!
			{
				// Counts as hit something!
				step--; // Step back
				break;
			}
			// Else, check if there is a tile
			hitTile = GetTile(current);
			if (hitTile.HasValue)
			{
				// Hit something!
				step--; // Step back
				break;
			}
			
			step++; // Step forward
			// Update current position
			current = start + Math::Vector2Int(direction * (float)step);
		}

		// Step back if we reached the end without colliding (because end is a count and we want the position)
		if (step == end)
			step--;

		// Recompute final position
		current = start + Math::Vector2Int(direction * (float)step);

		// Make sure we didn't run out of steps or didn't actually move
		if (step == maxSteps || current == origin)
			return std::make_pair(origin, Tile());

		// Can successfully move!
		return std::make_pair(current, hitTile);
	}

	bool Tilemap::DetectCollisionRect(Math::Vector2Int bottomLeft, Math::Vector2Int topRight) // TODO: Make tilemap collision detection only check for certain tiles
	{
		return mp_ActiveTilesContainer->DetectCollisionRect(bottomLeft, topRight);
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
				// (all that is happening with shared system is the container ID is changing from pending to active)
				// Add tile and register to shared
				mp_ActiveTilesContainer->Set(PropertyManager, {pos.X, pos.Y, ACTIVE_TILES_ID}, rPendingTile, true);
				if (rPendingTile.BehaviorUID != NULL)
					tilesToUpdate.insert(std::make_pair(pos, rPendingTile));
				
				// Remove pending from shared
				PropertyManager.TryEraseData(rPendingTile.p_Properties, { pos.X, pos.Y, PENDING_TILES_ID });
			}
			else if (mp_ActiveTilesContainer->Contains(pos) && rPendingTile.p_Properties == nullptr) // Remove
			{
				// Remove the active tile
				mp_ActiveTilesContainer->Erase(PropertyManager, {pos.X, pos.Y, ACTIVE_TILES_ID});
			}
			else if (rPendingTile.p_Properties != nullptr)
			{
				throw std::exception("Empty tiles shouldn't have properties!");
			}

			// Queue update surrounding
			for (auto& off : SURROUNDING_TILES)
			{
				Tile tile = GetTile(off + pos); // Active or pending because they haven't all been applied yet
				if (tile.HasValue && tile.BehaviorUID != NULL)
					tilesToUpdate.insert(std::make_pair(off + pos, tile));
			}

			return true; // continue iteration
		});

		// TODO: Remove at some point stat output or make it into a gui display
		if (int(time.CurrentTime * 30) % 30 == 0)
		{
			fprintf(stdout, "Tile Total Properties Size: ");
			fprintf(stdout, std::to_string(PropertyManager.DataContainer.size()).c_str());
			fprintf(stdout, "\n");
		}

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