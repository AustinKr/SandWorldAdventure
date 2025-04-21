#pragma once
#include "HeaderFiles/GraphicsPipeline/Shaders/IShaderInformation.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/StaticQuadtreeTypes.h"

namespace SandboxEngine::GraphicsPipeline::Shaders
{
	struct TilemapShaderInformation : public IShaderInformation
	{
	public:
		GLuint mp_QuadtreeBufferName; // TODO: Rename this
		GLint p_UniformTilemapOrigin;
		GLint p_UniformTilemapBounds;
		GLint p_UniformTilemapWorldSize;

		int UpdateVertexData(IGraphicsPipeline* pPipeline, GLuint vertexBufferName, GLuint pVertexArray, const Vertex* pVertexBuffer, Game::GameObject::Tilemap::StaticQuadtree* pQuadtree);
	};
}