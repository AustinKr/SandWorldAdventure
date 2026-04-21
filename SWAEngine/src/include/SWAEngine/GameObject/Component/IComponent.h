#pragma once
#include <SWAEngine/Math/Time.h>
#include <SWAEngine/dllClause.h>
#include <string>

namespace SWAEngine::GameObject::Component
{
	// User should create Components through GameObject
	struct SWA_ENGINE_API IComponent
	{
		// The alias used for this component. 
		// Note: GameObjects can only have one of each Component
		virtual std::string const GetName() = 0; // TODO: Maybe make component names static

		virtual void Initialize(std::string objName) = 0; // Called immediately after creating an instance
		virtual void Release() = 0; // Release this component and all data created within it
		virtual void Update(std::string objectName, Math::Time) = 0; // Used for physics and other
		virtual void SetActive(bool state) = 0;
		virtual bool GetActive() = 0;
	};
}