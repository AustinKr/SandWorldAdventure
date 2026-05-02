#include <SWAEngine/GameObject.h>
#include <SWAEngine/SceneManager.h>

namespace SWAEngine
{
	std::string const GameObject::GetName()
	{
		return m_Name;
	}

	GameObject::GameObject(std::string name) : m_Name(name), m_IsActive(false)
	{}

	void GameObject::Update(Math::Time time)
	{
		for (auto& pair : m_Components)
		{
			pair.second->Update(GetName(), time);
		}
	}
	void GameObject::Release()
	{
		// Release components
		for (auto& pair : m_Components)
		{
			pair.second->Release();
		}
		m_Components.clear();
	}


	void GameObject::SetActive(bool state)
	{
		m_IsActive = state;
		for (auto& pair : m_Components)
		{
			pair.second->SetActive(state);
		}
	}
	bool GameObject::GetActive()
	{
		return m_IsActive;
	}
}