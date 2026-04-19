#pragma once
#include <SWAEngine/Math/Time.h>
#include <SWAEngine/dllClause.h>

namespace SWAEngine::GameObject::Component
{
	struct IComponent
	{
		virtual void Update(Math::Time) = 0;
		virtual void Release() = 0;
	};
}