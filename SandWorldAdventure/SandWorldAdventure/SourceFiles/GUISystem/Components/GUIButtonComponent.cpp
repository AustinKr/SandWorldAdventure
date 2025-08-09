#include "HeaderFiles/GUISystem/Components/GUIButtonComponent.h"
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
		MasterWindow::MouseButtonEventHandler.SubscribeEvent(new MouseButtonDelegate(this, pElement));
	}
	void GUIButtonComponent::Release()
	{
		delete(this);
	}

	// - Mouse button event delegate -
	GUIButtonComponent::MouseButtonDelegate::MouseButtonDelegate(GUIButtonComponent* pButton, GUIElement* pElement) : mp_Button(pButton), mp_Element(pElement)
	{/*nothing*/}
	void GUIButtonComponent::MouseButtonDelegate::Invoke(void*)
	{
		// Mouse button change!

		if (!mp_Element->GetActiveState() || mp_Button->ButtonEventHandler.GetCount() == 0)
			return;

		// Get element transform
		GUITransform transform = mp_Element->GetTransform();

		// Get mouse position
		Vector2 mousePos = {};
		glfwGetCursorPos(MasterWindow::p_glfwWindow, &mousePos.X, &mousePos.Y);
		mousePos.Y = MasterWindow::Pipeline.ActiveCamera.ScreenSize.Y - mousePos.Y; // Invert

		// Test whether in bounds
		if (mousePos.X < transform.GlobalPosition.X || mousePos.Y < transform.GlobalPosition.Y ||
			mousePos.X >= transform.GlobalPosition.X + transform.GlobalScale.X || mousePos.Y >= transform.GlobalPosition.Y + transform.GlobalScale.Y)
			return;

		// Iterate
		for (auto iter = mp_Button->ButtonEventHandler.GetBegin(); iter != mp_Button->ButtonEventHandler.GetEnd(); iter++)
		{
			// Parse flags
			int flags = iter->second->ButtonEventFlags, key = -1;

			/*if(key & BUTTON_EVENT_ORIGINATE_IN_BOUNDS && !originateInBounds)
				return;*/

			if (key & BUTTON_EVENT_MOUSE_1)
				key = GLFW_MOUSE_BUTTON_1;
			else if (key & BUTTON_EVENT_MOUSE_2)
				key = GLFW_MOUSE_BUTTON_2;
			else if (key & BUTTON_EVENT_MOUSE_3)
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
			iter->second->Invoke(nullptr);
		}
	}
}