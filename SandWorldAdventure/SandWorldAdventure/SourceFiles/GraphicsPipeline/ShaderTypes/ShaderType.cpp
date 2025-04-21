#include "HeaderFiles/GraphicsPipeline/ShaderTypes/ShaderType.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline::ShaderTypes
{
	bool ShaderType::ParseCode(std::string code, std::string* pVertCode, std::string* pFragCode)
	{
		size_t startIndex = code.find("#VERTEX") + 8; // 8 characters in tag
		size_t endIndex= code.find("#ENDVERTEX");
		if (startIndex == std::string::npos || endIndex == std::string::npos)
			return false;
		*pVertCode = code.substr(startIndex, endIndex - startIndex).c_str();

		//code = code.substr(endIndex, code.length()); // Shorten the length to search for the fragment tags. Note: this would force there to be an order in which vertex comes before fragment
		startIndex = code.find("#FRAGMENT") + 10; // 10 characters in tag
		endIndex = code.find("#ENDFRAGMENT");
		if (startIndex == std::string::npos || endIndex == std::string::npos)
			return false;
		*pFragCode = code.substr(startIndex, endIndex - startIndex).c_str();

		return true;
	}

	ShaderType::ShaderType(std::string name, std::string filepath, Shaders::IShaderInformation* pShaderInformation) : m_Name(name), mp_VertexShader(0), mp_FragmentShader(0), p_ShaderInformation(pShaderInformation)
	{
		// Load shaders' code
		std::string shaderCode = GraphicsPipeline2D::LoadShaderCodeFromFile(std::string(PROJECT_DIRECTORY).append(filepath).c_str());
		
		if (!ParseCode(shaderCode, &m_VertexShaderCode, &m_FragmentShaderCode))
		{
			fprintf(stderr, std::string("Failed to parse shader code!\n").append(GetName()).c_str());
#ifdef _DEBUG
			throw new std::exception("Failed to parse shader code!");
#endif
		}
	}

	int ShaderType::Compile(IGraphicsPipeline* pPipeline)
	{
		int returnCodeA = IGraphicsPipeline::GP_SUCCESS;
		int returnCodeB = IGraphicsPipeline::GP_SUCCESS;
		// Shader code
		const char* vertexShaderCodeChar = m_VertexShaderCode.c_str();
		const char* fragmentShaderCodeChar = m_FragmentShaderCode.c_str();
		// Compile
		returnCodeA = ((GraphicsPipeline2D*)pPipeline)->CompileShader(&mp_VertexShader, vertexShaderCodeChar);
		returnCodeB = ((GraphicsPipeline2D*)pPipeline)->CompileShader(&mp_FragmentShader,fragmentShaderCodeChar);

		// Create the program
		p_ShaderInformation->p_Program = glCreateProgram();
		// Link the shaders
		glAttachShader(p_ShaderInformation->p_Program, mp_VertexShader);
		glAttachShader(p_ShaderInformation->p_Program, mp_FragmentShader);
		glLinkProgram(p_ShaderInformation->p_Program);

		return returnCodeA == -1 ? -1 : returnCodeB;
	}
	void ShaderType::Release()
	{
		delete(p_ShaderInformation);

		glDetachShader(p_ShaderInformation->p_Program, mp_FragmentShader);
		glDetachShader(p_ShaderInformation->p_Program, mp_VertexShader);
		delete(this);

		glDeleteProgram(p_ShaderInformation->p_Program);
		p_ShaderInformation->p_Program = 0;
	}
	

	GLuint ShaderType::GetProgram()
	{
		return p_ShaderInformation->p_Program;
	}

	std::string ShaderType::GetName()
	{
		return "Shader:"+ m_Name;
	}
}