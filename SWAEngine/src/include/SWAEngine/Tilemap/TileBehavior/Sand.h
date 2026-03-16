#pragma once
#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"
#include "SWAEngine/Math/vector2.h"
#include <unordered_map>

namespace SWAEngine::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API Sand : IBehavior
	{
	private:
		std::unordered_map<Math::Vector2Int, Math::Vector2, Math::Vector2Hasher> m_TileVelocities;

	public:
		Sand();

		virtual void OnCreate(Tile tile, Math::Vector2Int pos) override;
		virtual void OnRemove(Tile tile, Math::Vector2Int pos) override;

		virtual void Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time) override;
	};
}