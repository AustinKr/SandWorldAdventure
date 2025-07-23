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
		static const UID NULL_UID;

		GUISystem *p_System;

		GUIHierarchy(GUISystem *pSystem);
		void Release();

		std::unordered_map<UID, GUIElement*>::iterator GetBegin();
		std::unordered_map<UID, GUIElement*>::iterator GetEnd();

		void RegisterElement(GUIElement* pElement);
		void UnregisterElement(UID id);
		GUIElement* GetElement(UID id);
	private:
		UID m_UIDNext;
		std::unordered_map<UID, GUIElement*> m_ElementsRegistry;
	};
}