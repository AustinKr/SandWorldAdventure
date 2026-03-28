#pragma once
#include "SWAEngine/Tilemap/ITilemapContainer.h"
#include <unordered_map>

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API TilemapContainer : public ITilemapContainer
	{
	private:
		std::unordered_map<Math::Vector2Int, Tile, Math::Vector2Hasher> m_TilesRegistry;

		Math::Vector2Int m_Bounds;

	public:
		TilemapContainer();

		virtual Math::Vector2Int GetBounds() override;
		virtual int Size() override;
		virtual void Clear() override;

		virtual Tile& Get(Math::Vector2Int position) override;
		virtual Tile& Set(TilePropertyManager::PropertyManager& rManager, Math::Int3 location, Tile tile, bool shouldOverride = true) override;
		virtual void Erase(TilePropertyManager::PropertyManager& rManager, Math::Int3 location) override;
		virtual bool Contains(Math::Vector2Int position) override;

		virtual void Iterate(std::function<bool(Math::Vector2Int, Tile&)> func) override;

		// Counts tiles as collisions when they are contained in this registry
		virtual bool DetectCollisionRect(Math::Vector2Int bottomLeft, Math::Vector2Int topRight) override;
	};
}