#include "SWAEngine/Tilemap/TilemapContainer.h"

namespace SWAEngine::Tilemap
{
	TilemapContainer::TilemapContainer()
	{
		m_TilesRegistry = {};
		m_Bounds = {};
	}

	Math::Vector2Int TilemapContainer::GetBounds()
	{
		return m_Bounds;
	}
	int TilemapContainer::Size()
	{
		return m_TilesRegistry.size();
	}

	bool TilemapContainer::Contains(Math::Vector2Int position)
	{
		return m_TilesRegistry.contains(position);
	}
	Tile& TilemapContainer::Get(Math::Vector2Int position)
	{
		return m_TilesRegistry.at(position);
	}
	Tile& TilemapContainer::Set(Math::Vector2Int position, Tile tile, bool shouldOverride)
	{
		// Update bounds
		if (position.X > m_Bounds.X)
			m_Bounds.X = position.X;
		if (position.Y > m_Bounds.Y)
			m_Bounds.Y = position.Y;

		// Set
		if (shouldOverride && Contains(position))
		{
			m_TilesRegistry.at(position) = tile;
			return m_TilesRegistry.at(position);
		}
		
		return m_TilesRegistry.insert(std::make_pair(position, tile)).first->second;
	}
	void TilemapContainer::Erase(Math::Vector2Int position)
	{
		m_TilesRegistry.erase(position);
	}
	void TilemapContainer::Clear()
	{
		m_TilesRegistry.clear();
	}

	void TilemapContainer::Iterate(std::function<bool(Math::Vector2Int, Tile&)> func)
	{
		for (auto& pair : m_TilesRegistry)
		{
			if (!func(pair.first, pair.second))
				return;
		}
	}

	bool TilemapContainer::DetectCollisionRect(Math::Vector2Int bottomLeft, Math::Vector2Int topRight)
	{
		// TODO: if using acceleration structure in the future, this should check larger areas before individual tiles
		for (int x = bottomLeft.X; x < topRight.X; x++)
		{
			for (int y = bottomLeft.Y; y < topRight.Y; y++)
			{
				if (Contains({ x,y }))
					return true;
			}
		}
		return false;
	}
}