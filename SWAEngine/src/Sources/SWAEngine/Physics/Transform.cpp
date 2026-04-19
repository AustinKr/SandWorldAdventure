#include <SWAEngine/Math/Transform.h>

namespace SWAEngine::Math
{
	Transform::Transform() : m_Position{}, m_Scale{}, OnSetPosition{}, OnSetScale{}
	{}
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
}