#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/GraphicsPipeline/ShaderTypes/ShaderType.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/ShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline
{
	void Mesh::TransformVertexCoordinates(GraphicsPipeline::GraphicsPipeline2D* pPipeline, Vertex buffer[3])
	{
		if (UseWorldCoordinates)
		{
			buffer[0].pos = pPipeline->ActiveCamera.WorldToViewport(buffer[0].pos * Scale + Origin) * 2.0 - Vector2(1, 1);
			buffer[1].pos = pPipeline->ActiveCamera.WorldToViewport(buffer[1].pos * Scale + Origin) * 2.0 - Vector2(1, 1);
			buffer[2].pos = pPipeline->ActiveCamera.WorldToViewport(buffer[2].pos * Scale + Origin) * 2.0 - Vector2(1, 1);
			return;
		}
		buffer[0].pos = pPipeline->ActiveCamera.ScreenToViewport(buffer[0].pos * Scale + Origin) * 2.0 - Vector2(1, 1);
		buffer[1].pos = pPipeline->ActiveCamera.ScreenToViewport(buffer[1].pos * Scale + Origin) * 2.0 - Vector2(1, 1);
		buffer[2].pos = pPipeline->ActiveCamera.ScreenToViewport(buffer[2].pos * Scale + Origin) * 2.0 - Vector2(1, 1);
	}

	Mesh::Mesh(bool useWorldCoordinates) : Origin{}, Scale(1,1), Vertices{}, Triangles{}, Shaders{}, UseWorldCoordinates(useWorldCoordinates), Texture{}, color{1,1,1,1}
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
				// Apply scaling a transformation from world or screen to viewport to open gl coordinates where (-1, -1) is the bottom left corner of the screen and (1, 1) is the top right.

				// Prepare vertex buffer
				vertexBuffer[0] = Vertices[Triangles[triangle]];
				vertexBuffer[1] = Vertices[Triangles[triangle + 1]];
				vertexBuffer[2] = Vertices[Triangles[triangle + 2]];
				TransformVertexCoordinates(pPipeline, vertexBuffer);

				ShaderTypes::ShaderType* pShader = pPipeline->TryGetShader<ShaderTypes::ShaderType>(Shaders[s * 3 + 2]); // Get the shader
				((Shaders::ShaderInformation*)pShader->p_ShaderInformation)->UpdateVertexData(pPipeline, this, vertexBufferName, pVertexArray, vertexBuffer, 3);
				
				glUseProgram(pShader->GetProgram()); // Set the program

				// Draw triangles
				glDrawArrays(GL_TRIANGLES, 0, 3);
			}
		}
	}

	void Mesh::Release()
	{
		if (Texture.p_Colors != nullptr && Texture.ColorsSize != 0)
			delete[](Texture.p_Colors);
		delete(this);
	}
}