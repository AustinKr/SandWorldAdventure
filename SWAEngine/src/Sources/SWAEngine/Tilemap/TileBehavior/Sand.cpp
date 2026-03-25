#include "SWAEngine/Tilemap/TileBehavior/Sand.h"
#include <string> // TODO: Remove include

namespace SWAEngine::Tilemap::TileBehavior
{

	Sand::Sand() :
		m_TileVelocities{}
	{

	}

	void Sand::OnCreate(Tile tile, Math::Vector2Int pos)
	{
		m_TileVelocities.insert(std::make_pair(pos, Math::Vector2{}));
	}
	void Sand::OnRemove(Tile tile, Math::Vector2Int pos)
	{
		m_TileVelocities.erase(pos);
	}


	void Sand::Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time)
	{
		/*auto belowPos = pos - Math::Vector2Int(0, 1);
		Tile pendingTile = pTilemap->GetTile(belowPos);

		if (pTilemap->IsInBounds(belowPos) && !pendingTile.HasValue)
		{
			pTilemap->SwapTiles(belowPos, pos, pendingTile);
		}*/
			


		//// Apply gravity
		//m_TileVelocities.at(pos) += {0, -time.FrameDeltaTime};

		//// Apply velocity
		pTilemap->TryStepMoveTile(pos, Math::Vector2(0, -2));
		//fprintf(stdout, std::to_string(pTilemap->TryStepMoveTile(pos, Math::Vector2(0, -2))).c_str());
		//fprintf(stdout, "\n");
	}
}