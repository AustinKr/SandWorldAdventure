#include "SWAEngine/Tilemap/TileBehavior/Sand.h"
#include <string> // TODO: Remove include

namespace SWAEngine::Tilemap::TileBehavior
{

	Sand::Sand() /*:
		m_ActiveTileVelocities{},
		m_ClonedTileVelocities{}*/
	{

	}

	void Sand::OnCreate(Tile tile, Math::Vector2Int pos)
	{
		//// Default: (0,0)
		//Math::Vector2 velocity = {};

		//// Try carry over velocity
		//if (m_ClonedTileVelocities.contains(pos))
		//{
		//	velocity = m_ClonedTileVelocities.at(pos);
		//	m_ClonedTileVelocities.erase(pos); // clear up space for new clones
		//}

		//m_ActiveTileVelocities[pos] = velocity;
	}
	void Sand::OnRemove(Tile tile, Math::Vector2Int pos)
	{
		//m_ActiveTileVelocities.erase(pos);
	}
	void Sand::OnClone(Tile original, Math::Vector2Int originalPos, Math::Vector2Int newPos)
	{
		//m_ClonedTileVelocities[newPos] = m_ActiveTileVelocities.at(originalPos);
	}


	void Sand::Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time)
	{
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





		//if(!tile.HasValue)
		//	fprintf(stdout, "update null tile\n");
		//if (!m_ActiveTileVelocities.contains(pos) && m_ClonedTileVelocities.contains(pos))
		//	fprintf(stdout, "non-applied clone\n");

		//if (!m_ActiveTileVelocities.contains(pos)) // TODO: Shouldn't need thius
		//{
		//	//OnCreate(tile, pos);
		//	return;
		//}

		//// Apply gravity
		//float speed = -2;
		//Math::Vector2 velocity = (m_ActiveTileVelocities.at(pos) + Math::Vector2(0, speed* time.FrameDeltaTime)) * .9f;
		//m_ActiveTileVelocities.at(pos) = velocity;

		//if (velocity.GetMagnitudeSqrd() < 1)
		//{
		//	// Try update again next frame
		//	TileBehavior::IBehavior::s_Behaviors.at(tile.BehaviorUID)->OnClone(tile, pos, pos);
		//	pTilemap->SetTile(pos, tile);
		//	return;
		//}
		//// We have enough momentum to move
		//TryMove(velocity, tile, pos, pTilemap, time);
	}

	void Sand::TryMove(Math::Vector2 velocity, Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap)
	{
		// Apply velocity
		auto pair = pTilemap->TryStepMoveTile(pos, Math::Vector2(0, velocity.Y)); // Try move laterally
		pair = pTilemap->TryStepMoveTile(pair.first, Math::Vector2(velocity.X, 0)); // Try move again horizontally

		// Return if we didn't actually move
		if (pair.first == pos)
			return;

		//// Clone the tile(retaining velocity)
		//TileBehavior::IBehavior::s_Behaviors.at(tile.BehaviorUID)->OnClone(tile, pos, pair.first);
		// Swap original with translated(empty space)
		pTilemap->SwapTiles(pair.first, pos, Tile());

		// TODO: I shouldn't be getting a different behavior when I specify using tile rather than retreive the active tile( they should already be the same)
	}
}