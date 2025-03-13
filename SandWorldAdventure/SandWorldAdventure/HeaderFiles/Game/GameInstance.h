#pragma once
#include "HeaderFiles\Render\Camera.h";
#include "Layer.h"
#include "HeaderFiles/Time.h"
#include <map>
#include <string>

namespace SandboxEngine::Game
{
	static class GameInstance
	{
	public:
		// TODO: Could replace with quadtree
		// Layers are composited from back to front (0 is forground, ..., 10 is background)
		static std::vector<Layer> Layers; 
		static Render::Camera MainCamera;
		static Time TimeInfo;

		// Adds pointer to a layer and creates a new layer if needed
		static void RegisterGameObject(int layer, std::string layerName, const GameObject::IGameObject* pGameObject, std::string objectName);
		// Adds pointer to a layer and creates a new layer if needed
		static void RegisterGameObject(std::vector<Layer>::iterator layerIterator, const GameObject::IGameObject* pGameObject, std::string objectName);

		static void UpdateObjects();
		// Takes the data from each layer, translates and scales that data, and then combines them to one composition.
		static void CompositeLayers();
		// Calls IGameObject::Release() on all objects
		static void Release();

		static void WhereAllObjects(void(*func)(std::vector<Layer>::reverse_iterator layerIter, std::unordered_map<std::string, GameObject::IGameObject*>::const_iterator objIter));
	};
}