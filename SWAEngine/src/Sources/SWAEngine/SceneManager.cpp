#include "SWAEngine/SceneManager.h"
#include <stdexcept>

namespace SWAEngine
{
	std::unordered_map<std::string, Scene> SceneManager::ms_ScenesRegistry = {};
	std::string SceneManager::ms_ActiveScene = "\0";

	bool SceneManager::CreateScene(std::string name)
	{
		bool succeeded = ms_ScenesRegistry.insert(std::make_pair(name, Scene(name))).second;
		if (ms_ActiveScene.empty() && succeeded)
			ms_ActiveScene = name;
		return succeeded;
	}
	void SceneManager::ReleaseScene(std::string name)
	{
		// Try release all
		if (name.empty())
		{
			for (auto& scene : ms_ScenesRegistry)
			{
				scene.second.TryReleaseObject();
			}
			ms_ScenesRegistry.clear();
			return;
		}

		// Try release given
		if (!ms_ScenesRegistry.contains(name))
			return;
		ms_ScenesRegistry.at(name).TryReleaseObject();
		ms_ScenesRegistry.erase(name);
	}
	Scene& SceneManager::GetScene(std::string name)
	{
		if (name.empty())
		{
			if (!ms_ScenesRegistry.contains(ms_ActiveScene))
				throw std::runtime_error("No active scene assigned!");

			return ms_ScenesRegistry.at(ms_ActiveScene);
		}
		return ms_ScenesRegistry.at(name);
	}

	void SceneManager::SetActiveScene(std::string newScene)
	{
		std::string oldScene = ms_ActiveScene;

		if (oldScene == newScene || !ms_ScenesRegistry.contains(newScene))
			return;

		ms_ActiveScene = newScene;
		// Change scenes
		GetScene(oldScene).SetActive(); // Suppress
		ms_ScenesRegistry.at(newScene).SetActive(); // Unsuppress
	}
}