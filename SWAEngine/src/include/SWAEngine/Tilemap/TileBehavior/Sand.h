#pragma once
#include "SWAEngine/Tilemap/TileBehavior/IBehavior.h"
#include "SWAEngine/Math/vector2.h"
#include <unordered_map>

namespace SWAEngine::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API Sand : IBehavior
	{
	public:
		Sand();

		virtual void OnCreate(Tile tile, Math::Vector2Int pos) override;
		virtual void OnRemove(Tile tile, Math::Vector2Int pos) override;
		// Carries over the velocity
		virtual void OnClone(Tile original, Math::Vector2Int originalPos, Math::Vector2Int newPos) override;

		virtual void Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time) override;

	private:
		//std::unordered_map<Math::Vector2Int, Math::Vector2, Math::Vector2Hasher> m_ActiveTileVelocities;
		//std::unordered_map<Math::Vector2Int, Math::Vector2, Math::Vector2Hasher> m_ClonedTileVelocities; // Velocities to be carried over to a moved tile

		void TryMove(Math::Vector2 velocity, Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap);
	};
}