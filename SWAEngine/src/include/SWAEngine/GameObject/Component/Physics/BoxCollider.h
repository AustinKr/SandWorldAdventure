#pragma once
#include "Collider.h"
#include <SWAEngine/GameObject/Component/Transform.h>

#include <SWAEngine/GameObject/GameObject.h>

namespace SWAEngine::GameObject::Component::Physics
{
	// Requires a Transform component
	struct SWA_ENGINE_API BoxCollider : Collider
	{
	public:
		virtual ColliderTypes GetType() override;
		static std::string const GetName();
		virtual void Release() override;

		virtual bool IsColliding(Collider* other, int tag = SELECT_ALL) override;

		static bool DetectAABB(Math::Vector2 a_leftBottom, Math::Vector2 a_rightTop, Math::Vector2 b_leftBottom, Math::Vector2 b_rightTop);
		static bool DetectAABB(Transform& a, Transform& b);
	};
}