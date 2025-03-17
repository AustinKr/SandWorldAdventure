#pragma once
#include "src/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "IShader.h"

namespace GraphicsPipeline::Shaders
{
	struct Shader : IShader
	{
	private:
		GLuint mp_VertexShader;
		GLuint mp_FragmentShader;

		GLuint mp_Program;

		// position  member of Vertex
		GLint mp_VPosition;
		// uvCoord  member of Vertex
		GLint mp_UVPosition;
	public:
		// The global time variable
		GLint p_UniformTime;

		Shader();

		virtual int Compile(IGraphicsPipeline* pPipeline) override;
		virtual int CreateProgram(IGraphicsPipeline* pPipeline) override;
		virtual int UpdateVertexData(GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, int count) override;
		virtual void Release() override;

		virtual GLuint GetProgram() override;
		virtual std::string GetName() override;
	};
}