#pragma once
#include <SWAEngine/dllClause.h>
#include <unordered_map>
#include <string>
#include "Component/IComponent.h"

namespace SWAEngine
{
#define GAME_OBJECT_H
#ifndef SCENE_H
#define SCENE_H
	class SWA_ENGINE_API Scene;
#endif

	namespace GameObject
	{
		// TODO: Could make all component factory static functions into methods of gameobject that automatically registers
		// TODO: Rename BaseGameObject to GameObject and make custom functionality through Components
		// A physical object inside of the game that requires functionality every frame
		// (by default is an empty object that may be used to group other objects)
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
			void TryUnregisterComponent(Component::IComponent* pComp);
			template<typename TYPE>
			TYPE* const TryGetComponent(std::string name) // TODO: MAke this only require the template
			{
				return m_Components.contains(name) ? static_cast<TYPE*>(m_Components.at(name)) : nullptr;
			}
			// Gets the current component, or create and initalizes one if it doesn't already exist
			// Note the alias must match TYPE::GetName();
			template<typename TYPE>
			TYPE* const GetComponent(std::string alias)
			{
				TYPE* comp = TryGetComponent<TYPE>(alias);
				if (comp == nullptr)
				{
					comp = static_cast<TYPE*>(m_Components.insert(std::make_pair(alias, new TYPE())).first->second);
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
}