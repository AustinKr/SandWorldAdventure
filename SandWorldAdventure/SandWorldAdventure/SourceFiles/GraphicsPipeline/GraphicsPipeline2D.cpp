#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"
#include "HeaderFiles/GraphicsPipeline/ShaderTypes/ShaderType.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/ShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/TextureShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/Shaders/TilemapShaderInformation.h"
#include <fstream>

namespace SandboxEngine::GraphicsPipeline
{
	GraphicsPipeline2D::GraphicsPipeline2D() : m_VertexBufferName(0), mp_VertexArray(0), m_Layers{}, ActiveCamera{}, m_AllShaderObjects{}, m_UniformVariableLocations{}
	{
		/*nothing*/
	}

	int GraphicsPipeline2D::CompileShaders()
	{
		int failedCount = IGraphicsPipeline::GP_SUCCESS;
		for (ShaderTypes::IShaderType* pShaderObject : m_AllShaderObjects)
		{
			if (pShaderObject->Compile(this) == IGraphicsPipeline::GP_SUCCESS)
				continue;
			fprintf(stderr, ("Shader object failed to compile: " + pShaderObject->GetName() + "\n").c_str());
			failedCount++;
		}
		return failedCount;
	}
	//int GraphicsPipeline2D::CreatePrograms()
	//{
	//	int failedCount = IGraphicsPipeline::GP_SUCCESS;
	//	// TODO: Could try to compile all shaders, and then report which ones didn't compile
	//	for (Shaders::IShader* pShaderObject : m_AllShaderObjects)
	//	{
	//		if (pShaderObject->CreateProgram(this) == IGraphicsPipeline::GP_SUCCESS)
	//			continue;
	//		fprintf(stderr, ("Shader object failed to create program: " + pShaderObject->GetName() + "\n").c_str());
	//		failedCount++;
	//	}
	//	return failedCount;
	//}

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
	/**
	Texture GraphicsPipeline2D::LoadPNGTextureFromFile(const char* fullFilePath)
	{
		
		//Header(1byte) + 4
		//ascii IDHR(1byte) + 4
		//width(4bytes)
		//height(4bytes)
		//(1byte each) bit depth, colr type, compression method, filter method, interlace method
		//


		std::ifstream inputStream(fullFilePath, std::ios::binary); // TODO: Could reuse the input stream
		if (!inputStream.is_open())
		{
			fprintf(stderr, std::string("Failed to open PNG file path: ").append(fullFilePath).c_str());
#ifdef _DEBUG
			throw std::invalid_argument("Invalid file path!");
#endif
			return {};
		}
		// Get begin, end, and size
		std::streampos begin, end;
		begin = inputStream.tellg();;
		end = inputStream.seekg(0, std::ios::end).tellg();
		int size = end - begin - 12 - 17; // Skip the header(8) + 4, and skip the idhr chunk(17)
		
		// Read
		char *pData = new char[17];
		inputStream.seekg(12, std::ios::beg);
		inputStream.read(pData, 17);
		
		unsigned short width = pData[7], height = pData[11], 
					   bitDepth = pData[12], colorType = pData[13], 
						compression = pData[14], filter = pData[15],
						interlace = pData[16]; // 13 bytes total

		for (int i = 0; i < size; i++)
		{
			fprintf(stdout, std::to_string((unsigned int)pData[i]).append(" | ").c_str());
		}
		delete[](pData);

		//TODO: Put only pixel color information in tex.Data
		inputStream.seekg(12 + 17, std::ios::beg);
		char* pPixels = new char[size];
		inputStream.read(pPixels, size);

		// Close
		inputStream.close();

		Texture tex = Texture{ pPixels, size };
		tex.Size = Vector2Int(width, height);

		for (int i = 0; i < size; i++)
		{
			fprintf(stdout, std::to_string((unsigned int)pPixels[i]).append(" | ").c_str());
		}

		return tex;
	}**/


	void GraphicsPipeline2D::RegisterShader(ShaderTypes::IShaderType* pShader)
	{
		m_AllShaderObjects.push_back(pShader);
	}
	int GraphicsPipeline2D::CompileShader(GLuint* ppShader, GLenum shaderType, const char* pShaderCode)
	{
		// Create the shader
		*ppShader = glCreateShader(shaderType);
		if (*ppShader == 0)
			return -1;
		glShaderSource(*ppShader, 1, &pShaderCode, NULL);
		glCompileShader(*ppShader);

		// Check for compile errors
		GLint compileStatus = 0;
		glGetShaderiv(*ppShader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus != GL_FALSE)
			return IGraphicsPipeline::GP_SUCCESS;

		// Get the length of the message
		GLint infoLogLength = 0;
		glGetShaderiv(*ppShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		// Get the string and print to console
		GLchar* pInfoLog = NULL;
		if (infoLogLength != 0)
		{
			pInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar));
			glGetShaderInfoLog(*ppShader, infoLogLength, NULL, pInfoLog);
			if (pInfoLog != NULL)
				fprintf(stderr, pInfoLog);
			fprintf(stderr, "\n");
		}
		TryPrintGlError();
			
