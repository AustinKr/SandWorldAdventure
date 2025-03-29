#pragma once
#include "IMesh.h"
#include "HeaderFiles/Game/GameObjects/IGameObject.h"

#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"

namespace SandboxEngine::GraphicsPipeline
{
	class TilemapMesh : public IMesh
	{
	private:
		Game::GameObject::IGameObject* mp_Tilemap;
		
		Game::GameObject::Tilemap::Chunk* CreateOnScreenTilesBuffer();
	public:
		TilemapMesh(Game::GameObject::IGameObject* pTilemap);

		virtual void Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray) override;
		virtual void Release() override;
	};
}