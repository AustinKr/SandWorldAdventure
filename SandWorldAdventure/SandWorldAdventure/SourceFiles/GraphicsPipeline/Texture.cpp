#include "HeaderFiles/GraphicsPipeline/Texture.h"
#include "HeaderFiles/FileUtility.h"
#include <string>

namespace SandboxEngine::GraphicsPipeline
{
	void Texture::Release()
	{
		if (p_Colors == nullptr || ColorsSize == 0)
			return;
		
		delete[](p_Colors);
		p_Colors = nullptr;
		ColorsSize = 0;
	}

	Texture Texture::LoadBMPTextureFromFile(const char* fullFilePath)
	{
		Texture texture = {};
		texture.BitmapExtraBitMasks =
		{
			0x000000ff,
			0x0000ff00,
			0x00ff0000,
			0xff000000,
		};

		char* pData; long dataSize = 0;
		FileUtility::LoadFile(&pData, &dataSize, fullFilePath, true, 0x8a, 0);
		if (pData == nullptr)
			return texture;
		fprintf(stdout, std::string(" Pixel Array Size:").append(std::to_string(dataSize - 0x8a)).append("\n").c_str());


		// Read bmp file header
		texture.BitmapFileHeader = *reinterpret_cast<BMPFileHeader*>(pData); // size 14 bytes
		if (texture.BitmapFileHeader.Identifier != 0x4D42) // Check for BM in ASCII code
		{
			fprintf(stderr, std::string("Failed to identify .bmp file: ").append(fullFilePath).c_str());
			return texture;
		}
		int currentDataOffset = 14; // Size 14 bytes

		// Read DIB header
		texture.BitmapInfoHeader = *reinterpret_cast<BITMAPINFOHEADER*>(pData + currentDataOffset); // Those versions of headers that come after BITMAPINFOHEADER only add to the end, so this is the minimum version we need
		texture.Size = Vector2Int(texture.BitmapInfoHeader.biWidth, texture.BitmapInfoHeader.biHeight);
		int rowStride = (texture.BitmapInfoHeader.biBitCount * texture.Size.X + 31) / 32 * 4;
		texture.ColorsSize = rowStride * texture.Size.Y;
		currentDataOffset += texture.BitmapInfoHeader.biSize; // Add size of dib to offset
		
		// Extra bit masks
		if (texture.BitmapInfoHeader.biSize == sizeof(BITMAPINFOHEADER)) // Extra bit masks only present if bitmapinfoheader is used
			switch (texture.BitmapInfoHeader.biCompression) // BI_BITFIELDS is the only compression(other than 0 or rgb) that is supported
			{
			case BI_RGB:
				break; // Default ARGB
			case BI_BITFIELDS:
			{
				BITMAPINFO info = (BITMAPINFO)texture.BitmapInfoHeader;
				texture.BitmapExtraBitMasks.red_mask = info.bmiColors[0].rgbRed;// >> 8; 4?
				texture.BitmapExtraBitMasks.green_mask = info.bmiColors[0].rgbGreen;// >> 8;
				texture.BitmapExtraBitMasks.blue_mask = info.bmiColors[0].rgbBlue;// >> 8;


				//texture.BitmapExtraBitMasks = {};
				//// Extra bit masks
				//texture.BitmapExtraBitMasks.red_mask = *reinterpret_cast<uint32_t*>(pData + currentDataOffset);// *reinterpret_cast<BMPColorHeader*>(pData + currentDataOffset);
				//currentDataOffset += 4;
				//texture.BitmapExtraBitMasks.green_mask = *reinterpret_cast<uint32_t*>(pData + currentDataOffset);
				//currentDataOffset += 4;
				//texture.BitmapExtraBitMasks.blue_mask = *reinterpret_cast<uint32_t*>(pData + currentDataOffset);
				//currentDataOffset += 4;
				/*if (texture.BitmapInfoHeader.biBitCount == 32)
				{
					texture.BitmapExtraBitMasks.alpha_mask = *reinterpret_cast<uint32_t*>(pData + currentDataOffset);
					currentDataOffset += 4;
				}*/
				break;
			}
			}
		// Color table is unsupported
		if (texture.BitmapInfoHeader.biClrUsed != 0)
		{
			fprintf(stderr, "Color palette is unsupported!");
			return texture;
		}
		
		// Read pixel array
		texture.p_Colors = new unsigned char[texture.ColorsSize];
		memcpy_s(texture.p_Colors, rowStride * texture.Size.Y, pData + texture.BitmapFileHeader.Offset, rowStride * texture.Size.Y);// texture.ColorsSize x2
		
		delete[](pData);
		return texture;
	}
}