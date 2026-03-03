#include "SWAEngine/Tilemap/TilemapContainer.h"

namespace SWAEngine::Tilemap
{
	bool TilemapContainer::Contains(Vector2Int position)
	{
		return m_TilesRegistry.contains(position);
	}
	Tile& TilemapContainer::Get(Vector2Int position)
	{
		return m_TilesRegistry.at(position);
	}
	Tile& TilemapContainer::Set(Vector2Int position, Tile tile, bool shouldOverride)
	{
		if (shouldOverride && Contains(position))
		{
			m_TilesRegistry.at(position) = tile;
			return m_TilesRegistry.at(position);
		}
		m_TilesRegistry.insert(std::make_pair(position, tile));
		return m_TilesRegistry.at(position);
	}
	void TilemapContainer::Erase(Vector2Int position)
	{
		m_TilesRegistry.erase(position);
	}
	void TilemapContainer::Iterate(std::function<bool(Vector2Int, Tile&)> func)
	{
		for (auto& pair : m_TilesRegistry)
		{
			if (!func(pair.first, pair.second))
				return;
		}
	}
}