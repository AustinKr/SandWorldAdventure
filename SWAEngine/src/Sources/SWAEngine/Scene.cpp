#include "SWAEngine/Scene.h"
#include "SWAEngine/SceneManager.h"

namespace SWAEngine
{
	Scene::Scene(const char* sceneName) : m_Name(sceneName)
	{
	}
	const char* const Scene::GetName()
	{
		return m_Name;
	}
	void Scene::SetActive()
	{
		bool isActive = GetActive();
		for (auto& obj : m_ObjectsRegistry)
		{
			obj.second->SetActive(isActive);
		}
	}
	bool Scene::GetActive()
	{
		return m_Name == SceneManager::GetScene().GetName();
	}

	BaseGameObject* const Scene::GetObject(const char* name)
	{
		if (!m_ObjectsRegistry.contains(name))
			return nullptr;

		return m_ObjectsRegistry.at(name);
	}
	bool Scene::RegisterObject(BaseGameObject* pObj)
	{
		return m_ObjectsRegistry.insert(std::make_pair(pObj->GetName(), pObj)).second;
	}


	void Scene::UpdateObjects(Time time)
	{
		for (auto& obj : m_ObjectsRegistry)
		{
			obj.second->Update(time);
		}
	}
	void Scene::ReleaseObject(const char* name)
	{
		// Try release all
		if (name == nullptr)
		{
			for (auto& obj : m_ObjectsRegistry)
			{
				obj.second->Release();
			}
			m_ObjectsRegistry.clear();
			return;
		}

		// Try release given object
		if (!m_ObjectsRegistry.contains(name))
			return;
		m_ObjectsRegistry.at(name)->Release();
		m_ObjectsRegistry.erase(name);
	}
}