#pragma once
#include "HeaderFiles/GraphicsPipeline/Math.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include <vector>
#include <string>

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct IShader
	{
	public:
		// Creates and compiles shaders from shader code
		virtual int Compile(IGraphicsPipeline* pPipeline) = 0;
		// Creates the program, attaches the shaders to it, and defines variables used in the shaders
		virtual int CreateProgram(IGraphicsPipeline* pPipeline) = 0;
		// Generates(if necessary) and sets the data of vertex buffers and arrays, and enables the attributes.
		virtual int UpdateVertexData(GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, int count) = 0;
		// Deletes this object (and any other associated that should also be deleted). Also deattach from the program.
		virtual void Release() = 0;

		virtual GLuint GetProgram() = 0;
		virtual std::string GetName() = 0;
	};
}