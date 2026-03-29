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
	Tile& TilemapContainer::Set(TilePropertyManager::PropertyManager& rManager, Math::Int3 location, Tile tile, bool shouldOverride)
	{
		// Update bounds
		if (location.X > m_Bounds.X)
			m_Bounds.X = location.X;
		if (location.Y > m_Bounds.Y)
			m_Bounds.Y = location.Y;
		
		// Try erase
		if (shouldOverride && Contains({ location.X, location.Y }))
		{
			// Handle edge case where the existing property data is also the data we are setting, so don't erase it from memory
			bool shouldRelease = Get({ location.X, location.Y }).p_Properties != tile.p_Properties;
			// Erase
			Erase(rManager, location, shouldRelease);
		}

		// Try add tile
		auto insertion = m_TilesRegistry.insert(std::make_pair(Math::Vector2Int(location.X, location.Y), tile));

		// Try add to property's shared tiles (if successfully added new tile, properties exist, and not already recorded as shared)
		if (insertion.second && tile.p_Properties != nullptr)
			rManager.DataContainer[tile.p_Properties].insert(location);

		return insertion.first->second;
	}
	void TilemapContainer::Erase(TilePropertyManager::PropertyManager& rManager, Math::Int3 location, bool shouldRelease)
	{
		// Try release memory 
		rManager.TryEraseData(Get({location.X, location.Y}).p_Properties, location, shouldRelease);

		// Erase tile
		m_TilesRegistry.erase({location.X, location.Y});
	}
	void TilemapContainer::Clear()
	{
		m_TilesRegistry.clear();
	}

	void TilemapContainer::Iterate(std::function<bool(Math::Vector2Int, Tile&)> func)
	{
		auto cpy = m_TilesRegistry;
		for (auto& pair : cpy)
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