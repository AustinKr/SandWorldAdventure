#pragma once
#include <SWAEngine/dllClause.h>
#include <unordered_map>
#include <string>
#include "Component/IComponent.h"

namespace SWAEngine::GameObject
{
	// TODO: Rename BaseGameObject to GameObject and make custom functionality through Components
	// A physical object inside of the game that requires functionality every frame
	// (by default is an empty object that may be used to group other objects)
	struct SWA_ENGINE_API GameObject
	{
	private:
		std::string m_Name; // TODO: can make name changeable
		bool m_IsActive;

		std::unordered_map<std::string, Component::IComponent*> m_Components;
	public:
		// Registers the component, returns nullptr if the component alias is not available or it failed to insert for whatever reason
		template<typename TYPE>
		TYPE* RegisterComponent(TYPE* pComp)
		{
			if (m_Components.contains(pComp->GetName()))
				return nullptr;
			return m_Components.insert(std::make_pair(pComp->GetName(), pComp)).second ? pComp : nullptr;
		}
		void TryUnregisterComponent(Component::IComponent* pComp);
		template<typename TYPE>
		TYPE* TryGetComponent(std::string name)
		{
			return m_Components.contains(name) ? m_Components.at(name) : nullptr;
		}

		std::string const GetName();
		bool GetActive();

		// Creates an object
		GameObject(std::string objName);

		// Nothing by default
		virtual void Update(Math::Time);
		// Release this by default
		virtual void Release();
		// Sets active state by default
		virtual void SetActive(bool state);
	};
}