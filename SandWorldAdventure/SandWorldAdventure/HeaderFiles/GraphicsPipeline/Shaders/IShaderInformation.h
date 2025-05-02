#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "HeaderFiles/Math.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	// TODO: Possibly rename to- more accurately- BaseShaderInformation
	struct IShaderInformation
	{
	public:
		// position  member of Vertex
		GLint p_Position;
		// uvCoord  member of Vertex
		GLint p_UVPosition;

		// The program
		GLuint p_Program;
	};
}