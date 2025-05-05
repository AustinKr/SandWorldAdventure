#include "HeaderFiles/GraphicsPipeline/Meshes/TilemapMesh.h"
#include "HeaderFiles/GraphicsPipeline/GraphicsPipeline2D.h" // Needed for IMesh.h
#include "HeaderFiles/GraphicsPipeline/Shaders/TilemapShaderInformation.h"
#include "HeaderFiles/GraphicsPipeline/ShaderTypes/ShaderType.h"

#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h"
#include <vector>
#include <algorithm>

namespace SandboxEngine::GraphicsPipeline
{
	using namespace Game::GameObject::Tilemap;

	const Vertex TilemapMesh::m_Verts[4] =
	{
		Vertex{{-1, -1}, {0, 0, 0}},
		Vertex{{ 1, -1}, {1, 0, 0}},
		Vertex{{ 1,  1}, {1, 1, 0}},
		Vertex{{-1,  1}, {0, 1, 0}},
	};

	StaticQuadtree TilemapMesh::CreateOnScreenTilesBuffer()
	{
		Tilemap* pTilemap = ((Tilemap*)mp_Tilemap);

		// Find the two tile positions that correspond to the bottom left and top right of the screen
		Vector2Int bottomLeft = std::clamp(pTilemap->FromWorldToTile(
											Game::GameInstance::Pipeline.ActiveCamera.ViewportToWorld(
												Vector2(0, 0))),
										  {}, (Vector2)pTilemap->Container.GetTileBounds()),
				   bottomLeftChunk = pTilemap->Container.TileToChunkCoordinates(bottomLeft);
		Vector2Int topRight = std::clamp(pTilemap->FromWorldToTile(
											Game::GameInstance::Pipeline.ActiveCamera.ViewportToWorld(
												Vector2(1, 1))),
										 {}, (Vector2)pTilemap->Container.GetTileBounds()),
				   topRightChunk = pTilemap->Container.TileToChunkCoordinates(topRight);
		Vector2Int screenSizeTiles = topRight - bottomLeft;

		
		//int tilesCount = screenSizeTiles.X * screenSizeTiles.Y;
		//TILE_DATA_TYPE* const pTiles = (TILE_DATA_TYPE*)malloc(sizeof(TILE_DATA_TYPE) * tilesCount);
		//for (int y = bottomLeftChunk.Y; y < topRightChunk.Y; y++)
		//{
		//	for (int x = bottomLeftChunk.X; x < topRightChunk.X; x++)
		//	{
		//		Chunk* pChunk = pTilemap->Container.GetChunk(x + y * pTilemap->Container.GetChunkBounds().X);
		//		if (!pChunk->Tiles.has_value())
		//			continue;

		//		for (int i = 0; i < pChunk->Tiles.value().size(); i++)
		//		{
		//			Vector2Int pos = TilemapContainer::CHUNK_SIZE * Vector2(pChunk->X, pChunk->Y) // Tile position of chunk
		//							+ Vector2Int(i / TilemapContainer::CHUNK_SIZE.X,              // Offset (in tiles) inside chunk for current tile 
		//									  i % TilemapContainer::CHUNK_SIZE.X);
		//			int tileID = pos.X + pos.Y * pTilemap->Container.GetTileBounds().X;
		//			pTiles[tileID] = pChunk->Tiles.value()[i].Color; // Assign the color
		//		}
		//	}
		//}
		
		// TODO: In the future, this would be replaced with a dynamic quadtree used to store the tiles.
		// TODO: Tile should inherit IQuadtreeData
		std::vector<std::pair<Quadtree::Vector2, LeafData*>> tileColors = {};
		for (int y = bottomLeftChunk.Y; y < topRightChunk.Y; y++)
		{
			for (int x = bottomLeftChunk.X; x < topRightChunk.X; x++)
			{
				Chunk* pChunk = pTilemap->Container.GetChunk(x + y * pTilemap->Container.GetChunkBounds().X);
				if (!pChunk->Tiles.has_value())
					continue;

				for (int i = 0; i < pChunk->Tiles.value().size(); i++)
				{
					Vector2Int pos = TilemapContainer::CHUNK_SIZE * Vector2Int(pChunk->X, pChunk->Y)
									+ Vector2Int(i / TilemapContainer::CHUNK_SIZE.X,
											  i % TilemapContainer::CHUNK_SIZE.X);
					tileColors.push_back(std::make_pair(Quadtree::Vector2(pos.X, pos.Y), new LeafData(pChunk->Tiles.value()[i].Color)));
				}
			}
		}

		return CreateQuadtree(bottomLeft, topRight, tileColors.begin(), tileColors.end());
	}

	TilemapMesh::TilemapMesh(Game::GameObject::IGameObject* pTilemap) : mp_Tilemap(pTilemap)
	{
		/*nothing*/
	}

	void TilemapMesh::Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray)
	{
		StaticQuadtree quadtree = CreateOnScreenTilesBuffer();

		ShaderTypes::ShaderType* pShader = pPipeline->TryGetShader<ShaderTypes::ShaderType>(GraphicsPipeline::GraphicsPipeline2D::GP2D_TILEMAP_SHADER); // Get the shader
		Shaders::TilemapShaderInformation* pShaderInformation = (Shaders::TilemapShaderInformation*)pShader->p_ShaderInformation;
		pShaderInformation->UpdateVertexData(pPipeline, vertexBufferName, pVertexArray, m_Verts, quadtree.p_NodesBegin, quadtree.NodeCount); // Pass in vertex buffer data and quadtree of tilemap

		Vector2Int bounds = ((Tilemap*)mp_Tilemap)->Container.GetTileBounds();
		Vector2 tilemapOriginCoord = pPipeline->ActiveCamera.WorldToViewport(((Tilemap*)mp_Tilemap)->Position); // Convert the origin to viewport
		
		glUseProgram(pShader->GetProgram()); // Set the program
		// Set Uniform variables
		glUniform2d(pShaderInformation->p_UniformTilemapOrigin, tilemapOriginCoord.X, tilemapOriginCoord.Y); // Set the tilemap origin
		glUniform2i(pShaderInformation->p_UniformTilemapBounds, bounds.X, bounds.Y); // Set the tilemap columns and rows
		glUniform2d(pShaderInformation->p_UniformRootNodeOrigin, quadtree.RootOrigin.X, quadtree.RootOrigin.Y);
		glUniform1d(pShaderInformation->p_UniformRootNodeSize, quadtree.RootSize);
		glUniform1i(pShaderInformation->p_UniformLeafDepth, quadtree.LeafDepth);

		// Draw quads
		glDrawArrays(GL_QUADS, 0, 4);

		free(quadtree.p_NodesBegin); // Free the block of memory
	}

	void TilemapMesh::Release()
	{
		((Game::GameObject::Tilemap::Tilemap*)mp_Tilemap)->p_Mesh = nullptr;
		delete(this);
	}
}