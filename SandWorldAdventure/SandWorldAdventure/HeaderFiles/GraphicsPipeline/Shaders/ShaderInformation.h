#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/Math.h"
#include "BaseShaderInformation.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct ShaderInformation : public BaseShaderInformation
	{
	public:
		const char* ShadeColorName = "ShadeColor";

		// Generates(if necessary) and sets the data of vertex buffers and arrays, and enables the attributes.
		virtual int UpdateVertexData(
			IGraphicsPipeline *pPipeline,
			Mesh *pMesh, 
			GLuint vertexBufferName, 
			GLuint pVertexArray, 
			const Vertex* pVertexBuffer, 
			int count);
		virtual int UpdateUniformVariables(IGraphicsPipeline* pPipeline) override;
		inline virtual void Release() override {/*nothing*/ };
	};
}