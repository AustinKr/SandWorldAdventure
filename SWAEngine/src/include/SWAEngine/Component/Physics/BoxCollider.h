#pragma once
#include "Collider.h"

namespace SWAEngine::Component::Physics
{
	// Requires a Transform component
	struct SWA_ENGINE_API BoxCollider : Collider
	{
	public:
		// Inherited/Required by IComponent
		
		static std::string const GetName();
		virtual bool IsColliding(Collider* other) override;

		// Inherited by Collider

		virtual ColliderTypes GetType() override;

		// Collision methods

		static bool DetectAABB(Math::Vector2 a_leftBottom, Math::Vector2 a_rightTop, Math::Vector2 b_leftBottom, Math::Vector2 b_rightTop);
		static bool DetectAABB(Transform& a, Transform& b);
	};
}