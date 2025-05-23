#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/GraphicsPipeline/ShaderTypes/ShaderType.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/ShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline
{
	Mesh::Mesh() : Origin({}), Scale({ 1,1 }), Vertices({}), Triangles({}), Shaders{}
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
				// Apply scaling a transformation from world to viewport to open gl coordinates where (-1, -1) is the bottom left corner of the screen and (1, 1) is the top right.

				vertexBuffer[0] = Vertices[Triangles[triangle]];
				vertexBuffer[0].pos = pPipeline->ActiveCamera.WorldToViewport(vertexBuffer[0].pos * Scale + Origin) * 2.0 - Vector2(1, 1);

				vertexBuffer[1] = Vertices[Triangles[triangle + 1]];
				vertexBuffer[1].pos = pPipeline->ActiveCamera.WorldToViewport(vertexBuffer[1].pos * Scale + Origin) * 2.0 - Vector2(1, 1);

				vertexBuffer[2] = Vertices[Triangles[triangle + 2]];
				vertexBuffer[2].pos = pPipeline->ActiveCamera.WorldToViewport(vertexBuffer[2].pos * Scale + Origin) * 2.0 - Vector2(1, 1);

				ShaderTypes::ShaderType* pShader = pPipeline->TryGetShader<ShaderTypes::ShaderType>(Shaders[s * 3 + 2]); // Get the shader
				((Shaders::ShaderInformation*)pShader->p_ShaderInformation)->UpdateVertexData(pPipeline, vertexBufferName, pVertexArray, vertexBuffer, 3);
				
				glUseProgram(pShader->GetProgram()); // Set the program

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