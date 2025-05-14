#include "HeaderFiles/GraphicsPipeline/Shaders/TilemapShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	int TilemapShaderInformation::UpdateVertexData(
		IGraphicsPipeline* pPipeline,
		GLuint vertexBufferName,
		GLuint pVertexArray,
		const Vertex* pVertexBuffer,
		std::pair<unsigned int* const, Vector2Int> tilemapBuffer)
	{
		int returnCode = IGraphicsPipeline::GP_SUCCESS;

		// TODO: (Maybe) Use a texture and simply send a limited amount of detail in tiles instead of a quadtree because it is less data most of the time
		
		if (tilemapBuffer.first != nullptr)
		{
			// Create
			glGenTextures(1, &p_TextureBufferName);
			glBindTexture(GL_TEXTURE_2D, p_TextureBufferName);
			// Set params
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			// Set data
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tilemapBuffer.second.X, tilemapBuffer.second.Y, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, tilemapBuffer.first);
			((GraphicsPipeline2D*)pPipeline)->TryPrintGlError();
		}

		// glBufferData deletes any pre-existing data
		// Set gl vertex buffer data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), pVertexBuffer, GL_STREAM_DRAW); // Mutable buffer storage
		glBindVertexArray(pVertexArray);

		// Get the variables we know are contained in the linked program
		p_Position = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vPos", 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		p_UVPosition = ((GraphicsPipeline2D*)pPipeline)->TryEnableVertexAttribute(p_Program, "vUvCoord", 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, uvCoord));
		
		UpdateUniformVariables(pPipeline);
		
		return returnCode;
	}

	int TilemapShaderInformation::UpdateUniformVariables(IGraphicsPipeline* pPipeline)
	{
		// Update uniforms
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, TextureName);
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, TextureOriginCoordName);
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, TextureSizeCoordName);
		((GraphicsPipeline2D*)pPipeline)->UpdateUniformLocation(p_Program, TextureSizeName);
		return IGraphicsPipeline::GP_SUCCESS;
	}

	void TilemapShaderInformation::Release()
	{
		if (p_TextureBufferName == 0)
			return;
		glDeleteTextures(1, &p_TextureBufferName);
		p_TextureBufferName = 0;
	}
}