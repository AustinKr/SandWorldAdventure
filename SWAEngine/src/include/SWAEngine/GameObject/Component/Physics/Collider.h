#pragma once
#include <SWAEngine/dllClause.h>
#include <SWAEngine/GameObject/Component/IComponent.h>
#include <SWAEngine/GameObject/Component/Transform.h>
#include <set>

#include "ColliderTypes.h"

namespace SWAEngine::GameObject::Component::Physics
{
	// TODO: For collision optimisation: use an acceleration structure like a Quadtree
	// Derivatives of this struct should have a factory static function to create an instance
	struct SWA_ENGINE_API Collider : IComponent
	{
	protected:
		static std::set<Collider*> ms_CollidersRegistry;

	public:
		static const int SELECT_ALL = 0;

		Transform* p_LinkedTransform;

		virtual ColliderTypes GetType() = 0; 

		virtual void Initialize(std::string objName) override; // Registers with internal registry and links transform by default
		virtual void Release() override; // Deletes memory and removes from internal registry by default
		virtual void Update(std::string, Math::Time) override {} // Nothing by default
		virtual void SetActive(bool state) override {}; // Nothing by default
		virtual bool GetActive() override { return true; } // Returns true by default

		/// <summary/>
		/// <param name="other">The other collider</param>
		/// <param name="tag">Used to select colliders with one of the given tags to test collision</param>
		virtual bool IsColliding(Collider* other, int tag = SELECT_ALL) = 0;

		/// <summary> Checks collision with all registered colliders with one of the given tags </summary>
		/// <param name="tag">Used to select colliders with one of the given tags to test collision</param>
		virtual bool IsColliding(int tag = SELECT_ALL);
	};
}
