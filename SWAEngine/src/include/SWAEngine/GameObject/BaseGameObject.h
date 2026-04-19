#pragma once
#include <SWAEngine/dllClause.h>
#include <unordered_map>
#include <string>
#include "Component/IComponent.h"

namespace SWAEngine::GameObject
{
	// TODO: Rename BaseGameObject to GameObject and make custom functionality through Components
	// Should be created by new()
	// A physical object inside of the game that requires functionality every frame
	// (by default is an empty object that may be used to group other objects)
	struct SWA_ENGINE_API BaseGameObject
	{
	private:
		const char* m_Name; // TODO: can make name changeable
		bool m_IsActive;

		std::unordered_map<std::string, Component::IComponent*> m_Components;
	public:
		// Registers the component, returns nullptr if the alias is not available or it failed to insert for whatever reason
		template<typename TYPE>
		TYPE* RegisterComponent(std::string alias, TYPE* pComp)
		{
			if (m_Components.contains(alias))
				return nullptr;
			return m_Components.insert(std::make_pair(alias, pComp)).second ? pComp : nullptr;
		}
		template<typename TYPE>
		TYPE* TryGetComponent(std::string alias)
		{
			return m_Components.contains(alias) ? m_Components.at(alias) : nullptr;
		}

		const char* const GetName();
		bool GetActive();

		// Registers self (with the active scene if not specified)
		BaseGameObject(const char* objName, const char* sceneName = nullptr);

		// Nothing by default
		virtual void Update(Math::Time);
		// Release this by default
		virtual void Release();
		// Sets active state by default
		virtual void SetActive(bool state);
	};
}