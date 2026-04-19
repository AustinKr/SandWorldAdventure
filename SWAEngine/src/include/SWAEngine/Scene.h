#pragma once
#include "SWAEngine/dllClause.h"
#include "GameObject/GameObject.h"
#include <unordered_map>
#include <string>

namespace SWAEngine
{
	// TODO: Could create a hierarchy for objects very similar to the GP2D::GUI::Hierarchy
	// Should be created using SceneManager::CreateScene("MyName")
	struct SWA_ENGINE_API Scene
	{
	private:
		std::unordered_map<std::string, GameObject::GameObject> m_ObjectsRegistry;

		// The name of the scene
		std::string m_Name;
	public:

		// Creates a new scene
		Scene(std::string sceneName);
		std::string const GetName();

		// TODO: Could make a way to suppress(free memory) a scene when its deactivated to clear up memory
		// Sets the state of all contained objects
		void SetActive();
		bool GetActive();

		bool ContainsObject(std::string name);
		GameObject::GameObject& GetObject(std::string name);
		// Registers the object with this scene and sets active
		bool RegisterObject(GameObject::GameObject& obj);

		// Updates all registered objects
		void UpdateObjects(Math::Time time);
		// Releases the object (or all registered objects if nullptr)
		void TryReleaseObject(const char* name = nullptr);
	};
}