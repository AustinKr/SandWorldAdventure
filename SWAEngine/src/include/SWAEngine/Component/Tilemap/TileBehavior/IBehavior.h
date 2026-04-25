#pragma once
#include <SWAEngine/dllClause.h>
#include <SWAEngine/Math/vector2.h>
#include <SWAEngine/Math/Time.h>
#include <SWAEngine/Component/Tilemap/Tilemap.h>
#include <unordered_map>

namespace SWAEngine::Component::Tilemap::TileBehavior
{
	struct SWA_ENGINE_API IBehavior
	{
		static std::unordered_map<unsigned int, IBehavior*> s_Behaviors;

		virtual void Update(Tile tile, Math::Vector2Int pos, Tilemap* const pTilemap, Math::Time time) = 0;
		
		// Allocates property data in new tile. Caller is responsible for releasing it
		virtual Tile CreateNew(Math::Time time) = 0;
	};
}