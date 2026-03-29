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
		float deltaTime = time.CurrentTime - pProperties->LastUpdateTime;

		pProperties->Velocity += Math::Vector2(0, -1) * deltaTime;
		pProperties->Velocity *= .98;

		Math::Vector2 movement = pProperties->Velocity * deltaTime + Math::Vector2(0, -1);
		if (abs(movement.X) < 1 && abs(movement.Y) < 1)
		{
			// Can't move, try again next frame
			pTilemap->SetTile(pos, tile); // Update
			return;
		}

		pProperties->LastUpdateTime = time.CurrentTime;
		TryMove(movement, pos, pTilemap);
	}

	Tile Sand::CreateNew(Time time)
	{
		Tile tile = {SAND, 0x0, true};
		tile.p_Properties = new TilePropertyData::SandData{ {}, (float)time.CurrentTime };
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