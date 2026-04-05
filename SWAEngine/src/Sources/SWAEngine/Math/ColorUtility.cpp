#include "SWAEngine/Math/ColorUtility.h"

namespace SWAEngine::Math
{
	GP2D::GP2D_HEX_COLOR MixColor(GP2D::GP2D_HEX_COLOR colA, GP2D::GP2D_HEX_COLOR colB, float factor)
	{
		int r1 = (colA >> 24) & 0xFF;
		int g1 = (colA >> 16) & 0xFF;
		int b1 = (colA >> 8) & 0xFF;
		int a1 = colA & 0xFF;

		int r2 = (colB >> 24) & 0xFF;
		int g2 = (colB >> 16) & 0xFF;
		int b2 = (colB >> 8) & 0xFF;
		int a2 = colB & 0xFF;

		int red = (float)r1 * factor + r2 * (1.0f - factor);
		int green = (float)g1 * factor + g2 * (1.0f - factor);
		int blue = (float)b1 * factor + b2 * (1.0f - factor);
		int alpha = (float)a1 * factor + a2 * (1.0f - factor);

		return alpha | (blue << 8) | (green << 16) | (red << 24);
	}
}