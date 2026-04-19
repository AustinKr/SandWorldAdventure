#pragma once
#include <SWAEngine/Math/Time.h>
#include <SWAEngine/dllClause.h>
#include <string>

namespace SWAEngine::GameObject::Component
{
	struct IComponent
	{
		virtual std::string const GetName() = 0;
		virtual void Update(Math::Time) = 0;
		virtual void Release() = 0;
	};
}