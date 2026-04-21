#pragma once
#include "SWAEngine/dllClause.h"
#include "Math/Time.h"
#include <unordered_map>
#include <string>

namespace SWAEngine
{
#define SCENE_H
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
	namespace GameObject
	{
		class SWA_ENGINE_API GameObject;
	}
#endif

	// TODO: Could create a hierarchy for objects very similar to the GP2D::GUI::Hierarchy
	// Should be created using SceneManager::CreateScene("MyName")
	class SWA_ENGINE_API Scene
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
		GameObject::GameObject& GetGameObject(std::string name); // TODO: Make static function to get object in active scene
		// Registers the new object with this scene and sets active
		GameObject::GameObject& CreateGameObject(std::string name);

		// Updates all registered objects
		void UpdateObjects(Math::Time time);
		// Releases the object (or all registered objects if nullptr)
		void TryReleaseObject(const char* name = nullptr);
	};
}