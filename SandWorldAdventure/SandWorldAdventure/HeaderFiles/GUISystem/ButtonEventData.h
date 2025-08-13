#pragma once
#include "ButtonEventFlags.h"

namespace SandboxEngine::GUISystem
{
	struct ButtonEventData
	{
		// Determines how this button is interactable
		int ButtonFlags;
		
		// (Optional) Extra integer data
		int ExtraFlags;

		// (Optional) More extra data
		void* p_Extra;

		inline ButtonEventData(int extraFlags = 0, void* pExtra = nullptr, int flags = BUTTON_EVENT_MOUSE_1 | BUTTON_EVENT_MOUSE_DOWN)
		{
			ExtraFlags = extraFlags;
			ButtonFlags = flags;
			p_Extra = pExtra;
		}
	};
}