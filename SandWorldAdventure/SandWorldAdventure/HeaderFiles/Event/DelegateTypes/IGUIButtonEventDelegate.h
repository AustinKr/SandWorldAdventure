#pragma once
#include "IEventDelegate.h"
#include "HeaderFiles/GUISystem/ButtonEventFlags.h"

namespace SandboxEngine::Event
{
	// A wrapper on the event delegate
	struct IGUIButtonEventDelegate : IEventDelegate
	{
	public:
		int ButtonEventFlags;

		IGUIButtonEventDelegate(int flags = GUISystem::BUTTON_EVENT_MOUSE_1 | GUISystem::BUTTON_EVENT_MOUSE_DOWN) : ButtonEventFlags(flags)
		{
			/*nothing*/
		}
	};
}