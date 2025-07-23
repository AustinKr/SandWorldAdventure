#pragma once
#include "HeaderFiles/GUISystem/GUITransform.h"
#include "HeaderFiles/GUISystem/ElementAlignmentFlags.h"
#include "HeaderFiles/Event/EventHandler.h"
#include <vector>

#define GUIELEMENT_H
namespace SandboxEngine::GUISystem
{
#ifndef GUISYSTEM_H
#define GUISYSTEM_H
	class GUISystem;
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
	private:
		//typedef std::pair<Vector2Int, int> GUI_ALIGNMENT_COORDINATE;
		// TODO: alignment coord only needed when setting coordinate or when parent is the screen (i think)
		//// The original position from a certain alignment
		//GUI_ALIGNMENT_COORDINATE m_OriginalPosition;
		//// The original scale from a certain alignment
		//GUI_ALIGNMENT_COORDINATE m_OriginalScale;

		static const int MAX_PARENT_ELEMENTS;

		// This stores the position and scale and is always kept updated because every SetTransform operation will update it.
		GUITransform m_Transform;

		std::vector<Components::IGUIComponent*> m_Components;

		// Used to apply alignment
		/// <param name="offset">The distance (from a given starting point)</param>
		/// <param name="aligmentFlags">The starting point</param>
		Vector2 ApplyAligmentFlags(Vector2 coord, Vector2 bounds, int alignmentFlags);

	public:
		typedef unsigned long int UID;

		// Event system for transform changes
		Event::EventHandler<> TransformEventHandler;

		// Reference to system
		GUISystem* p_System;
		
		// Reference to the parent
		UID Parent;
		// References to the children
		std::vector<UID> Children;

		GUIElement(GUISystem* pSystem);
		GUIElement(GUISystem* pSystem, UID parent);
		
		// Registers the component with this element. (will be released automatically)
		void RegisterComponent(Components::IGUIComponent* pComponent);
		// Tries to find a matching memory address in the registry and releases it
		void UnregisterComponent(Components::IGUIComponent* pComponent);

		/// <summary>
		/// Sets the position and scale with alignment.
		/// Updates global position and scale by iterating through parents and returning: the sum of their scaled coordinates, and the product of their scales.
		/// </summary>
		/// <param name="(position/scale)offset">The distance (0-1 from a given staring point) that is the origin of this element's bounds</param>
		/// <param name="isLocalSpace">If true, then the offset is interpreted as local space</param>
		/// <param name="alignmentFlags">The starting point</param>		
		void SetTransform(Vector2 positionOffset, Vector2 scaleOffset, bool isLocalSpace, int alignmentFlags = ALIGNMENT_LEFT | ALIGNMENT_BOTTOM);
		// Returns a copy of the transform
		GUITransform GetTransform();
		
		// Converts the global coordinate to local space that is relative to this element(as if it were a child of this element)
		Vector2 ConvertGlobalToLocal(Vector2Int globalCoord, bool applyOrigin = true);
		std::pair<Vector2, Vector2> ConvertGlobalToLocal(Vector2Int globalPosition, Vector2Int globalScale);
		// Converts the local coordinate, that is relative to this element(as if it were a child of this element), to global space
		Vector2Int ConvertLocalToGlobal(Vector2 localCoord, bool applyOrigin = true);
		std::pair<Vector2Int, Vector2Int> ConvertLocalToGlobal(Vector2 localPosition, Vector2 localScale);
		
		virtual void Release();

	};
}