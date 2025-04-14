#include "HeaderFiles/GraphicsPipeline/Shaders/TilemapShader.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	TilemapShader::TilemapShader() : mp_VertexShader(0), mp_FragmentShader(0), mp_Program(0), mp_Position(0), mp_UVPosition(0), 
				mp_QuadtreeBufferName(0),	p_UniformTime(0), p_UniformTilemapBounds(0), p_UniformTilemapOrigin(0), p_UniformTilemapWorldSize(0)
	{
		/*nothing*/
	}

	int TilemapShader::Compile(IGraphicsPipeline* pPipeline)
	{
		// TODO: Might want to check for errors throughout

		// Load shaders' code
		std::string vertexShaderCode = GraphicsPipeline2D::LoadShaderCodeFromFile(std::string(PROJECT_DIRECTORY).append("Shaders/Vertex/TilemapVert.glsl").c_str());
		const char* vertexShaderCodeChar = vertexShaderCode.c_str();
		std::string fragmentShaderCode = GraphicsPipeline2D::LoadShaderCodeFromFile(std::string(PROJECT_DIRECTORY).append("Shaders/Fragment/TilemapFrag.glsl").c_str());
		const char* fragmentShaderCodeChar = fragmentShaderCode.c_str();

		// The vertex shader
		mp_VertexShader = glCreateShader(GL_VERTEX_SHADER);
		if (mp_VertexShader == 0)
			return -1;
		glShaderSource(mp_VertexShader, 1, &vertexShaderCodeChar, NULL);

		// The fragment shader
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
		
		int returnCode = -1;
		if (compileStatusVert == GL_FALSE && compileStatusFrag == GL_FALSE)
			returnCode = 3;
		else if (compileStatusVert == GL_FALSE)
			returnCode = 1;
		else if (compileStatusFrag == GL_FALSE)
			returnCode = 2;
		
		if(returnCode != -1)
		{
			// Get the lengths
			GLint vertexInfoLogLength = 0, fragmentInfoLogLength = 0;
			glGetShaderiv(mp_VertexShader, GL_INFO_LOG_LENGTH, &vertexInfoLogLength);
			glGetShaderiv(mp_FragmentShader, GL_INFO_LOG_LENGTH, &fragmentInfoLogLength);

			// Get the strings and print
			GLchar* pVertexInfoLog = NULL, * pFragmentInfoLog = NULL;
			if (vertexInfoLogLength != 0)
			{
				pVertexInfoLog = (GLchar*)malloc(vertexInfoLogLength * sizeof(GLchar));
				glGetShaderInfoLog(mp_VertexShader, vertexInfoLogLength, NULL, pVertexInfoLog);
				if (pVertexInfoLog != NULL)
					fprintf(stderr, pVertexInfoLog);
				fprintf(stderr, "\n");
				free(pVertexInfoLog);
			}
			if (fragmentInfoLogLength != 0)
			{
				pFragmentInfoLog = (GLchar*)malloc(fragmentInfoLogLength * sizeof(GLchar));
				glGetShaderInfoLog(mp_VertexShader, fragmentInfoLogLength, NULL, pFragmentInfoLog);
				if (pFragmentInfoLog != NULL)
					fprintf(stderr, pFragmentInfoLog);
				fprintf(stderr, "\n");
				free(pFragmentInfoLog);
			}
			return returnCode;
		}
		return IGraphicsPipeline::GP_SUCCESS;
	}
	int TilemapShader::CreateProgram(IGraphicsPipeline* pPipeline)
	{
		// Create the program
		mp_Program = glCreateProgram();
		// Link the shaders
		glAttachShader(mp_Program, mp_VertexShader);
		glAttachShader(mp_Program, mp_FragmentShader);
		
		glLinkProgram(mp_Program);

		return IGraphicsPipeline::GP_SUCCESS;
	}
	void TilemapShader::Release()
	{
		glDetachShader(mp_Program, mp_FragmentShader);
		glDetachShader(mp_Program, mp_VertexShader);
		delete(this);

		glDeleteProgram(mp_Program);
		mp_Program = 0;
	}

	GLuint TilemapShader::GetProgram()
	{
		return mp_Program;
	}

	std::string TilemapShader::GetName()
	{
		return "TilemapShader";
	}

	int TilemapShader::UpdateVertexData(GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, Game::GameObject::Tilemap::StaticQuadtree* pQuadtree)
	{
		int returnCode = IGraphicsPipeline::GP_SUCCESS;

		int myArray[4] = { 0,1,2,3 };
		
		// Create the tilemap quadtree buffer
		glGenBuffers(1, &mp_QuadtreeBufferName);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, mp_QuadtreeBufferName);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(myArray)/*sizeof(*pQuadtree)*/, myArray/*pQuadtree*/, GL_STREAM_DRAW); // Mutable buffer storage
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, mp_QuadtreeBufferName);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0); // unbind
		
		// glBufferData deletes any pre-existing data
		// Set gl vertex buffer data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), pVertexBuffer, GL_STREAM_DRAW); // Mutable buffer storage
		glBindVertexArray(pVertexArray);

		// Get the variables we know are contained in the now-linked program
		mp_Position = glGetAttribLocation(mp_Program, "vPos");
		mp_UVPosition = glGetAttribLocation(mp_Program, "vUVCoord");
		// TODO: Probably remove time
		p_UniformTime = glGetUniformLocation(mp_Program, "Time"); // optional
		p_UniformTilemapOrigin = glGetUniformLocation(mp_Program, "TilemapOrigin");
		p_UniformTilemapBounds = glGetUniformLocation(mp_Program, "TilemapBounds");
		p_UniformTilemapWorldSize = glGetUniformLocation(mp_Program, "TilemapWorldSize");
		
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

		if (mp_Position == -1)
		{
			returnCode = -1;
			fprintf(stderr, "failed to find location of shader mp_Position variable!\n");
		}
		else
		{
			glEnableVertexAttribArray(mp_Position); // Enable the attribute to be accessed and used for rendering
			glVertexAttribPointer(mp_Position, 2, GL_FLOAT, GL_FALSE,
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
}