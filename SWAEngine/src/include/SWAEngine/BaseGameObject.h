#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Time.h"
#include <unordered_map>

namespace SWAEngine
{
	// TODO: Could create a hierarchy for objects very similar to the GP2D::GUI::Hierarchy
	// Should be created by new()
	// A physical object inside of the game that requires functionality every frame
	// (by default is an empty object that may be used to group other objects)
	struct SWA_ENGINE_API BaseGameObject
	{
	private:
		static std::unordered_map<const char*, BaseGameObject*> ms_Registry;
		const char* m_Name;
	public:
		static BaseGameObject* const GetObject(const char* name);

		// Updates all registered objects
		static void UpdateObjects(Time time);
		// Releases all registered objects
		static void ReleaseObjects();

		const char* const GetName();

		// Registers self
		BaseGameObject(const char* name);

		// Nothing by default
		virtual void Update(Time);
		// Release this by default
		virtual void Release();
	};
}