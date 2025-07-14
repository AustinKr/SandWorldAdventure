#pragma once
#include "HeaderFiles/Math.h"
#include <Windows.h>

namespace SandboxEngine::GraphicsPipeline
{
	struct Texture
	{
#pragma pack(push, 1)
		struct BMPFileHeader
		{
			uint16_t Identifier; // Should be 0x4D42 (or BM is ASCII)
			uint32_t Size; // Size of file in bytes 
			uint16_t Reserved1;
			uint16_t Reserved2;
			uint32_t Offset; // Starting address of the byte where the bitmap image data can be found
		};
		// Interpreted as ABGR
		struct BMPExtraBitMasks
		{
			uint32_t red_mask;         // Bit mask for the red channel
			uint32_t green_mask;       // Bit mask for the green channel
			uint32_t blue_mask;        // Bit mask for the blue channel
			uint32_t alpha_mask;       // Bit mask for the alpha channel
		};
#pragma pack(pop)


		Vector2Int Size;
		// Each 4 or 3 bytes is one full color (rgba or rgb but use color mask)
		unsigned char* p_Colors;
		unsigned int ColorsSize; // Size in bytes of array

		BMPFileHeader BitmapFileHeader;
		BITMAPINFOHEADER BitmapInfoHeader;
		BMPExtraBitMasks BitmapExtraBitMasks;

		// Creates null texture
		inline Texture() : p_Colors(nullptr), ColorsSize(0), Size{}
		{
			/*nothing*/
		}
		// Holds texture information
		inline Texture(unsigned char* pData, int byteCount) : p_Colors(pData), ColorsSize(byteCount), Size{}
		{
			/*nothing*/
		}

		void Release();

		// only reads 24 or 32 bpp rgb or rgba bitmaps
		static Texture LoadBMPTextureFromFile(const char* fullFilePath);
	};
}