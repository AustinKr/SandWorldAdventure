#pragma once
#include "SWAEngine/Tilemap/ITilemapContainer.h"
#include <unordered_map>

namespace SWAEngine::Tilemap
{
	struct SWA_ENGINE_API TilemapContainer : public ITilemapContainer
	{
	private:
		std::unordered_map<Vector2Int, Tile, Vector2Hasher, Vector2Compare> m_TilesRegistry;

	public:
		virtual bool Contains(Vector2Int position) override;
		virtual Tile& Get(Vector2Int position) override;
		virtual Tile& Set(Vector2Int position, Tile tile, bool shouldOverride = true) override;
		virtual void Erase(Vector2Int position) override;
		virtual void Iterate(std::function<bool(Vector2Int, Tile&)> func) override;
	};
}