#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "HeaderFiles/Math.h"
#include "IShaderInformation.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct ShaderInformation : public IShaderInformation
	{
	public:
		// Generates(if necessary) and sets the data of vertex buffers and arrays, and enables the attributes.
		int UpdateVertexData(IGraphicsPipeline* pPipeline, GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, int count);
	};
}