#pragma once
#include "HeaderFiles/GraphicsPipeline/GraphicsPipelineFramework.h"
#include "IShader.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/StaticQuadtreeTypes.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct TilemapShader : IShader
	{
	private:
		GLuint mp_VertexShader;
		GLuint mp_FragmentShader;

		GLuint mp_Program;

		// Variables in shader

		// position  member of Vertex
		GLint mp_Position;
		// uvCoord  member of Vertex
		GLint mp_UVPosition;

		GLuint mp_QuadtreeBufferName;
	public:
		//TODO: Probably wont use time!
		GLint p_UniformTime; // Global time variable
		GLint p_UniformTilemapOrigin;
		GLint p_UniformTilemapBounds;
		GLint p_UniformTilemapWorldSize;

		TilemapShader();

		virtual int Compile(IGraphicsPipeline* pPipeline) override;
		virtual int CreateProgram(IGraphicsPipeline* pPipeline) override;
		virtual void Release() override;

		virtual GLuint GetProgram() override;
		virtual std::string GetName() override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertexBufferName"></param>
		/// <param name="pVertexArray"></param>
		/// <param name="pVertexBuffer"<Assumed to contain 4 vertices</param>
		/// <param name=""></param>
		/// <returns></returns>
		int UpdateVertexData(GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, Game::GameObject::Tilemap::StaticQuadtree* pQuadtree);
	};
}