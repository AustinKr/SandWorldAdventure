#include "HeaderFiles/GraphicsPipeline/Shaders/TextureShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	const char* TextureShaderInformation::TextureName = "Tex";
	const char* TextureShaderInformation::RedMaskName = "RedMask";
	const char* TextureShaderInformation::GreenMaskName = "GreenMask";
	const char* TextureShaderInformation::BlueMaskName = "BlueMask";
	const char* TextureShaderInformation::AlphaMaskName = "AlphaMask";

	int TextureShaderInformation::UpdateVertexData(
		IGraphicsPipeline* pPipeline,
		Mesh* pMesh,
		GLuint vertexBufferName,
		GLuint pVertexArray,
		const Vertex* pVertexBuffer,
		int count)
	{
		if (pMesh->Texture.p_Colors != nullptr)
		{
			// 32 bits = one full color(4bytes); 8 bits x4 = 256 values x 4 = 0xff x4
			// Set bit masks
			uint32_t maskR = pMesh->Texture.BitmapExtraBitMasks.red_mask;
			uint32_t maskG = pMesh->Texture.BitmapExtraBitMasks.green_mask;
			uint32_t maskB = pMesh->Texture.BitmapExtraBitMasks.blue_mask;
			uint32_t maskA = pMesh->Texture.BitmapExtraBitMasks.alpha_mask;
			glUniform4f(((GraphicsPipeline2D*)pPipeline)->GetUniformLocation(RedMaskName), maskR >> 16 & 0xff, maskR >> 8 & 0xff, maskR & 0xff, maskR >> 24 & 0xff);
			glUniform4f(((GraphicsPipeline2D*)pPipeline)->GetUniformLocation(GreenMaskName), maskG >> 16 & 0xff, maskG >> 8 & 0xff, maskG & 0xff, maskG >> 24 & 0xff);
			glUniform4f(((GraphicsPipeline2D*)pPipeline)->GetUniformLocation(BlueMaskName), maskB >> 16 & 0xff, maskB >> 8 & 0xff, maskB & 0xff, maskB >> 24 & 0xff);
			glUniform4f(((GraphicsPipeline2D*)pPipeline)->GetUniformLocation(AlphaMaskName), maskA >> 16 & 0xff, maskA >> 8 & 0xff, maskA & 0xff, maskA >> 24 & 0xff);

			// Create
			glGenTextures(1, &p_TextureBufferName);
			glBindTexture(GL_TEXTURE_2D, p_TextureBufferName);
			// Set params
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			// Set data

			int internalFormat = GL_RGBA8, format = GL_RGBA, type = GL_UNSIGNED_BYTE;
			switch (pMesh->Texture.BitmapInfoHeader.biBitCount)
			{
				/*case 8:
					internalFormat = GL_R;
					format = GL_R;
					break;
				case 16:
					internalFormat = GL_RG;
					format = GL_RG;
					break;*/
			case 24:
				internalFormat = GL_RGB8;
				format = GL_RGB;
				break;
			case 32:
				break;
			default:
				fprintf(stderr, "Texture color format not supported!");
				return -1;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, pMesh->Texture.Size.X, pMesh->Texture.Size.Y, 0, format, type, pMesh->Texture.p_Colors);
			((GraphicsPipeline2D*)pPipeline)->TryPrintGlError();
		}
		return ShaderInformation::UpdateVertexData(pPipeline, pMesh, vertexBufferName, pVertexArray, pVertexBuffer, count);
	}

	int TextureShaderInformation::UpdateUniformVariables(IGraphicsPipeline* pPipeline)
	{
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, TextureName);
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, RedMaskName);
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, GreenMaskName);
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, BlueMaskName);
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, AlphaMaskName);

		return ShaderInformation::UpdateUniformVariables(pPipeline);
	}

	void TextureShaderInformation::Release()
	{
		glDeleteTextures(1, &p_TextureBufferName);
	}
}