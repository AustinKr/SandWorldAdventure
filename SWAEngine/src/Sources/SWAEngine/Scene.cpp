#include <SWAEngine/Scene.h>
#include <SWAEngine/SceneManager.h>
#include <SWAEngine/GameObject/GameObject.h>

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
	GameObject::GameObject& Scene::GetGameObject(std::string name)
	{
		return m_ObjectsRegistry.at(name);
	}
	GameObject::GameObject& Scene::CreateGameObject(std::string name)
	{
		GameObject::GameObject obj(name);
		obj.SetActive(GetActive());
		return m_ObjectsRegistry.insert(std::make_pair(name, std::move(obj))).first->second;
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
		GetGameObject(name).Release();
		m_ObjectsRegistry.erase(name);
	}
}