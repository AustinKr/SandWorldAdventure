#pragma once
#include "dllClause.h"
#include "Math/vector2.h"
#include "EventHandler.h"

namespace SWAEngine
{
	struct SWA_ENGINE_API Transform 
	{
	private:
		Math::Vector2 m_Position;
		Math::Vector2 m_Scale;

	public:
		EventHandler<Math::Vector2> OnSetPosition;
		EventHandler<Math::Vector2> OnSetScale;
		
		Transform();
		Transform(Math::Vector2 position, Math::Vector2 scale);

		Math::Vector2 GetPosition() const;
		void SetPosition(Math::Vector2 newPosition);
		Math::Vector2 GetScale() const;
		void SetScale(Math::Vector2 newScale);
	};
}