		return -1;
	}

	void GraphicsPipeline2D::TryPrintGlError()
	{
		int err = glGetError();
		if (err == GL_NO_ERROR)
			return;

		switch (err)
		{
		case GL_INVALID_ENUM:
			fprintf(stderr, "error: invalid enum!\n");
			break;
		case GL_INVALID_VALUE:
			fprintf(stderr, "error: invalid value!\n");
			break;
		case GL_INVALID_OPERATION:
			fprintf(stderr, "error: invalid operation!\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			fprintf(stderr, "error: invalid framebuffer operation!\n");
			break;
		case GL_OUT_OF_MEMORY:
			fprintf(stderr, "error: out of memory!\n");
			break;
		case GL_STACK_UNDERFLOW:
			fprintf(stderr, "error: stack underflow!\n");
			break;
		case GL_STACK_OVERFLOW:
			fprintf(stderr, "error: stack overflow!\n");
			break;
		}
		TryPrintGlError();
	}

	GLint GraphicsPipeline2D::TryEnableVertexAttribute(GLuint program, const char* const pName, GLint size, GLenum type, GLsizei stride, const void* pAttribute)
	{
		GLint location = glGetAttribLocation(program, pName);

		if (location == -1)
		{
#ifdef _DEBUG
#ifdef  _PRINT_MISSING_GLSL_LOCATIONS
			fprintf(stderr, std::to_string(program).append(std::string("; failed to find location of shader attribute ").append(pName).append("\n")).c_str());
#endif
#endif
			TryPrintGlError();
			return -1;
		}
		glEnableVertexAttribArray(location); // Enable the attribute to be accessed and used for rendering
		glVertexAttribPointer(location, size, type, GL_FALSE, stride, pAttribute);
		TryPrintGlError();
		return location;
	}
	int GraphicsPipeline2D::UpdateUniformLocation(int program, std::string name)
	{
		GLint loc = glGetUniformLocation(program, name.c_str());
		if (m_UniformVariableLocations.contains(name))
			m_UniformVariableLocations.at(name) = loc;
		else
			m_UniformVariableLocations.insert(std::make_pair(name, loc));

		if (loc == -1)
		{
#ifdef _DEBUG
#ifdef  _PRINT_MISSING_GLSL_LOCATIONS
			fprintf(stderr, std::string("failed to find location of uniform shader variable: ").append(name).append("\n").c_str());
#endif
#endif
			TryPrintGlError();
			return -1;
		}
		TryPrintGlError();
		return 1;
	}
	GLint GraphicsPipeline2D::GetUniformLocation(std::string name)
	{
		if (m_UniformVariableLocations.contains(name))
			return m_UniformVariableLocations.at(name);
		return -1;
	}

	void GraphicsPipeline2D::Initialize()
	{
		// Create the vertex buffer that will be used for the shaders
		glGenBuffers(1, &m_VertexBufferName); // use bufferName = glGet(GL_ARRAY_BUFFER) to get the buffer name

		// Generate the array that will be used to draw triangles
		glGenVertexArrays(1, &mp_VertexArray);

		// - Add shaders(order dependent) -
		RegisterShader(new ShaderTypes::ShaderType("DefaultShader", "Shaders/DefaultShader.shader", new Shaders::ShaderInformation())); // GP2D_BASE_SHADER
		RegisterShader(new ShaderTypes::ShaderType("TilemapShader", "Shaders/TilemapShader.shader", new Shaders::TilemapShaderInformation())); // GP2D_TILEMAP_SHADER
		RegisterShader(new ShaderTypes::ShaderType("PlayerShader", "Shaders/PlayerShader.shader", new Shaders::ShaderInformation())); // GP2D_PLAYER_SHADER
		// TODO: Actually send texture to shader
		RegisterShader(new ShaderTypes::ShaderType("TextureShader", "Shaders/TextureShader.shader", new Shaders::TextureShaderInformation())); // GP2D_TEXTURE_SHADER

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
	}

	void GraphicsPipeline2D::RenderScene() // TODO: Print errors when information is missing!
	{
		for (auto& layer : m_Layers)
		{
			//TODO: Cameras will sort meshes based on distance

			for(auto meshIter = layer.begin(); meshIter != layer.end(); meshIter++)
			{
				TryPrintGlError();
				meshIter->second->Render(this, m_VertexBufferName, mp_VertexArray);
			}
		}
	}

	void GraphicsPipeline2D::Release()
	{
		// Release all mesh data
		for (RenderLayer& layer : m_Layers)
		{
			for (auto meshIter = layer.begin(); meshIter != layer.end(); meshIter++)
			{
				meshIter->second->Release();
			}
		}
		m_Layers.clear();

		// Release shaders
		for (ShaderTypes::IShaderType* pShaderObject : m_AllShaderObjects) // Loops through all shader objects
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