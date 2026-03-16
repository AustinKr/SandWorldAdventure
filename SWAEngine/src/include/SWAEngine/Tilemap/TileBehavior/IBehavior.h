#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Math/vector2.h"
#include "SWAEngine/Time.h"
#include "SWAEngine/Tilemap/Tilemap.h"
#include <unordered_map>

namespace SWAEngine::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API IBehavior
	{
		static std::unordered_map<unsigned int, IBehavior*> s_Behaviors;

		virtual void OnCreate(Tile tile, Math::Vector2Int pos) = 0;
		virtual void OnRemove(Tile tile, Math::Vector2Int pos) = 0;

		virtual void Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Time time) = 0;
	};
}