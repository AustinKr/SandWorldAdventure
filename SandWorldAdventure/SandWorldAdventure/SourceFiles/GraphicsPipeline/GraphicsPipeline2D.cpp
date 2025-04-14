#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/Shader.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/TilemapShader.h"
#include <fstream>

namespace SandboxEngine::GraphicsPipeline
{
	int GraphicsPipeline2D::CompileShaders()
	{
		int failedCount = IGraphicsPipeline::GP_SUCCESS;
		for (Shaders::IShader* pShaderObject : m_AllShaderObjects)
		{
			if (pShaderObject->Compile(this) == IGraphicsPipeline::GP_SUCCESS)
				continue;
			fprintf(stderr, ("Shader object failed to compile: " + pShaderObject->GetName() + "\n").c_str());
			failedCount++;
		}
		return failedCount;
	}
	int GraphicsPipeline2D::CreatePrograms()
	{
		int failedCount = IGraphicsPipeline::GP_SUCCESS;
		// TODO: Could try to compile all shaders, and then report which ones didn't compile
		for (Shaders::IShader* pShaderObject : m_AllShaderObjects)
		{
			if (pShaderObject->CreateProgram(this) == IGraphicsPipeline::GP_SUCCESS)
				continue;
			fprintf(stderr, ("Shader object failed to create program: " + pShaderObject->GetName() + "\n").c_str());
			failedCount++;
		}
		return failedCount;
	}

	void GraphicsPipeline2D::InsertLayer(int index, RenderLayer& rLayer)
	{
		m_Layers.insert(m_Layers.begin() + index, rLayer);
	}
	void GraphicsPipeline2D::InsertLayer(int index, RenderLayer&& rLayer)
	{
		m_Layers.insert(m_Layers.begin() + index, rLayer);
	}
	RenderLayer& GraphicsPipeline2D::GetLayer(int index)
	{
		return m_Layers.at(index);
	}
	int GraphicsPipeline2D::GetLayersCount()
	{
		return m_Layers.size();
	}

	std::string GraphicsPipeline2D::LoadShaderCodeFromFile(const char* fullFilePath)
	{
		std::ifstream inputStream(fullFilePath, std::ios::binary);
		if (!inputStream.is_open())
			throw std::invalid_argument("Invalid file path!");
		// Input stream is open- in other words, a file was found

		// Compute the filesize
		std::streampos fileSize = inputStream.tellg(); // Start position
		inputStream.seekg(0, std::ios::end); // Seek to the end
		fileSize = inputStream.tellg() - fileSize;
		inputStream.seekg(0);
		if (fileSize <= 0)
			throw std::exception("File size was less than 1!");

		// Finally, read the file data
		std::string dataString(fileSize, '\0');
		inputStream.read(&(dataString[0]), sizeof(char) * fileSize);
		//	throw std::exception(("Could not read file! " + std::to_string(inputStream.gcount())).c_str());
		inputStream.close();

		return dataString;
	}

	void GraphicsPipeline2D::RegisterShader(Shaders::IShader* pShader)
	{
		m_AllShaderObjects.push_back(pShader);
	}

	void GraphicsPipeline2D::Initialize()
	{
		// Create the vertex buffer that will be used for the shaders
		glGenBuffers(1, &m_VertexBufferName); // use bufferName = glGet(GL_ARRAY_BUFFER) to get the buffer name

		// Generate the array that will be used to draw triangles
		glGenVertexArrays(1, &mp_VertexArray);

		// - Add shaders -
		RegisterShader(new Shaders::Shader()); // GP2D_BASE_SHADER
		RegisterShader(new Shaders::TilemapShader()); // GP2D_TILEMAP_SHADER
		
		// - Compile -
		int shadersErrorCode = CompileShaders();
		if (shadersErrorCode != GraphicsPipeline::IGraphicsPipeline::GP_SUCCESS)
		{
			fprintf(stderr, std::string("GraphicsPipline::CompileShaders failed!  Returned code: ").append(std::to_string(shadersErrorCode)).append("\n").c_str());
#if _DEBUG
			throw std::exception("Failed to compile shader!");
#endif
			//exit(EXIT_FAILURE);
		}
		shadersErrorCode = CreatePrograms();
		if (shadersErrorCode != GraphicsPipeline::IGraphicsPipeline::GP_SUCCESS)
		{
			fprintf(stderr, std::string("GraphicsPipline::CreateProgram() failed!  Returned code: ").append(std::to_string(shadersErrorCode)).append("\n").c_str());
#if _DEBUG
			throw std::exception("Failed to create program!");
#endif
			//exit(EXIT_FAILURE);
		}
	}

	void GraphicsPipeline2D::RenderScene() // TODO: Print errors when information is missing!
	{
		for (auto& layer : m_Layers)
		{
			//TODO: Cameras will sort meshes based on distance

			for (int m = 0; m < layer.MeshesCount(); m++)
			{
				IMesh* pMesh = layer.MeshAt(m);

				pMesh->Render(this, m_VertexBufferName, mp_VertexArray);
			}
		}
	}

	void GraphicsPipeline2D::Release()
	{
		// Release all mesh data
		for (RenderLayer& layer : m_Layers)
		{
			for (int m = 0; m < layer.MeshesCount(); m++)
			{
				layer.MeshAt(m)->Release();
			}
		}
		m_Layers.clear();

		// Release shaders
		for (Shaders::IShader* pShaderObject : m_AllShaderObjects) // Loops through all shader objects
		{
			pShaderObject->Release(); // Delete memory and Detach from program
		}
		m_AllShaderObjects.clear();

		glDeleteBuffers(1, &m_VertexBufferName); // Not really needed

		// Not really necessary
		glDeleteVertexArrays(1, &mp_VertexArray);
		mp_VertexArray = -1;
	}
}