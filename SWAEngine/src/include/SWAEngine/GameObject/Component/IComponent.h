#pragma once
#include <SWAEngine/Math/Time.h>
#include <SWAEngine/dllClause.h>
#include <string>

namespace SWAEngine::GameObject::Component
{
	struct IComponent
	{
		// The alias used for this component. 
		// Note: GameObjects can only have one of each Component
		virtual std::string const GetName() = 0; // TODO: Maybe make component names static
		// Used for physics and other
		virtual void Update(std::string objectName, Math::Time) = 0;
		// Release this component and all data created within it
		virtual void Release() = 0;
	};
}