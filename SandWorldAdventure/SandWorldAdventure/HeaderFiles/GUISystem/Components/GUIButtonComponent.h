#pragma once
#include "IGUIComponent.h"
#include "HeaderFiles/Event/EventHandler.h"
#include "HeaderFiles/Event/DelegateTypes/IGUIButtonEventDelegate.h"

namespace SandboxEngine::GUISystem::Components
{
	struct GUIButtonComponent : public IGUIComponent
	{
	public:
		virtual ComponentTags GetTag() override;

		// Subscribes to MouseButtonEventHandler in MasterWindow
		virtual void Initialize(GUIElement* pElement) override;
		virtual void Release() override;

		// Has no arguments, but expects event delegates to be of IGUIButtonEventDelegate
		Event::EventHandler<Event::IGUIButtonEventDelegate> ButtonEventHandler;

	private:

		// For MasterWindow::MouseButtonEventHandler
		struct MouseButtonDelegate : Event::IEventDelegate
		{
		private:
			GUIButtonComponent* mp_Button;
			GUIElement* mp_Element;

		public:
			MouseButtonDelegate(GUIButtonComponent* pButton, GUIElement* pElement);
			virtual void Invoke(void*) override;
		};
	};
}