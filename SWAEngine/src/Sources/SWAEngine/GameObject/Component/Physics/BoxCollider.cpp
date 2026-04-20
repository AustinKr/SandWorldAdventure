#include <SWAEngine/GameObject/Component/Physics/BoxCollider.h>

namespace SWAEngine::GameObject::Component::Physics
{
	BoxCollider::BoxCollider(Transform* const pTransform) : Collider(pTransform) {}

	BoxCollider* const BoxCollider::CreateCollider(GameObject& linkedObject)
	{
		// Get transform
		Transform* pTransform = linkedObject.TryGetComponent<Transform>("transform");
		if (pTransform == nullptr)
			pTransform = linkedObject.RegisterComponent(new Transform());

		// Create
		BoxCollider* const newObject = new BoxCollider(pTransform);
		ms_CollidersRegistry.insert(newObject);

		// Return
		return newObject;
	}

	ColliderTypes BoxCollider::GetType()
	{
		return COLLIDER_TYPE_BOX;
	}
	std::string const BoxCollider::GetName()
	{
		return "box_collider";
	}
	void BoxCollider::Release()
	{
		delete(this);
	}

	bool BoxCollider::IsColliding(Collider* other, int tag)
	{
		switch (other->GetType())
		{
		case COLLIDER_TYPE_BOX:
			return DetectAABB(*p_LinkedTransform, *static_cast<BoxCollider*>(other)->p_LinkedTransform);

		default:
			// TODO: handle other. Could use test points to be semi-accurate with unknown colliders
			break;
		}
		
		return false;
	}

	bool BoxCollider::DetectAABB(Math::Vector2 a_leftBottom, Math::Vector2 a_rightTop, Math::Vector2 b_leftBottom, Math::Vector2 b_rightTop)
	{
		return
			a_leftBottom.X < b_rightTop.X && a_rightTop.X > b_leftBottom.X &&
			a_leftBottom.Y < b_rightTop.Y && a_rightTop.Y > b_leftBottom.Y;
	}
	bool BoxCollider::DetectAABB(Transform& a, Transform& b)
	{
		return DetectAABB(
			a.GetPosition(), a.GetPosition() + a.GetScale(),
			b.GetPosition(), b.GetPosition() + b.GetScale());
	}
}