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
		// Try detect AABB collision
		unsigned int tag = other->GetTag();
		if (tag & COMP_TAG_BOX_COLLIDER)
			return DetectAABB(*p_LinkedTransform, *static_cast<BoxCollider*>(other)->p_LinkedTransform);

		// Allow other to detect collision
		if (other->IsColliding(this))
			return true;

		// Default
		return ApproximateIsColliding(other);
	}
	bool BoxCollider::IsColliding(Math::Vector2 point)
	{
		Math::Vector2
			bottomLeft = p_LinkedTransform->GetPosition(),
			topRight = bottomLeft + p_LinkedTransform->GetScale();

		return point.X >= bottomLeft.X && point.X < topRight.X &&
			point.Y >= bottomLeft.Y && point.Y < topRight.Y;
	}

	bool BoxCollider::ApproximateIsColliding(Collider* other)
	{
		Math::Vector2
			bottomLeft = p_LinkedTransform->GetPosition(),
			topRight = bottomLeft + p_LinkedTransform->GetScale(),
			bottomRight = { topRight.X, bottomLeft.Y },
			topLeft = { bottomLeft.X, topRight.Y };

		return other->IsColliding(bottomLeft) || other->IsColliding(topRight) || other->IsColliding(bottomRight) || other->IsColliding(topLeft);
	}

	bool BoxCollider::DetectAABB(Math::Vector2 a_leftBottom, Math::Vector2 a_rightTop, Math::Vector2 b_leftBottom, Math::Vector2 b_rightTop)
	{
		return
			a_leftBottom.X < b_rightTop.X && a_rightTop.X >= b_leftBottom.X &&
			a_leftBottom.Y < b_rightTop.Y && a_rightTop.Y >= b_leftBottom.Y;
	}
	bool BoxCollider::DetectAABB(Transform& a, Transform& b)
	{
		return DetectAABB(
			a.GetPosition(), a.GetPosition() + a.GetScale(),
			b.GetPosition(), b.GetPosition() + b.GetScale());
	}
}