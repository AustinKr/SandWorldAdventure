#pragma once
#include "Collider.h"
#include "Rigidbody.h"
#include <SWAEngine/GameObject/Component/Transform.h>
#include <SWAEngine/GameObject/GameObject.h>

namespace SWAEngine::GameObject::Component::Physics
{
	// Requires a Transform component
	struct SWA_ENGINE_API BoxCollider : Collider
	{
	private:
		BoxCollider(Transform* const pTransform);

	public:
		Transform* const p_LinkedTransform;
		
		// Creates an instance of box collider. Automatically creates a transform component if needed
		static BoxCollider* const CreateCollider(GameObject& linkedObject);

		virtual ColliderTypes GetType() override;
		virtual std::string const GetName() override;
		virtual void Release() override;

		virtual bool IsColliding(Collider* other, int tag = SELECT_ALL) override;

		static bool DetectAABB(Math::Vector2 a_leftBottom, Math::Vector2 a_rightTop, Math::Vector2 b_leftBottom, Math::Vector2 b_rightTop);
		static bool DetectAABB(Transform& a, Transform& b);
	};
}