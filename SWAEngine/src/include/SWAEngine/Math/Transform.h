#pragma once
#include <SWAEngine/dllClause.h>
#include <SWAEngine/EventHandler.h>
#include "vector2.h"

namespace SWAEngine::Math
{
	struct SWA_ENGINE_API Transform 
	{
	private:
		Vector2 m_Position;
		Vector2 m_Scale;

	public:
		EventHandler<Vector2> OnSetPosition;
		EventHandler<Vector2> OnSetScale;
		
		Transform();
		Transform(Vector2 position, Vector2 scale);

		Vector2 GetPosition() const;
		void SetPosition(Vector2 newPosition);
		Vector2 GetScale() const;
		void SetScale(Vector2 newScale);
	};
}