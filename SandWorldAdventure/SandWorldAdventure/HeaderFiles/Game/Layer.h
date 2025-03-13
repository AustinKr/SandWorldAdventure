#pragma once
#include "HeaderFiles/Game/GameObjects/IGameObject.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace SandboxEngine::Game
{
	class Layer 
	{
	public:
		// List of pointers to objects allocated on the heap
		std::unordered_map<std::string, GameObject::IGameObject*> Objects;
		std::string Name;

		inline Layer(std::string name)
		{
			Objects = {};
			Name = name;
		}
	};
}