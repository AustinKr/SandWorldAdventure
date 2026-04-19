#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Scene.h"
#include <unordered_map>
#include <string>

namespace SWAEngine
{
	struct SWA_ENGINE_API SceneManager
	{
	private:
		static std::unordered_map<std::string, Scene> ms_ScenesRegistry;
		static std::string ms_ActiveScene;

	public:
		// Creates and registers the scene, and if there is no active scene assigns it
		static bool CreateScene(std::string name);
		// Releases the scene (or all in nullptr)
		static void ReleaseScene(std::string name = nullptr);
		// Gets the scene (or active if nullptr)
		static Scene& GetScene(std::string name = nullptr);

		static void SetActiveScene(std::string name);
	};
}