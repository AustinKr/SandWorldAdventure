#include "SWAEngine/BaseGameObject.h"

namespace SWAEngine
{
	std::unordered_map<const char*, BaseGameObject*> BaseGameObject::ms_Registry;

	BaseGameObject* const BaseGameObject::GetObject(const char* name)
	{
		if (!ms_Registry.contains(name))
			return nullptr;

		return ms_Registry.at(name);
	}

	const char* const BaseGameObject::GetName()
	{
		return m_Name;
	}

	BaseGameObject::BaseGameObject(const char* name) : m_Name(name)
	{
		ms_Registry.insert(std::make_pair(name, this));
	}

	void BaseGameObject::Update(Time)
	{
		/*nothing*/
	}

	void BaseGameObject::Release()
	{
		ms_Registry.erase(m_Name);
		delete(this);
	}
}