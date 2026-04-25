#pragma once
#include <unordered_map>
#include <string>
#include "Component/IComponent.h"
#include "dllClause.h"

namespace SWAEngine
{
#define GAME_OBJECT_H
#ifndef SCENE_H
#define SCENE_H
	class SWA_ENGINE_API Scene;
#endif

	// A physical object inside of the game that requires functionality every frame
	// (by default is an empty object that may be used to group other objects)
	// GameObjects can only have one of each Component
	class SWA_ENGINE_API GameObject
	{
		friend SWAEngine::Scene;

	private:
		std::string m_Name; // TODO: can make name changeable
		bool m_IsActive;

		std::unordered_map<std::string, Component::IComponent*> m_Components;
	private:
		// Creates an object
		GameObject(std::string objName);
	public:
		// Only registers the component, returns nullptr if the component alias is not available or it failed to insert for whatever reason
		template<typename TYPE>
		TYPE* const TryRegisterComponent(TYPE* pComp)
		{
			if (m_Components.contains(pComp->GetName()))
				return nullptr;
			return m_Components.insert(std::make_pair(pComp->GetName(), pComp)).second ? pComp : nullptr;
		}
		void TryUnregisterComponent(std::string name);
		// Expects TYPE to be of IComponent and contain a static std::string GetName()
		template<typename TYPE>
		TYPE* const TryGetComponent()
		{
			return m_Components.contains(TYPE::GetName()) ? static_cast<TYPE*>(m_Components.at(TYPE::GetName())) : nullptr;
		}
		// Gets the current component, or create and initalizes one if it doesn't already exist
		// Expects TYPE to be of IComponent and contain a static std::string GetName()
		template<typename TYPE>
		TYPE* const GetComponent()
		{
			TYPE* comp = TryGetComponent<TYPE>();
			if (comp == nullptr)
			{
				comp = static_cast<TYPE*>(m_Components.insert(std::make_pair(TYPE::GetName(), new TYPE())).first->second);
				comp->Initialize(GetName());
			}
			return comp;
		}

		std::string const GetName();
		bool GetActive();

		// Updates registered components
		void Update(Math::Time time);
		// Releases registered components
		void Release();
		void SetActive(bool state);
	};
}