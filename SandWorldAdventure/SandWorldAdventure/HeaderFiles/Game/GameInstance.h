#pragma once
#include "HeaderFiles/Time.h"
#include <map>
#include <string>
#include <vector>
#include "HeaderFiles/Game/GameObjects/IGameObject.h"

#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::Game
{
	static class GameInstance
	{
	public:
		// TODO: Could replace with quadtree
		// TODO: Could replace with a hierarchy where objects are parented and group to other objects
		static std::vector<std::pair<std::string, GameObject::IGameObject*>> GameObjectsRegistry;
		// The graphics pipeline
		static GraphicsPipeline::GraphicsPipeline2D Pipeline;
		// Time
		static Time TimeInfo;

		// Adds the address of the object to the registry with a name
		static void RegisterGameObject(std::string objectName, const GameObject::IGameObject* pGameObject);
		
		static void UpdateObjects();
		// Calls IGameObject::Release() on all objects and delete(), and calls Pipline.Release()
		static void Release();

		static void WhereAllObjects(void(*func)(std::vector<std::pair<std::string, GameObject::IGameObject*>>::const_iterator objIter));
	};
}