#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Time.h"

namespace SWAEngine
{
	// Should be created by new()
	// A physical object inside of the game that requires functionality every frame
	// (by default is an empty object that may be used to group other objects)
	struct SWA_ENGINE_API BaseGameObject
	{
	private:
		const char* m_Name; // TODO: can make name changeable
		bool m_IsActive;

	public:
		const char* const GetName();
		bool GetActive();

		// Registers self (with the active scene if not specified)
		BaseGameObject(const char* objName, const char* sceneName = nullptr);

		// Nothing by default
		virtual void Update(Time);
		// Release this by default
		virtual void Release();
		// Sets active state by default
		virtual void SetActive(bool state);
	};
}