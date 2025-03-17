#pragma once
#include "Math.h"

namespace GraphicsPipeline
{
	struct Camera
	{
	public:
		float2 Origin;
		float2 Scale;

		Camera() : Origin({0,0}), Scale({1,1})
		{
			/*nothing*/
		}
	};
}