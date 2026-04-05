#pragma once
#include "SWAEngine/dllClause.h"
#include "GP2D/GraphicsTypes.h"

namespace SWAEngine::Math
{
	SWA_ENGINE_API GP2D::GP2D_HEX_COLOR MixColor(GP2D::GP2D_HEX_COLOR colA, GP2D::GP2D_HEX_COLOR colB, float factor);
}