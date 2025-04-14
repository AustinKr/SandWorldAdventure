#pragma once
#include "HeaderFiles/Math.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include <vector>
#include <string>

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct IShader
	{
	public:
		const char* PROJECT_DIRECTORY = "C:/dev/C++ Projects/SandWorldAdventure/SandWorldAdventure/SandWorldAdventure/";

		// Creates and compiles shaders from shader code
		virtual int Compile(IGraphicsPipeline* pPipeline) = 0;
		// Creates the program, attaches the shaders to it, and defines variables used in the shaders
		virtual int CreateProgram(IGraphicsPipeline* pPipeline) = 0;
		
		//virtual int UpdateVertexData(GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, int count) = 0;
		// Deletes this object (and any other associated that should also be deleted). Also deattach from the program.
		virtual void Release() = 0;

		virtual GLuint GetProgram() = 0;
		virtual std::string GetName() = 0;
	};
}