#include <SWAEngine/Component/Physics/BoxCollider.h>
#include <SWAEngine/Component/ComponentTags.h>

namespace SWAEngine::Component::Physics
{
	unsigned int const BoxCollider::GetTag()
	{
		return COMP_TAG_COLLIDER | COMP_TAG_BOX_COLLIDER;
	}

	bool BoxCollider::IsColliding(Collider* other)
	{
		unsigned int tag = other->GetTag();
		if (tag & COMP_TAG_BOX_COLLIDER)
			return DetectAABB(*p_LinkedTransform, *static_cast<BoxCollider*>(other)->p_LinkedTransform);

		// Handle default
		bool isOtherColliding = other->IsColliding(this);

		// TODO: handle other. Could use test points to be semi-accurate with unknown colliders
		//if (!isOtherColliding)
		//{
		//	// try handle other
		//	//...
		//	return approximate;
		//}

		return isOtherColliding;
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