#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Tilemap/Tile.h"
#include <unordered_map>

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API Tilemap
	{
	private:
		std::unordered_map<Int2, Tile> m_TilesRegistry;

	public:
		void GetTile();
		void SetTile();
	};
}