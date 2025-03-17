#pragma once
#include "GraphicsPipelineFramework.h"
#include "RenderLayer.h"
#include "Camera.h"
#include <vector>
#include <string>

namespace GraphicsPipeline
{
	// TODO: Rename this to GraphicsPipeline2D
	// A generic pipeline that allows the easy creation and dynamic use of shaders
	// Uses the GLFW3 and GLEW libraries.
	class GraphicsPipeline2D : IGraphicsPipeline
	{
	private:
		std::vector<Shaders::IShader*> m_AllShaderObjects;

		std::vector<RenderLayer> m_Layers;

		GLuint m_VertexBufferName;
		// The vertex array that contains vertices to draw with glDrawArrays()
		GLuint mp_VertexArray;

		// Compiles the collection of shaders
		/// <returns> The count of shaders that failed to compile </returns>
		virtual int CompileShaders() override;
		/// <returns> The count of shaders that failed to create programs </returns>
		virtual int CreatePrograms() override;
	public:
		Camera ActiveCamera;

		inline GraphicsPipeline2D() : m_VertexBufferName(0), mp_VertexArray(0), m_Layers({}), ActiveCamera({}), m_AllShaderObjects({})
		{
			/*nothing*/
		}

		void InsertLayer(int index, RenderLayer& rLayer);
		RenderLayer& GetLayer(int index);
		int GetLayersCount();

		// Loads the data in a file and iterprets it as text
		static std::string LoadShaderCodeFromFile(const char* fullFilePath);

		// Use this to add shaders that need to be compiled
		void RegisterShader(Shaders::IShader* pShader);
		
		template<typename TYPE>
		inline TYPE* TryGetShader(int id) 
		{
			if (id >= m_AllShaderObjects.size())
				return nullptr;
			Shaders::IShader* pShader = m_AllShaderObjects[id];
			return (TYPE*)pShader;
		}

		// Note: add shaders before calling this function because they are only compiled after this point
		virtual void Initialize() override;

		// Go through all the meshes in each render layer and draw the vertex data through their shaders.
		// Assumes the shaders all inherit  Shader
		virtual void RenderScene() override;

		virtual void Release() override;
	};
}