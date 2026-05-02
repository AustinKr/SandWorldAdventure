#include <SWAEngine/Component/Transform.h>
#include <SWAEngine/Component/ComponentTags.h>

namespace SWAEngine::Component
{
	Transform::Transform(Math::Vector2 position, Math::Vector2 scale) : m_Position(position), m_Scale(scale), OnSetPosition{}, OnSetScale{}
	{}

	Math::Vector2 Transform::GetPosition() const
	{
		return m_Position;
	}
	void Transform::SetPosition(Math::Vector2 newPosition)
	{
		OnSetPosition.InvokeEvents(newPosition);
		m_Position = newPosition;
	}
	Math::Vector2 Transform::GetScale() const
	{
		return m_Scale;
	}
	void Transform::SetScale(Math::Vector2 newScale)
	{
		OnSetScale.InvokeEvents(newScale);
		m_Scale = newScale;
	}

	unsigned int const Transform::GetTag()
	{
		return COMP_TAG_TRANSFORM;
	}
	void Transform::Initialize(std::string objName)
	{}
	void Transform::Release()
	{
		delete(this);
	}
	void Transform::Update(std::string, Math::Time)
	{
		/*nothing*/
	}
	void Transform::SetActive(bool state) {}
	bool Transform::GetActive() { return true; }
}