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

		// Collection of Components with Tags as keys
		std::unordered_map<unsigned int, Component::IComponent*> m_Components;
	private:
		// Creates an object
		GameObject(std::string objName);
	public:
		
		// Returns the first Component with the given Tag, or else nullptr
		// Expects TYPE to be of IComponent
		template<typename TYPE>
		TYPE* const TryFindComponent()
		{
			unsigned int tag = TYPE{}.GetTag(); // Create temporary object to obtain tag

			for (auto& comp : m_Components)
			{
				if (comp.second->GetTag() & tag)
					return static_cast<TYPE*>(comp.second);
			}
			return nullptr;
		}
		// Evaluates to TryFindComponent<TYPE>() != nullptr
		template<typename TYPE>
		bool ContainsComponent()
		{
			return TryFindComponent<TYPE>() != nullptr;
		}
		// Expects TYPE to be of IComponent
		template<typename TYPE>
		TYPE* const TryUnregister()
		{
			TYPE* pComp = TryFindComponent<TYPE>();
			pComp->Release();
			m_Components.erase(pComp->GetTag());
		}
		// Gets the existing Component, or creates and initalizes one if it doesn't already exist
		// Expects TYPE to be of IComponent
		template<typename TYPE>
		TYPE* const GetComponent()
		{
			TYPE* comp = TryFindComponent<TYPE>();
			if (comp == nullptr)
			{
				TYPE* newComp = new TYPE();
				comp = static_cast<TYPE*>(m_Components.insert(std::make_pair(newComp->GetTag(), newComp)).first->second);
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