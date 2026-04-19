#pragma once
#include "Collider.h"
#include "IPhysicsObject.h"
#include <SWAEngine/Math/Transform.h>

namespace SWAEngine::Physics
{
	struct SWA_ENGINE_API BoxCollider : Collider
	{
	private:
		BoxCollider() = default;

	public:
		// Position is the origin, scale is the total width and height
		Math::Transform Coordinates;
		
		// Creates an instance of box collider
		static BoxCollider* const CreateCollider();
		// Creates an instance of box collider
		static BoxCollider* const CreateCollider(IPhysicsObject* const pLinkedObj);

		virtual ColliderTypes GetType() override;
		virtual bool IsColliding(Collider* other, int tag = SELECT_ALL) override;

		static bool DetectAABB(Math::Vector2 a_leftBottom, Math::Vector2 a_rightTop, Math::Vector2 b_leftBottom, Math::Vector2 b_rightTop);
		static bool DetectAABB(Math::Transform a, Math::Transform b);
	};
}