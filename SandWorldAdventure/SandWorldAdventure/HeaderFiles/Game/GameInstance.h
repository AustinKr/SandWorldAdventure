#pragma once
#include "HeaderFiles/Time.h"
#include <map>
#include <string>
#include <vector>
#include "HeaderFiles/Game/GameObjects/IGameObject.h"

namespace SandboxEngine::Game
{
	static class GameInstance
	{
	public:
		// TODO: Could replace with quadtree
		// TODO: Could replace with a hierarchy where objects are parented and group to other objects
		static std::vector<std::pair<std::string, GameObject::IGameObject*>> GameObjectsRegistry;

		static Time TimeInfo;

		// Adds the address of the object to the registry with a name
		static void RegisterGameObject(std::string objectName, const GameObject::IGameObject* pGameObject);
		
		static void UpdateObjects();
		// Calls IGameObject::Release() on all objects
		static void Release();

		static void WhereAllObjects(void(*func)(std::vector<std::pair<std::string, GameObject::IGameObject*>>::const_iterator objIter));
	};
}