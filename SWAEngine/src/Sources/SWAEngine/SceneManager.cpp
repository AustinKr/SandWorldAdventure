#include "SWAEngine/SceneManager.h"

namespace SWAEngine
{
	std::unordered_map<const char*, Scene> SceneManager::ms_ScenesRegistry = {};
	const char* SceneManager::ms_ActiveScene = nullptr;

	bool SceneManager::CreateScene(const char* name)
	{
		bool succeeded = ms_ScenesRegistry.insert(std::make_pair(name, Scene(name))).second;
		if (ms_ActiveScene == nullptr && succeeded)
			ms_ActiveScene = name;
		return succeeded;
	}
	void SceneManager::ReleaseScene(const char* name)
	{
		// Try release all
		if (name == nullptr)
		{
			for (auto& scene : ms_ScenesRegistry)
			{
				scene.second.ReleaseObject();
			}
			ms_ScenesRegistry.clear();
			return;
		}

		// Try release given
		if (!ms_ScenesRegistry.contains(name))
			return;
		ms_ScenesRegistry.at(name).ReleaseObject();
		ms_ScenesRegistry.erase(name);
	}
	Scene& SceneManager::GetScene(const char* name)
	{
		if (name == nullptr)
		{
			if (!ms_ScenesRegistry.contains(ms_ActiveScene))
				throw std::exception("No active scene assigned!");

			return ms_ScenesRegistry.at(ms_ActiveScene);
		}
		return ms_ScenesRegistry.at(name);
	}

	void SceneManager::SetActiveScene(const char* newScene)
	{
		const char* oldScene = ms_ActiveScene;

		if (oldScene == newScene || !ms_ScenesRegistry.contains(newScene))
			return;

		ms_ActiveScene = newScene;
		// Change scenes
		GetScene(oldScene).SetActive(); // Suppress
		ms_ScenesRegistry.at(newScene).SetActive(); // Unsuppress
	}
}