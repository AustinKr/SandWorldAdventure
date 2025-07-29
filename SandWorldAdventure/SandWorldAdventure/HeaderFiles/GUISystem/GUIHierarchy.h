#pragma once
#include "HeaderFiles/GraphicsPipeline/Meshes/IMesh.h"
#include <unordered_map>

#define GUIHIERARCHY_H
namespace SandboxEngine::GUISystem
{
#ifndef GUISYSTEM_H
#define GUISYSTEM_H
	class GUISystem;
#endif
#ifndef GUIELEMENT_H
#define GUIELEMENT_H
	class GUIElement;
#endif

	class GUIHierarchy
	{
	public:
		typedef unsigned long int UID;
		typedef std::unordered_map<UID, GUIElement*> COLLECTION;
		static const UID NULL_UID;

		GUISystem *p_System;

		GUIHierarchy(GUISystem *pSystem);
		void Release();

		COLLECTION::iterator GetBegin();
		COLLECTION::iterator GetEnd();

		UID RegisterElement(GUIElement* pElement);
		void UnregisterElement(UID id);
		GUIElement* GetElement(UID id);

		// Unparents the child. Also adds the element to another collection that stores all the unparented/root elements
		void UnparentElement(UID parentID, UID childID);
		// Parents to the parent. Also removes the element from the root elements collection when an element has been parented
		void ParentElement(UID parentID, UID childID);

		void EraseElement(UID elementID);
		void EraseChildren(UID elementID);

		// Used when the screen resizes
		void UpdateAllTransforms();
	private:
		UID m_UIDNext;
		// Contains all elements
		COLLECTION m_ElementsRegistry;
		// Elements that have no parent element(parent is the window)
		COLLECTION m_RootElements;
	};
}