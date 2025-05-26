#pragma once

//#define _PRINT_MISSING_GLSL_LOCATIONS // Used in graphics pipeline2D

#include "GraphicsPipelineFramework.h"
#include "RenderLayer.h"
#include "Camera.h"
#include <vector>
#include <string>
#include <unordered_map>

#define GRAPHICS_PIPELINE2D_H
namespace SandboxEngine::GraphicsPipeline
{
	// A generic pipeline that allows the easy creation and dynamic use of shaders
	// Stores all meshes and shaders.
	// Uses the GLFW3 and GLEW libraries.
	class GraphicsPipeline2D : public IGraphicsPipeline
	{
	private:
		typedef unsigned int UINT;

		std::vector<ShaderTypes::IShaderType*> m_AllShaderObjects;
		std::vector<RenderLayer> m_Layers;
		std::unordered_map<std::string, GLint> m_UniformVariableLocations;

		GLuint m_VertexBufferName;
		// The vertex array that contains vertices to draw with glDrawArrays()
		GLuint mp_VertexArray;

		// Compiles the collection of shaders
		/// <returns> The count of shaders that failed to compile </returns>
		virtual int CompileShaders() override;
	public:
		enum
		{
			GP2D_BASE_SHADER = 0,
			GP2D_TILEMAP_SHADER = 1,
			GP2D_PLAYER_SHADER = 2,
		};

		Camera ActiveCamera;

		inline GraphicsPipeline2D() : m_VertexBufferName(0), mp_VertexArray(0), m_Layers{}, ActiveCamera{}, m_AllShaderObjects{}, m_UniformVariableLocations{}
		{
			/*nothing*/
		}

		void InsertLayer(int index, RenderLayer& rLayer);
		void InsertLayer(int index, RenderLayer&& rLayer);
		RenderLayer& GetLayer(int index);
		int GetLayersCount();

		// Loads the data in a file and iterprets it as text
		static std::string LoadShaderCodeFromFile(const char* fullFilePath);

		// Use this to add shaders that need to be compiled
		void RegisterShader(ShaderTypes::IShaderType* pShader);
		// Used to compile shaders
		int CompileShader(GLuint* ppShader, GLenum shaderType, const char* pShaderCode);

		template<typename TYPE>
		inline TYPE* TryGetShader(int id) 
		{
			if (id >= m_AllShaderObjects.size())
				return nullptr;
			ShaderTypes::IShaderType* pShader = m_AllShaderObjects[id];
			return (TYPE*)pShader;
		}

		void TryPrintGlError();

		/// <summary>
		/// 
		/// </summary>
		/// <returns>The location of the attribute</returns>
		GLint TryEnableVertexAttribute(GLuint program,
			const char* const pName,
			GLint size, 
			GLenum type, 
			GLsizei stride, 
			const void* pAttribute);
		int UpdateUniformLocation(int program, std::string name);
		GLint GetUniformLocation(std::string name);

		// Note: add shaders before calling this function because they are only compiled after this point
		virtual void Initialize() override;

		// Go through all the meshes in each render layer and draw the vertex data through their shaders.
		// Assumes the shaders all inherit  Shader
		virtual void RenderScene() override;

		// Releases all mesh and shader data
		virtual void Release() override;


		static inline UINT RGBA_To_UINT(int r, int g, int b, int a)
		{
			// Each channel is 8 bits.
			return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8)
				+ (a & 0xff);
		}
	};
}