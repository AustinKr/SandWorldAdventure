#pragma once
#include <SWAEngine/Math/Time.h>
#include <SWAEngine/dllClause.h>
#include <string>

namespace SWAEngine::Component
{
	
	// User should create Components through GameObject
	// Notes: There should be a static method std::string GetName();
	struct SWA_ENGINE_API IComponent
	{
		virtual void Initialize(std::string objName) = 0; // Called immediately after creating an instance
		virtual void Release() = 0; // Release this component and all data created within it
		virtual void Update(std::string objectName, Math::Time) = 0; // Used for physics and other
		virtual void SetActive(bool state) = 0;
		virtual bool GetActive() = 0;
	};
}