#include <SWAEngine/GameObject/Component/Physics/BoxCollider.h>

namespace SWAEngine::GameObject::Component::Physics
{
	BoxCollider* const BoxCollider::CreateCollider()
	{
		BoxCollider* newObject = new BoxCollider();
		ms_CollidersRegistry.insert(newObject);

		return newObject;
	}
	BoxCollider* const BoxCollider::CreateCollider(Rigidbody* const pLinkedObj)
	{
		BoxCollider* const pObj = CreateCollider();
		pLinkedObj->Coordinates.OnSetPosition += [=](auto pos) {pObj->Coordinates.SetPosition(pos); };
		pLinkedObj->Coordinates.OnSetScale += [=](auto scale) {pObj->Coordinates.SetScale(scale); };
		return pObj;
	}

	ColliderTypes BoxCollider::GetType()
	{
		return COLLIDER_TYPE_BOX;
	}
	bool BoxCollider::IsColliding(Collider* other, int tag)
	{
		switch (other->GetType())
		{
		case COLLIDER_TYPE_BOX:
			return DetectAABB(Coordinates, static_cast<BoxCollider*>(other)->Coordinates);

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
	bool BoxCollider::DetectAABB(Math::Transform a, Math::Transform b)
	{
		return DetectAABB(
			a.GetPosition(), a.GetPosition() + a.GetScale(),
			b.GetPosition(), b.GetPosition() + b.GetScale());
	}
}