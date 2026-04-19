#pragma once
#include <SWAEngine/dllClause.h>
#include <set>

#include "ColliderTypes.h"

namespace SWAEngine::Physics
{
	// TODO: For collision optimisation: use an acceleration structure like a Quadtree
	struct SWA_ENGINE_API Collider
	{
	protected:
		static std::set<Collider*> ms_CollidersRegistry;

		Collider() = default;
	public:
		static const int SELECT_ALL = 0;

		virtual ColliderTypes GetType() = 0;
		/// <summary/>
		/// <param name="other">The other collider</param>
		/// <param name="tag">Used to select colliders with one of the given tags to test collision</param>
		virtual bool IsColliding(Collider* other, int tag = SELECT_ALL) = 0;

		/// <summary> Checks collision with all registered colliders with one of the given tags </summary>
		/// <param name="tag">Used to select colliders with one of the given tags to test collision</param>
		bool IsColliding(int tag = SELECT_ALL);

		// Deletes memory and removes from internal registry
		void Release();
		static void ReleaseAll(); // TODO: Call this inside game somewhere
	};
}
