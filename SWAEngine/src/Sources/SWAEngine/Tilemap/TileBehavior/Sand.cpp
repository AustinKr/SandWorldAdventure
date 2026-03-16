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

		if (!pTilemap->GetTile(belowPos).HasValue && pTilemap->IsInBounds(belowPos))
			pTilemap->SwapTiles(pos, belowPos);


		//// Apply gravity
		//m_TileVelocities.at(pos) += {0, -time.FrameDeltaTime};

		//// Apply velocity

	}
}