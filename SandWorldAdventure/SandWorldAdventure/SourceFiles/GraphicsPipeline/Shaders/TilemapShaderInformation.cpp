#include "HeaderFiles/GraphicsPipeline/Shaders/TilemapShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	int TilemapShaderInformation::UpdateVertexData(IGraphicsPipeline* pPipeline, GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, Game::GameObject::Tilemap::StaticQuadtree* const pQuadtree)
	{
		int returnCode = IGraphicsPipeline::GP_SUCCESS;

		// TODO: (Maybe) Use a texture and simply send a limited amount of detail in tiles instead of a quadtree because it is less data most of the time
		// Create the tilemap quadtree buffer
		glGenBuffers(1, &mp_QuadtreeBufferName);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mp_QuadtreeBufferName);
		// Set data
		int firstDataSize = sizeof(float) * 2 + sizeof(float) + sizeof(int),
			arrayDataSize = sizeof(Game::GameObject::Tilemap::StaticQuadtreeNode) * pQuadtree->NodeCount;
		glBufferData(GL_SHADER_STORAGE_BUFFER, firstDataSize + arrayDataSize, pQuadtree, GL_STREAM_DRAW); // Set the first block of data
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, firstDataSize, arrayDataSize, pQuadtree->p_NodesBegin); // Set the second(node array)
		// Bind to gl block
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mp_QuadtreeBufferName);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind

		// glBufferData deletes any pre-existing data
		// Set gl vertex buffer data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), pVertexBuffer, GL_STREAM_DRAW); // Mutable buffer storage
		glBindVertexArray(pVertexArray);

		// Get the variables we know are contained in the now-linked program
		p_Position = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vPos", 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		p_UVPosition = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vUvCoord", 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uvCoord));
		//TODO: Make a system to add uniform variables by one method
		p_UniformTilemapOrigin = glGetUniformLocation(p_Program, "TilemapOrigin");
		p_UniformTilemapBounds = glGetUniformLocation(p_Program, "TilemapBounds");
		p_UniformTilemapWorldSize = glGetUniformLocation(p_Program, "TilemapWorldSize");
		/*p_UnifromNodeCount = glGetUniformLocation(p_Program, "NodeCount");
		p_UniformRootNodeOrigin = glGetUniformLocation(p_Program, "RootNodeOrign");
		p_UniformRootNodeSize = glGetUniformLocation(p_Program, "RootNodeSize");
		p_UniformLeafDepth = glGetUniformLocation(p_Program, "LeafDepth");*/

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

	/*	if (p_UniformRootNodeOrigin == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader p_UniformRootNodeOrigin variable!\n");
		}if (p_UniformRootNodeSize == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader p_UniformRootNodeOrigin variable!\n");
		}*/

		return returnCode;
	}
}