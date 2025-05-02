#include "HeaderFiles/GraphicsPipeline/Meshes/Mesh.h"
#include "HeaderFiles/GraphicsPipeline/ShaderTypes/ShaderType.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/ShaderInformation.h"
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
				// Scale and offset
				vertexBuffer[0].pos = vertexBuffer[0].pos * Scale + Origin - float2(pPipeline->ActiveCamera.Origin);
				vertexBuffer[0].pos /= float2(pPipeline->ActiveCamera.Scale * pPipeline->ActiveCamera.WORLD_UNITS);

				vertexBuffer[1] = Vertices[Triangles[triangle + 1]];
				// Scale and offset
				vertexBuffer[1].pos = vertexBuffer[1].pos * Scale + Origin - float2(pPipeline->ActiveCamera.Origin);
				vertexBuffer[1].pos /= float2(pPipeline->ActiveCamera.Scale * pPipeline->ActiveCamera.WORLD_UNITS);

				vertexBuffer[2] = Vertices[Triangles[triangle + 2]];
				// Scale and offset
				vertexBuffer[2].pos = vertexBuffer[2].pos * Scale + Origin - float2(pPipeline->ActiveCamera.Origin);
				vertexBuffer[2].pos /= float2(pPipeline->ActiveCamera.Scale * pPipeline->ActiveCamera.WORLD_UNITS);

				ShaderTypes::ShaderType* pShader = pPipeline->TryGetShader<ShaderTypes::ShaderType>(Shaders[s * 3 + 2]); // Get the shader
				((Shaders::ShaderInformation*)pShader->p_ShaderInformation)->UpdateVertexData(pPipeline, vertexBufferName, pVertexArray, vertexBuffer, 3);
				//glUniform1f(((Shaders::ShaderInformation*)pShader->p_ShaderInformation)->p_UniformTime, glfwGetTime()); // Set the time

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