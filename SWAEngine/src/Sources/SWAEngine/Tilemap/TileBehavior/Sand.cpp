#include "SWAEngine/Tilemap/TileBehavior/Sand.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"
#include "SWAEngine/Tilemap/TilePropertyManager/Data/SandData.h"
#include <utility>
#include <string> // TODO: Remove include

namespace SWAEngine::Tilemap::TileBehavior
{
	void Sand::Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time)
	{
		if (tile.p_Properties == nullptr)
			return; // No properties given to this tile
		
		// TODO: not sure rvalue works here...
		auto& rProperties = *static_cast<TilePropertyManager::Data::SandData*>(tile.p_Properties);
		rProperties.Velocity += Math::Vector2(0, -5) * time.FrameDeltaTime;
		rProperties.Velocity *= .98;

		Math::Vector2 movement = rProperties.Velocity * time.FrameDeltaTime;
		if (abs(movement.X) < 1 && abs(movement.Y) < 1)
		{
			// Can't move, try again next frame
			pTilemap->SetTile(pos, tile); // Update
			return;
		}

		TryMove(movement, pos, pTilemap);


		//// Apply gravity
		//float speed = -2;
		//tile.Velocity = (tile.Velocity + Math::Vector2(0, speed * time.FrameDeltaTime)) * .999f; // note this is a copy

		//if (tile.Velocity.GetMagnitudeSqrd() < 1)
		//{
		//	// Try update again next frame
		//	pTilemap->SetTile(pos, tile);
		//	return;
		//}
		//// We have enough momentum to move
		//TryMove(tile.Velocity, tile, pos, pTilemap);

	}

	Tile Sand::CreateNew()
	{
		Tile tile = {SAND, 0x0, true};
		tile.p_Properties = new TilePropertyManager::Data::SandData();
		return tile;
	}

	void Sand::TryMove(Math::Vector2 movement, Math::Vector2Int pos, Tilemap* const pTilemap)
	{
		// Apply movement
		auto pair = pTilemap->TryStepMoveTile(pos, Math::Vector2(0, movement.Y)); // Try move laterally
		pair = pTilemap->TryStepMoveTile(pair.first, Math::Vector2(movement.X, 0)); // Try move again horizontally

		// Return if we didn't actually move
		if (pair.first == pos)
			return;

		// Swap original with translated(empty space)
		pTilemap->SwapTiles(pair.first, pos);
	}
}