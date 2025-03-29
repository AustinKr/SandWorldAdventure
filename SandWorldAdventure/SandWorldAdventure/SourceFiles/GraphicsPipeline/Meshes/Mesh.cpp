#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/Shader.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline
{
	Mesh::Mesh() : Origin({}), Scale({ 1,1 }), Vertices({}), Triangles({})
	{
		/*nothing*/
	}

	void Mesh::Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray)
	{
		Vertex vertexBuffer[3] = { {}, {}, {} };

		for (int s = 0; s < Shaders.size() / 3; s++) // mesh.Shaders should be a multiple of 3
		{
			for (int i = Shaders[s * 3]; i < Shaders[s * 3 + 1]; i++) // Iterate through the triangles specified by the  Shaders  collection
			{
				int triangle = i * 3; // Get the index

				// Set data into a continuous collection
				vertexBuffer[0] = Vertices[Triangles[triangle]];
				vertexBuffer[0].pos *= Scale * float2(pPipeline->ActiveCamera.Scale); // scale
				vertexBuffer[0].pos += Origin - float2(pPipeline->ActiveCamera.Origin); // offset

				vertexBuffer[1] = Vertices[Triangles[triangle + 1]];
				vertexBuffer[1].pos *= Scale * pPipeline->ActiveCamera.Scale; // scale
				vertexBuffer[1].pos += Origin - pPipeline->ActiveCamera.Origin; // offset

				vertexBuffer[2] = Vertices[Triangles[triangle + 2]];
				vertexBuffer[2].pos *= Scale * pPipeline->ActiveCamera.Scale; // scale
				vertexBuffer[2].pos += Origin - pPipeline->ActiveCamera.Origin; // offset

				Shaders::Shader* pShader = pPipeline->TryGetShader<Shaders::Shader>(Shaders[s * 3 + 2]); // Get the shader
				pShader->UpdateVertexData(vertexBufferName, pVertexArray, vertexBuffer, 3);

				glUseProgram(pShader->GetProgram()); // Set the program
				glUniform1f(pShader->p_UniformTime, glfwGetTime()); // Set the time

				// Draw triangles
				glDrawArrays(GL_TRIANGLES, 0, 3);
			}
		}
	}

	void Mesh::Release()
	{
		delete(this);
	}
}