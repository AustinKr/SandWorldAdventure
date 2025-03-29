#include "HeaderFiles/GraphicsPipeline/Meshes/TilemapMesh.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h" // Needed for IMesh.h

#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"

namespace SandboxEngine::GraphicsPipeline
{
	TilemapMesh::TilemapMesh(Game::GameObject::IGameObject* pTilemap) : mp_Tilemap(pTilemap)
	{
		/*nothing*/
	}

	void TilemapMesh::Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray)
	{

	}

	void TilemapMesh::Release()
	{
		((Game::GameObject::Tilemap::Tilemap*)mp_Tilemap)->p_Mesh = nullptr;
		delete(this);
	}
}