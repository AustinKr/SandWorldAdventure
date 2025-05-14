#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "HeaderFiles/Math.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct BaseShaderInformation
	{
	public:
		// position  member of Vertex
		GLint p_Position;
		// uvCoord  member of Vertex
		GLint p_UVPosition;

		// The program
		GLuint p_Program;

		virtual void Release() = 0;
		// Used after the program is linked
		virtual int UpdateUniformVariables(IGraphicsPipeline* pPipeline) = 0;
	};
}