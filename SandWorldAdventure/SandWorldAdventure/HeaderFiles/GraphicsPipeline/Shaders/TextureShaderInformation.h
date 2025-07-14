#pragma once
#include "ShaderInformation.h"
#include "HeaderFiles/Math.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct TextureShaderInformation : public ShaderInformation
	{
	public:
		GLuint p_TextureBufferName;
		static const char* TextureName;
		static const char* RedMaskName;
		static const char* GreenMaskName;
		static const char* BlueMaskName;
		static const char* AlphaMaskName;

		// Generates(if necessary) and sets the data of vertex buffers and arrays, and enables the attributes.
		virtual int UpdateVertexData(
			IGraphicsPipeline* pPipeline,
			Mesh* pMesh,
			GLuint vertexBufferName,
			GLuint pVertexArray,
			const Vertex* pVertexBuffer,
			int count) override;
		virtual int UpdateUniformVariables(IGraphicsPipeline* pPipeline) override;
		virtual void Release() override;
	};
}