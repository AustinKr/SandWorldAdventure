#pragma once
namespace SandboxEngine::GUISystem
{
	// Combine State, Number, and Other(optional)
	enum ButtonEventFlag
	{
		BUTTON_EVENT_MOUSE_DOWN = 1, // State
		BUTTON_EVENT_MOUSE_UP = 2, // State
		BUTTON_EVENT_MOUSE_CLICKED = 4,// State

		BUTTON_EVENT_MOUSE_1 = 8,// Number
		BUTTON_EVENT_MOUSE_2 = 16,// Number
		BUTTON_EVENT_MOUSE_3 = 32,// Number

		BUTTON_EVENT_ORIGINATE_IN_BOUNDS = 64, // Other
	};
}