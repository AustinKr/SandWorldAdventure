#include "HeaderFiles/GraphicsPipeline/Shaders/ShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	void ShaderInformation::SetShadeColor(IGraphicsPipeline* pPipeline, float3 color)
	{
		glUniform3f(((GraphicsPipeline2D*)pPipeline)->GetUniformLocation(ShadeColorName), color.X, color.Y, color.Z);
	}

	int ShaderInformation::UpdateVertexData(IGraphicsPipeline* pPipeline, GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, int count)
	{
		int returnCode = IGraphicsPipeline::GP_SUCCESS;

		// Set gl buffer data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), pVertexBuffer, GL_STATIC_DRAW);

		glBindVertexArray(pVertexArray);

		// Get the variables we know are contained in the now-linked program

		p_Position = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vPos", 2, GL_FLOAT, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
		p_UVPosition = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vUvCoord", 3, GL_FLOAT, sizeof(Vertex), (const void*)offsetof(Vertex, uvCoord));
		UpdateUniformVariables(pPipeline);

		return returnCode;
	}

	int ShaderInformation::UpdateUniformVariables(IGraphicsPipeline* pPipeline)
	{
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, ShadeColorName);
		return IGraphicsPipeline::GP_SUCCESS;
	}
}