#include "HeaderFiles/GraphicsPipeline/Shaders/TilemapShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	int TilemapShaderInformation::UpdateVertexData(IGraphicsPipeline* pPipeline, GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, Game::GameObject::Tilemap::StaticQuadtree* pQuadtree)
	{
		int returnCode = IGraphicsPipeline::GP_SUCCESS;

		//int myArray[4] = { 0,1,2,3 };
		//
		//// Create the tilemap quadtree buffer
		//glGenBuffers(1, &mp_QuadtreeBufferName);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, mp_QuadtreeBufferName);
		//glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(myArray)/*sizeof(*pQuadtree)*/, myArray/*pQuadtree*/, GL_STREAM_DRAW); // Mutable buffer storage
		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, mp_QuadtreeBufferName);
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
		//

		// glBufferData deletes any pre-existing data
		// Set gl vertex buffer data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), pVertexBuffer, GL_STREAM_DRAW); // Mutable buffer storage
		glBindVertexArray(pVertexArray);

		// Get the variables we know are contained in the now-linked program
		p_Position = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vPos", 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		p_UVPosition = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vUvCoord", 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uvCoord));

		p_UniformTime = glGetUniformLocation(p_Program, "Time"); // optional
		p_UniformTilemapOrigin = glGetUniformLocation(p_Program, "TilemapOrigin");
		p_UniformTilemapBounds = glGetUniformLocation(p_Program, "TilemapBounds");
		p_UniformTilemapWorldSize = glGetUniformLocation(p_Program, "TilemapWorldSize");

		if (p_UniformTilemapOrigin == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader p_UniformTilemapOrigin variable!\n");
		}
		if (p_UniformTilemapBounds == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader p_UniformTilemapBounds variable!\n");
		}
		if (p_UniformTilemapWorldSize == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader p_UniformTilemapWorldSize variable!\n");
		}

		return returnCode;
	}
}