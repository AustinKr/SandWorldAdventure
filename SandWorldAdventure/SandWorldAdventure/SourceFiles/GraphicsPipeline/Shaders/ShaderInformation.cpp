#include "HeaderFiles/GraphicsPipeline/Shaders/ShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	int ShaderInformation::UpdateVertexData(IGraphicsPipeline* pPipeline, GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, int count)
	{
		int returnCode = IGraphicsPipeline::GP_SUCCESS;

		// Set gl buffer data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), pVertexBuffer, GL_STATIC_DRAW);

		glBindVertexArray(pVertexArray);

		// Get the variables we know are contained in the now-linked program

		p_Position = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vPos", 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		p_UVPosition = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vUvCoord", 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uvCoord));
		p_UniformTime = glGetUniformLocation(p_Program, "Time");
		/*if (p_UniformTime == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader p_UniformTime variable!\n");
		}*/

		return returnCode;
	}
}