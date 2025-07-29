#pragma once
#include "HeaderFiles/GUISystem/ComponentTags.h"

#define IGUICOMPONENT
namespace SandboxEngine::GUISystem
{
#ifndef GUIELEMENT_H
#define GUIELEMENT_H
	class GUIElement;
#endif

	namespace Components
	{
		struct IGUIComponent
		{
			virtual ComponentTags GetTag() = 0;

			virtual void Initialize(GUIElement* pElement) = 0; // Initialize component (can also have a constructor)
			virtual void Release() = 0; // Release any stored data and itself
		};
	}
}