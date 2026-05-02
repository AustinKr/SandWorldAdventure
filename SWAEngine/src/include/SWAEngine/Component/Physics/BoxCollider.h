#pragma once
#include "Collider.h"

namespace SWAEngine::Component::Physics
{
	// Requires a Transform component
	struct SWA_ENGINE_API BoxCollider : Collider
	{
	public:
		// Inherited/Required by IComponent
		virtual unsigned int const GetTag() override;

		// Inherited by Collider
		virtual bool IsColliding(Collider* other) override;
		virtual bool IsColliding(Math::Vector2 point) override;

		// Collision methods

		bool ApproximateIsColliding(Collider* other); // Uses test points to be semi-accurate with unrecognized colliders
		static bool DetectAABB(Math::Vector2 a_leftBottom, Math::Vector2 a_rightTop, Math::Vector2 b_leftBottom, Math::Vector2 b_rightTop);
		static bool DetectAABB(Transform& a, Transform& b);
	};
}