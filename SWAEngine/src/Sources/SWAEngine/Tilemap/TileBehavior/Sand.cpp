#include "SWAEngine/Tilemap/TileBehavior/Sand.h"
#include "SWAEngine/Tilemap/TilePropertyManager/Data/SandData.h"
#include <string> // TODO: Remove include

namespace SWAEngine::Tilemap::TileBehavior
{

	Sand::Sand()
	{

	}

	void Sand::OnCreate(Tile tile, Math::Vector2Int pos)
	{
	}
	void Sand::OnRemove(Tile tile, Math::Vector2Int pos)
	{
	}
	void Sand::OnClone(Tile original, Math::Vector2Int originalPos, Math::Vector2Int newPos)
	{
	}


	void Sand::Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time)
	{
		//if (tile.p_Properties == nullptr)
		//	return; // No properties given to this tile
		
		//TilePropertyManager::PropertyManager::Get<TilePropertyManager::Data::SandData>(tile).Velocity;

		TryMove({ -1,-1 }, tile, pos, pTilemap);


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

	void Sand::TryMove(Math::Vector2 velocity, Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap)
	{
		// Apply velocity
		auto pair = pTilemap->TryStepMoveTile(pos, Math::Vector2(0, velocity.Y)); // Try move laterally
		pair = pTilemap->TryStepMoveTile(pair.first, Math::Vector2(velocity.X, 0)); // Try move again horizontally

		// Return if we didn't actually move
		if (pair.first == pos)
			return;

		// Swap original with translated(empty space)
		pTilemap->SwapTiles(pair.first, pos);

		// TODO: I shouldn't be getting a different behavior when I specify using tile rather than retreive the active tile( they should already be the same)
	}
}