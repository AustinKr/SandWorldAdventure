#include "SWAEngine/Tilemap/TileBehavior/Sand.h"
#include "SWAEngine/Tilemap/TileBehavior/Types.h"
#include "SWAEngine/Tilemap/TilePropertyData/SandData.h"
#include <utility>

namespace SWAEngine::Tilemap::TileBehavior
{
	void Sand::Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time)
	{
		if (tile.p_Properties == nullptr)
			return; // No properties given to this tile

		auto pProperties = static_cast<TilePropertyData::SandData*>(tile.p_Properties);
		float deltaTime = time.FrameDeltaTime;

		pProperties->Velocity += Math::Vector2(0, -1);
		//pProperties->Velocity *= .98;

		Math::Vector2 movement = pProperties->Velocity * deltaTime + Math::Vector2(0, -1);
		if (abs(movement.X) < 1 && abs(movement.Y) < 1)
		{
			// Can't move, try again next frame
			pTilemap->SetTile(pos, tile); // Refresh
			return;
		}

		TryMove(movement, pos, pTilemap);

		//TODO: if this one failed, go to the bottom of the stack and work way up
	}

	Tile Sand::CreateNew(Time time)
	{
		Tile tile = {SAND, 0x0, true};
		tile.p_Properties = new TilePropertyData::SandData{ {} };
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