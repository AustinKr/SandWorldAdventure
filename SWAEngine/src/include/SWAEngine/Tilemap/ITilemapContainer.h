#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Tilemap/Tile.h"
#include <functional>

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API ITilemapContainer
	{
		virtual bool Contains(Vector2Int position) = 0;
		virtual Tile& Get(Vector2Int position) = 0;
		virtual Tile& Set(Vector2Int position, Tile tile, bool shouldOverride = true) = 0;
		virtual void Erase(Vector2Int position) = 0;

		virtual void Iterate(std::function<bool(Vector2Int, Tile&)> func) = 0;
	};
}