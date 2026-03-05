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
		virtual bool Contains(Math::Vector2Int position) override;
		virtual Tile& Get(Math::Vector2Int position) override;
		virtual Tile& Set(Math::Vector2Int position, Tile tile, bool shouldOverride = true) override;
		virtual void Erase(Math::Vector2Int position) override;
		virtual void Iterate(std::function<bool(Math::Vector2Int, Tile&)> func) override;
		virtual void Clear() override;
	};
}