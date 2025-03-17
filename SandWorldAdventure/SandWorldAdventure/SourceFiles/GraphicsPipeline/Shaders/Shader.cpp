#include "HeaderFiles/GraphicsPipeline/Shaders/Shader.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace GraphicsPipeline::Shaders
{
	Shader::Shader() : mp_VertexShader(0), mp_FragmentShader(0), mp_Program(0), mp_VPosition(0), /*mp_VColor(0),*/ p_UniformTime(0)
	{
		/*nothing*/
	}

	int Shader::Compile(IGraphicsPipeline* pPipeline)
	{
		// TODO: Might want to check for errors throughout
		
		// Load shaders' code
		std::string vertexShaderCode = GraphicsPipeline2D::LoadShaderCodeFromFile(std::string(PROJECT_DIRECTORY).append("src/GraphicsPipeline/Shaders/Vertex/ShaderVert.glsl").c_str());
		const char* vertexShaderCodeChar = vertexShaderCode.c_str();
		std::string fragmentShaderCode = GraphicsPipeline2D::LoadShaderCodeFromFile(std::string(PROJECT_DIRECTORY).append("src/GraphicsPipeline/Shaders/Fragment/ShaderFrag.glsl").c_str());
		const char* fragmentShaderCodeChar = fragmentShaderCode.c_str();

		// The default vertex shader
		mp_VertexShader = glCreateShader(GL_VERTEX_SHADER);
		if (mp_VertexShader == 0)
			return -1;
		glShaderSource(mp_VertexShader, 1, &vertexShaderCodeChar, NULL);
		
		// The default fragment shader
		mp_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		if (mp_FragmentShader == 0)
			return -1;
		glShaderSource(mp_FragmentShader, 1, &fragmentShaderCodeChar, NULL);

		// Compile the shaders
		glCompileShader(mp_VertexShader);
		glCompileShader(mp_FragmentShader);

		// Check for compile errors
		GLint compileStatusVert = 0, compileStatusFrag = 0;
		glGetShaderiv(mp_VertexShader, GL_COMPILE_STATUS, &compileStatusVert);
		glGetShaderiv(mp_FragmentShader, GL_COMPILE_STATUS, &compileStatusFrag);
		if (compileStatusVert == GL_FALSE || compileStatusFrag == GL_FALSE)
		{
			// Get the lengths
			GLint vertexInfoLogLength = 0, fragmentInfoLogLength = 0;
			glGetShaderiv(mp_VertexShader, GL_INFO_LOG_LENGTH, &vertexInfoLogLength);
			glGetShaderiv(mp_FragmentShader, GL_INFO_LOG_LENGTH, &fragmentInfoLogLength);

			// Get the strings and print
			GLchar *pVertexInfoLog = NULL, *pFragmentInfoLog = NULL;
			if (vertexInfoLogLength != 0)
			{
				pVertexInfoLog = (GLchar*)malloc(vertexInfoLogLength * sizeof(GLchar));
				glGetShaderInfoLog(mp_VertexShader, vertexInfoLogLength, NULL, pVertexInfoLog);
				if (pVertexInfoLog != NULL)
					fprintf(stderr, pVertexInfoLog);
				fprintf(stderr, "\n");
			}
			if (fragmentInfoLogLength != 0)
			{
				pFragmentInfoLog = (GLchar*)malloc(fragmentInfoLogLength * sizeof(GLchar));
				glGetShaderInfoLog(mp_VertexShader, fragmentInfoLogLength, NULL, pFragmentInfoLog);
				if (pFragmentInfoLog != NULL)
					fprintf(stderr, pFragmentInfoLog);
				fprintf(stderr, "\n");
			}
			return -1;
		}
		return IGraphicsPipeline::GP_SUCCESS;
	}
	int Shader::CreateProgram(IGraphicsPipeline* pPipeline)
	{
		// Create the program
		mp_Program = glCreateProgram();
		// Link the shaders
		glAttachShader(mp_Program, mp_VertexShader);
		glAttachShader(mp_Program, mp_FragmentShader);
		// ?
		glLinkProgram(mp_Program);

		return IGraphicsPipeline::GP_SUCCESS;
	}
	void Shader::Release()
	{
		glDetachShader(mp_Program, mp_FragmentShader);
		glDetachShader(mp_Program, mp_VertexShader);
		delete(this);

		glDeleteProgram(mp_Program);
		mp_Program = 0;
	}
	int Shader::UpdateVertexData(GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, int count)
	{
		int returnCode = IGraphicsPipeline::GP_SUCCESS;

		// Set gl buffer data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(Vertex), pVertexBuffer, GL_STATIC_DRAW);
		
		glBindVertexArray(pVertexArray);

		// Get the variables we know are contained in the now-linked program
		mp_VPosition = glGetAttribLocation(mp_Program, "vPos");
		mp_UVPosition = glGetAttribLocation(mp_Program, "vUVCoord");
		p_UniformTime = glGetUniformLocation(mp_Program, "Time");
		/*if (p_UniformTime == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader p_UniformTime variable!\n");
		}*/

		if (mp_VPosition == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader mp_VPosition variable!\n");
		}
		else
		{
			glEnableVertexAttribArray(mp_VPosition); // Enable the attribute to be accessed and used for rendering
			glVertexAttribPointer(mp_VPosition, 2, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (void*)offsetof(Vertex, pos));
		}
		if (mp_UVPosition == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader mp_UVPosition variable!\n");
		}
		else
		{
			glEnableVertexAttribArray(mp_UVPosition); // Enable the attribute to be accessed and used for rendering
			glVertexAttribPointer(mp_UVPosition, 3, GL_FLOAT, GL_FALSE,
				sizeof(Vertex), (void*)offsetof(Vertex, uvCoord));
		}
		return returnCode;
	}

	GLuint Shader::GetProgram()
	{
		return mp_Program;
	}

	std::string Shader::GetName()
	{
		return "Shader";
	}
}