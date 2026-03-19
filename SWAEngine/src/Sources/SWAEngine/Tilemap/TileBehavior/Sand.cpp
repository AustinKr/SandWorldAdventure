#include "SWAEngine/Tilemap/TileBehavior/Sand.h"

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
		auto belowPos = pos - Math::Vector2Int(0, 1);
		Tile pendingTile = pTilemap->GetTile(belowPos);

		//if (pTilemap->IsInBounds(belowPos) && !pTilemap->GetActiveTile(belowPos).HasValue)
		if (pTilemap->IsInBounds(belowPos) && !pendingTile.HasValue) // TODO: I cant figure out why it is creating duplicate tiles and not erasing original
		{
			// TODO: Not removing origin tile
			pTilemap->SwapTiles(belowPos, pos, pendingTile);
			// TODO: I am just manually erasing the original
			pTilemap->SetTile(pos, {});
		}
			


		//// Apply gravity
		//m_TileVelocities.at(pos) += {0, -time.FrameDeltaTime};

		//// Apply velocity

		//pTilemap->TryStepMoveTile(pos, Math::Vector2(0, -1));
	}
}