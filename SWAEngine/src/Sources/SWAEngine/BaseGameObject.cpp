#include "SWAEngine/BaseGameObject.h"
#include "SWAEngine/SceneManager.h"

namespace SWAEngine
{
	
	const char* const BaseGameObject::GetName()
	{
		return m_Name;
	}

	BaseGameObject::BaseGameObject(const char* name, const char* sceneName) : m_Name(name)
	{
		Scene& rScene = SceneManager::GetScene(sceneName);
		rScene.RegisterObject(this);
		m_IsActive = rScene.GetActive();
	}

	void BaseGameObject::Update(Time)
	{
		/*nothing*/
	}

	void BaseGameObject::Release()
	{
		delete(this);
	}


	void BaseGameObject::SetActive(bool state)
	{
		m_IsActive = state;
	}
	bool BaseGameObject::GetActive()
	{
		return m_IsActive;
	}
}