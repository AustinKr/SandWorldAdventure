#pragma once
#include "SWAEngine/dllClause.h"
#include "SWAEngine/Scene.h"
#include <unordered_map>

namespace SWAEngine
{
	struct SWA_ENGINE_API SceneManager
	{
	private:
		static std::unordered_map<const char*, Scene> ms_ScenesRegistry;
		static const char* ms_ActiveScene;

	public:
		// Creates and registers the scene, and if there is no active scene assigns it
		static bool CreateScene(const char* name);
		// Releases the scene (or all in nullptr)
		static void ReleaseScene(const char* name = nullptr); 
		// Gets the scene (or active if nullptr)
		static Scene& GetScene(const char* name = nullptr);

		static void SetActiveScene(const char* name);
	};
}