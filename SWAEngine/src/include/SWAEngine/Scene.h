#pragma once
#include "SWAEngine/dllClause.h"
#include "GameObject/BaseGameObject.h"
#include <unordered_map>

namespace SWAEngine
{
	// TODO: Could create a hierarchy for objects very similar to the GP2D::GUI::Hierarchy
	// Should be created using SceneManager::CreateScene("MyName")
	struct SWA_ENGINE_API Scene
	{
	private:
		std::unordered_map<const char*, GameObject::BaseGameObject*> m_ObjectsRegistry;

		// The name of the scene
		const char* m_Name;
	public:

		// Creates a new scene
		Scene(const char* sceneName);
		const char* const GetName();

		// TODO: Could make a way to suppress(free memory) a scene when its deactivated to clear up memory
		// Sets the state of all contained objects
		void SetActive();
		bool GetActive();

		GameObject::BaseGameObject* const GetObject(const char* name);
		bool RegisterObject(GameObject::BaseGameObject* pObj);

		// Updates all registered objects
		void UpdateObjects(Math::Time time);
		// Releases the object (or all registered objects if nullptr)
		void ReleaseObject(const char* name = nullptr);
	};
}