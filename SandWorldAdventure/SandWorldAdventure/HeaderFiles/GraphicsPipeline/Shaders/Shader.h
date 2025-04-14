#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "IShader.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct Shader : IShader
	{
	private:
		GLuint mp_VertexShader;
		GLuint mp_FragmentShader;

		GLuint mp_Program;

		// position  member of Vertex
		GLint mp_Position;
		// uvCoord  member of Vertex
		GLint mp_UVPosition;
	public:
		// The global time variable
		GLint p_UniformTime;

		Shader();

		virtual int Compile(IGraphicsPipeline* pPipeline) override;
		virtual int CreateProgram(IGraphicsPipeline* pPipeline) override;
		// Generates(if necessary) and sets the data of vertex buffers and arrays, and enables the attributes.
		int UpdateVertexData(GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, int count);
		virtual void Release() override;

		virtual GLuint GetProgram() override;
		virtual std::string GetName() override;
	};
}