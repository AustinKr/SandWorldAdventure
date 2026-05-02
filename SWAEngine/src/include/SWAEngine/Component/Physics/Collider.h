#pragma once
#include <SWAEngine/dllClause.h>
#include <SWAEngine/Component/IComponent.h>
#include <SWAEngine/Component/Transform.h>
#include <set>

namespace SWAEngine::Component::Physics
{
	// TODO: For collision optimisation: use an acceleration structure like a Quadtree
	// By default an empty Collider. Should not be instantiated because it contains no real functionality.
	struct SWA_ENGINE_API Collider : IComponent
	{
	protected:
		static std::set<Collider*> ms_CollidersRegistry;

	public:
		static const int SELECT_ALL = 0;

		Transform* p_LinkedTransform;

		// Inherited by IComponent

		virtual unsigned int const GetTag() override;
		virtual void Initialize(std::string objName) override; // Registers with internal registry and links transform by default
		virtual void Release() override; // Deletes memory and removes from internal registry by default
		virtual void Update(std::string, Math::Time) override {} // Nothing by default
		virtual void SetActive(bool state) override {}; // Nothing by default
		virtual bool GetActive() override { return true; } // Returns true by default

		// Member methods
		 // Returns false by default
		virtual bool IsColliding(Collider* other);
		/// <summary> Checks collision with all registered Colliders with one of the given Tags </summary>
		/// <param name="tag">Used to select Colliders with one of the given Tags to test collision</param>
		virtual bool IsColliding(unsigned int tag = SELECT_ALL);
	};
}
