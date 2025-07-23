#pragma once
namespace SandboxEngine::GUISystem
{
	// Must combine a horizontal flag with a vertical
	enum ElementAlignmentFlag
	{
		ALIGNMENT_LEFT = 1, // Horizontal
		ALIGNMENT_CENTER_HORIZONTAL = 2, // Horizontal
		ALIGNMENT_RIGHT = 4, // Horizontal

		ALIGNMENT_BOTTOM = 8, // Vertical
		ALIGNMENT_CENTER_VERTICAL = 16, // Vertical
		ALIGNMENT_TOP = 32, // Vertical
	};
}