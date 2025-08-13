#pragma once
#include "IGUIComponent.h"
#include "HeaderFiles/Event/EventHandler.h"
#include "HeaderFiles/GUISystem/ButtonEventData.h"

#define GUIBUTTONCOMPONENT_H

namespace SandboxEngine::GUISystem::Components
{
	struct GUIButtonComponent : public IGUIComponent
	{
	private:
		static void OnMouseButton(void*, void* pElement);

	public:
		Event::EventHandler<void*, ButtonEventData> ButtonEventHandler;

		// Subscribes to MouseButtonEventHandler in MasterWindow
		virtual void Initialize(GUIElement* pElement) override;
		virtual void Release() override;

		virtual ComponentTags GetTag() override;
	};
}