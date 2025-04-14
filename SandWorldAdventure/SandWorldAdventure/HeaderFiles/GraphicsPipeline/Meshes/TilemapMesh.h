#pragma once
#include "IMesh.h"
#include "HeaderFiles/Game/GameObjects/IGameObject.h"

#include "Quadtree.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/StaticQuadtreeTypes.h"

namespace SandboxEngine::GraphicsPipeline
{
	class TilemapMesh : public IMesh
	{
	private:
		// Note: this object will always be a tilemap- IGameObject* is only used for simplicity
		Game::GameObject::IGameObject* mp_Tilemap;
		static const Vertex m_Verts[4];

		Game::GameObject::Tilemap::StaticQuadtree&& CreateOnScreenTilesBuffer();
	public:
		TilemapMesh(Game::GameObject::IGameObject* pTilemap);

		virtual void Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray) override;
		virtual void Release() override;
	};
}