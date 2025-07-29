#pragma once
#include "HeaderFiles/GUISystem/GUITransform.h"
#include "HeaderFiles/GUISystem/ElementAlignmentFlags.h"
#include "HeaderFiles/Event/EventHandler.h"
#include <unordered_map>

#define GUIELEMENT_H
namespace SandboxEngine::GUISystem
{
#ifndef GUISYSTEM_H
#define GUISYSTEM_H
	class GUISystem;
#endif
#ifndef GUIHIERARCHY_H
#define GUIHIERARCHY_H
	class GUIHierarchy;
#endif

	namespace Components
	{
#ifndef IGUICOMPONENT_H
#define IGUICOMPONENT
		class IGUIComponent;
#endif
	}

	/*
	This is the basis of all gui elements. Do not inherit this- instead use the components system which allows multiple different functions to be attached to elements.
	*/
	class GUIElement
	{
		friend class GUIHierarchy;

	public:
		typedef unsigned long int UID;
	private:
		static const int MAX_PARENT_ELEMENTS;

		// This stores the position and scale and is always kept updated because every SetTransform operation will update it.
		GUITransform m_Transform;
		// fisrt: tag; second: pointer to component memory
		std::unordered_map<unsigned int, Components::IGUIComponent*> m_Components;
		// Reference to the parent
		UID m_ParentID;
		// References to the children
		std::vector<UID> m_Children;
		// Used to check whether the state needs to be changed when SetActiveState is called
		bool m_IsActive;

		// Used to apply alignment
		/// <param name="offset">The distance (from a given starting point)</param>
		/// <param name="aligmentFlags">The starting point</param>
		Vector2 ApplyAligmentFlags(Vector2 coord, Vector2 bounds, int alignmentFlags);

	public:
		struct GUICoordinate
		{
			Vector2 Offset;
			bool IsLocalSpace;
			int AlignmentFlags;

			inline GUICoordinate() : Offset{}, IsLocalSpace{}, AlignmentFlags{}
			{

			}
			inline GUICoordinate(Vector2 offset, bool isLocalSpace, int alignmentFlags = ALIGNMENT_LEFT | ALIGNMENT_BOTTOM) : Offset(offset), IsLocalSpace(isLocalSpace), AlignmentFlags(alignmentFlags)
			{

			}
		};

		// The UID used to register this element
		const UID Identifier;

		// Event system for transform changes
		Event::EventHandler<> TransformEventHandler;

		// Reference to system
		GUISystem* p_System;

		// Registers self
		GUIElement(GUISystem* pSystem);
		// Registers self
		GUIElement(GUISystem* pSystem, UID parentID);

		std::vector<UID>::iterator GetChildrenBegin();
		std::vector<UID>::iterator GetChildrenEnd();

		// Registers the component with this element. (will be released automatically)
		template<class COMP_TYPE = Components::IGUIComponent>
		inline COMP_TYPE* RegisterComponent(COMP_TYPE* pComponent)
		{
			m_Components.insert(std::make_pair(pComponent->GetTag(), pComponent));
			pComponent->Initialize(this);
			return pComponent;
		}
		// Tries to find a matching memory address in the registry and releases it
		void UnregisterComponent(Components::IGUIComponent* pComponent);
		
		template<class COMP_TYPE = Components::IGUIComponent>
		inline COMP_TYPE* GetComponent(unsigned int tag)
		{
			return (COMP_TYPE*)(m_Components.contains(tag) ? m_Components.at(tag) : nullptr);
		}

		/// <summary>
		/// Sets the position and scale with alignment.
		/// Updates global position and scale by iterating through parents and returning: the sum of their scaled coordinates, and the product of their scales.
		/// </summary>
		/// <param name="(position/scale)offset">
		/// Vector2: The distance (0-1 from a given staring point) that is the origin of this element's bounds.
		/// bool: If true, then the offset is interpreted as local space. </param>
		/// <param name="alignmentFlags">The starting point</param>		
		void SetTransform(GUICoordinate positionCoordinate, GUICoordinate scaleCoordinate);
		// Returns a copy of the transform
		GUITransform GetTransform();
		// Used when the child is reparented or the screen size changes
		void UpdateTransform();

		void SetActiveState(bool isActive);
		bool GetActiveState();

		// Converts the global coordinate to local space that is relative to this element's parent
		Vector2 ConvertGlobalToLocal(Vector2Int globalCoord, bool applyOrigin = true);
		std::pair<Vector2, Vector2> ConvertGlobalToLocal(Vector2Int globalPosition, Vector2Int globalScale);
		// Converts the local coordinate(that is relative to this element's parent) to global space 
		Vector2Int ConvertLocalToGlobal(Vector2 localCoord, bool applyOrigin = true);
		std::pair<Vector2Int, Vector2Int> ConvertLocalToGlobal(Vector2 localPosition, Vector2 localScale);
		
		virtual void Release();

	};
}