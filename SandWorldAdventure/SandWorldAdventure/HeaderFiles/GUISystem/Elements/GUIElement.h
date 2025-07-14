#pragma once
#include "HeaderFiles/GUISystem/ElementAlignmentFlags.h"
#include "HeaderFiles/Math.h"
#include <vector>

#define GUIELEMENT_H
namespace SandboxEngine::GUISystem
{
#ifndef GUISYSTEM_H
#define GUISYSTEM_H
	class GUISystem;
#endif

	//TODO: ISSUES:
	// . Parent/Children of elements are supposed to order in rendering(not really necessary but would be nice if there was some editor to create ui)
	
	// Handles creating its own mesh(if it is meant to have one) and behavior.
	// Also responsible for registering itself.
	// 
	// Does nothing by default and is a blank element(do not instantiate).
	class GUIElement
	{
	private:
		// Sets the position of the gui element(in most cases this is the mesh position)
		virtual void SetElementPosition(Vector2Int position) = 0;
		// Sets the position of the gui element(in most cases this is the mesh position)
		virtual void SetElementScale(Vector2Int scale) = 0;

		// The original position from a certain alignment
		std::pair<Vector2Int, int> m_OriginalPosition;
		// The original scale from a certain alignment
		std::pair<Vector2Int, int> m_OriginalScale;
	public:
		typedef unsigned long int UID;

		GUISystem* p_System;
		UID Parent;
		// TODO: Also you might want to make the children and parent readonly to external code
		std::vector<UID> Children; // TODO: the parent/children were supposed to play a part in the rendering order

		GUIElement(GUISystem* pSystem);
		GUIElement(GUISystem* pSystem, UID parent);
		void EraseChild(UID id);

		virtual void Release();

		// By default uses the stored origin coordinates and recalls transformation functions
		virtual void OnScreenResize(Vector2Int newSize);
		// Gets the position of the gui element
		virtual Vector2Int GetPosition() = 0;
		// Gets the scale of the gui element
		virtual Vector2Int GetScale() = 0;

		/// <summary> </summary>
		/// <param name="position">Position of the element</param>
		/// <param name="aligmentFlags">Where the position is measured from, all the same direction. e.g. {(-1, 2), bottom right} would be 1 from the right, and 2 from the bottom</param>
		virtual void SetPosition(Vector2Int position, int aligmentFlags);
		/// <summary> </summary>
		/// <param name="scale">Scale of the element</param>
		/// <param name="aligmentFlags">Where the scale is measured from, all the same direction</param>
		virtual void SetScale(Vector2Int scale, int aligmentFlags);
	};
}