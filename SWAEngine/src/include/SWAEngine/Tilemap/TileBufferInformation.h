#pragma once
#include "GP2D/Math/vector2.h"
#include "GP2D/Pipeline/GraphicsTypes.h"

namespace SWAEngine::Tilemap
{
	struct TileBufferInformation
	{
		GP2D::Pipeline::GP2D_HEX_COLOR* const p_Buffer;
		
		// The texture origin (in tile/pixels units)
		GP2D::Math::Int2 TextureOrigin;
		// The texture origin (in tile/pixels units)
		GP2D::Math::Int2 TextureSize;

		// Defaults to null information
		TileBufferInformation()
			: p_Buffer(nullptr), TextureOrigin{}, TextureSize{}
		{

		}

		TileBufferInformation(GP2D::Pipeline::GP2D_HEX_COLOR* const pBuffer, GP2D::Math::Int2 origin, GP2D::Math::Int2 size)
			: p_Buffer(pBuffer), TextureOrigin(origin), TextureSize(size)
		{
			
		}
	};
}
