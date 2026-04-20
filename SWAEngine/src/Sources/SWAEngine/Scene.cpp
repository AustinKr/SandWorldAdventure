#include "SWAEngine/Scene.h"
#include "SWAEngine/SceneManager.h"

namespace SWAEngine
{
	Scene::Scene(std::string sceneName) : m_Name(sceneName)
	{
	}
	std::string const Scene::GetName()
	{
		return m_Name;
	}
	void Scene::SetActive()
	{
		bool isActive = GetActive();
		for (auto& obj : m_ObjectsRegistry)
		{
			obj.second.SetActive(isActive);
		}
	}
	bool Scene::GetActive()
	{
		return m_Name == SceneManager::GetScene().GetName();
	}

	bool Scene::ContainsObject(std::string name)
	{
		return m_ObjectsRegistry.contains(name);
	}
	GameObject::GameObject& Scene::GetObject(std::string name)
	{
		return m_ObjectsRegistry.at(name);
	}
	bool Scene::RegisterObject(GameObject::GameObject& obj)
	{
		obj.SetActive(GetActive());
		return m_ObjectsRegistry.insert(std::make_pair(obj.GetName(), obj)).second;
	}

	void Scene::UpdateObjects(Math::Time time)
	{
		for (auto& obj : m_ObjectsRegistry)
		{
			obj.second.Update(time);
		}
	}
	void Scene::TryReleaseObject(const char* name)
	{
		// Try release all
		if (name == nullptr)
		{
			for (auto& obj : m_ObjectsRegistry)
			{
				obj.second.Release();
			}
			m_ObjectsRegistry.clear();
			return;
		}

		// Try release given object
		if (!ContainsObject(name))
			return;
		GetObject(name).Release();
		m_ObjectsRegistry.erase(name);
	}
}