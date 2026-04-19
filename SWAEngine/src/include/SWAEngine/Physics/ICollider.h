#pragma once
#include <SWAEngine/dllClause.h>
#include <set>

#include "ColliderTypes.h"

namespace SWAEngine::Physics
{
	struct SWA_ENGINE_API ICollider
	{
	protected:
		static std::set<ICollider*> ms_CollidersRegistry;

	public:
		static const int SELECT_ALL = 0;

		virtual ColliderTypes GetType() = 0;
		
		/// <summary/>
		/// <param name="other">The other collider</param>
		/// <param name="tag">Used to select colliders with one of the given tags to test collision</param>
		virtual bool IsColliding(ICollider* other, int tag = SELECT_ALL) = 0;

		/// <summary> Checks collision with all registered colliders with one of the given tags </summary>
		/// <param name="tag">Used to select colliders with one of the given tags to test collision</param>
		virtual bool IsColliding(int tag = SELECT_ALL) = 0;

		// Deletes memory and removes from internal registry
		void Release();
	};
}
