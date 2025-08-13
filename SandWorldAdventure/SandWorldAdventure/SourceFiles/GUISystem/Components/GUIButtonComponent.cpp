#include "HeaderFiles/GUISystem/Components/GUIButtonComponent.h"
#include "HeaderFiles/GUISystem/ComponentTags.h"
#include "HeaderFiles/GUISystem/ButtonEventFlags.h"
#include "HeaderFiles/GUISystem/GUISystemFramework.h"

#include "HeaderFiles/MasterWindow.h"

namespace SandboxEngine::GUISystem::Components
{
	ComponentTags GUIButtonComponent::GetTag()
	{
		return ComponentTags::COMPONENT_TAG_BUTTON;
	}

	void GUIButtonComponent::Initialize(GUIElement* pElement)
	{
		// Subscribe event
		MasterWindow::MouseButtonEventHandler.SubscribeEvent(Event::BaseEventDelegate<void*, void*>(OnMouseButton, (void*)pElement));
	}
	void GUIButtonComponent::Release()
	{
		delete(this);
	}

	void GUIButtonComponent::OnMouseButton(void*, void* pElement)
	{
		// Mouse button change!

		GUIElement* element = ((GUIElement*)pElement);
		GUIButtonComponent* button = element->GetComponent<GUIButtonComponent>(ComponentTags::COMPONENT_TAG_BUTTON);

		if (!element->GetActiveState() || button->ButtonEventHandler.GetCount() == 0)
			return;

		// Get element transform
		GUITransform transform = element->GetTransform();

		// Get mouse position
		Vector2 mousePos = {};
		glfwGetCursorPos(MasterWindow::p_glfwWindow, &mousePos.X, &mousePos.Y);
		mousePos.Y = MasterWindow::Pipeline.ActiveCamera.ScreenSize.Y - mousePos.Y; // Invert

		// Test whether in bounds
		if (mousePos.X < transform.GlobalPosition.X || mousePos.Y < transform.GlobalPosition.Y ||
			mousePos.X >= transform.GlobalPosition.X + transform.GlobalScale.X || mousePos.Y >= transform.GlobalPosition.Y + transform.GlobalScale.Y)
			return;

		// Iterate
		for (auto iter = button->ButtonEventHandler.GetBegin(); iter != button->ButtonEventHandler.GetEnd(); iter++)
		{
			// Parse flags
			int flags = iter->second.ExtraData.ButtonFlags, key = -1;

			/*if(flags & BUTTON_EVENT_ORIGINATE_IN_BOUNDS && !originateInBounds)
				return;*/

			if (flags & BUTTON_EVENT_MOUSE_1)
				key = GLFW_MOUSE_BUTTON_1;
			else if (flags & BUTTON_EVENT_MOUSE_2)
				key = GLFW_MOUSE_BUTTON_2;
			else if (flags & BUTTON_EVENT_MOUSE_3)
				key = GLFW_MOUSE_BUTTON_3;
			else
			{
#ifdef _DEBUG
				throw std::exception();
#endif
				fprintf(stderr, "GUIButton Event flags missing mouse button bit!");
				return;
			}

			if (flags & BUTTON_EVENT_MOUSE_DOWN && MasterWindow::GetKeyState(key) != GLFW_PRESS)
				break;
			else if (flags & BUTTON_EVENT_MOUSE_UP && MasterWindow::GetKeyState(key) != GLFW_RELEASE)
				break;
			/*else if (key & BUTTON_EVENT_MOUSE_CLICKED && MasterWindow::GetKeyState(key) != GLFW_CLICKED)
				break;*/

			// Invoke
			iter->second.p_Function(nullptr, iter->second.ExtraData);
		}
	}
}