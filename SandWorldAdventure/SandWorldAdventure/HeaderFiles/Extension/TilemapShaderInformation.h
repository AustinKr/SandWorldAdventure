#pragma once
#include "HeaderFiles/GraphicsPipeline/Shaders/BaseShaderInformation.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/StaticQuadtreeTypes.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct TilemapShaderInformation : public BaseShaderInformation
	{
	public:
		GLuint p_TextureBufferName;
		const char *TextureName = "TilemapTex";
		const char *TextureOriginCoordName = "TexOriginCoord";
		const char *TextureSizeCoordName = "TexSizeCoord";
		const char *TextureSizeName = "TexSize";

		TilemapShaderInformation() :
			p_TextureBufferName{ 0 }
		{
			/*nothing*/
		};
		int UpdateVertexData(
			IGraphicsPipeline* pPipeline,
			GLuint vertexBufferName,
			GLuint pVertexArray,
			const Vertex* pVertexBuffer,
			std::pair<unsigned int* const, Vector2Int> tilemapBuffer);
		virtual int UpdateUniformVariables(IGraphicsPipeline* pPipeline) override;
		virtual void Release() override;
	};
}