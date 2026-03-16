#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Tilemap/Tile.h"
#include "SWAEngine/Math/vector2.h"
#include <functional>

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API ITilemapContainer
	{
		// Returns the bounds of the map in tiles
		// (the top-right-most tile position)
		virtual Math::Vector2Int GetBounds() = 0;
		virtual int Size() = 0;
		virtual void Clear() = 0;

		virtual Tile& Get(Math::Vector2Int position) = 0;
		virtual Tile& Set(Math::Vector2Int position, Tile tile, bool shouldOverride = true) = 0;
		virtual void Erase(Math::Vector2Int position) = 0;
		virtual bool Contains(Math::Vector2Int position) = 0;

		virtual void Iterate(std::function<bool(Math::Vector2Int, Tile&)> func) = 0;

		virtual bool DetectCollisionRect(Math::Vector2Int bottomLeft, Math::Vector2Int topRight) = 0;
		// TODO: Create more collision detection shapes if needed
		//virtual bool DetectCollisionElipse();
		//virtual bool DetectCollisionLine();
	};
}