#pragma once
#include "HeaderFiles/Math.h"

namespace SandboxEngine::GUISystem
{
	struct GUITransform
	{
		// 0-1 values that are percentages of how much space this takes up relative to the parent (does not need to be updated as it is the basis for global position)
		Vector2 LocalPosition;
		// 0-1 values that are percentages of how much space this takes up relative to the parent (does not need to be updated as it is the basis for global scale)
		Vector2 LocalScale;
		// The global(screen coordinate) position of this element in pixels
		Vector2Int GlobalPosition;
		// The global(screen coordinate) scale of this element in pixels
		Vector2Int GlobalScale;

		inline GUITransform() : LocalPosition{}, LocalScale{}, GlobalPosition{}, GlobalScale{}
		{
			/*nothign*/
		}
	};
}