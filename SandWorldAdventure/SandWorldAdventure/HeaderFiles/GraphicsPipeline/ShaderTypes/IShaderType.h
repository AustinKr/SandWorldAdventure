#pragma once
#include "HeaderFiles/Math.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include <vector>
#include <string>

namespace SandboxEngine::GraphicsPipeline::ShaderTypes
{
	struct IShaderType
	{
	public:
		const char* PROJECT_DIRECTORY = "C:/dev/C++ Projects/SandWorldAdventure/SandWorldAdventure/SandWorldAdventure/";

		// Creates and compiles shaders from shader code. Also creates a program and attaches shader to it.
		virtual int Compile(IGraphicsPipeline* pPipeline) = 0;
		
		// Deletes this object (and any other associated that should also be deleted). Also deattach from the program.
		virtual void Release() = 0;

		virtual GLuint GetProgram() = 0;
		virtual std::string GetName() = 0;
	};
}