#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "HeaderFiles/Math.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct IShaderInformation
	{
	public:
		// position  member of Vertex
		GLint p_Position;
		// uvCoord  member of Vertex
		GLint p_UVPosition;

		// The global time variable
		GLint p_UniformTime;

		// The program
		GLuint p_Program;
	};
}