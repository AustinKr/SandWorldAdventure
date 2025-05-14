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

	std::pair<TilemapMesh::TILE_DATA_TYPE* const, std::pair<Vector2Int, Vector2Int>> TilemapMesh::CreateOnScreenTilesBuffer()
	{
		Tilemap *pTilemap = ((Tilemap*)mp_Tilemap);
		GraphicsPipeline::Camera *const pCamera= &Game::GameInstance::Pipeline.ActiveCamera;

		// Find the two tile positions that correspond to the bottom left and top right of the screen
		Vector2Int bottomLeft = std::clamp(pTilemap->FromWorldToTile(
											pCamera->ViewportToWorld(
												Vector2(0, 0))),
										  {}, (Vector2)pTilemap->Container.GetTileBounds() - Vector2(1,1));//,
				   //bottomLeftChunk = pTilemap->Container.TileToChunkCoordinates(bottomLeft);
		Vector2Int topRight = std::clamp(pTilemap->FromWorldToTile(
											pCamera->ViewportToWorld(
												Vector2(1, 1))),
										 {}, (Vector2)pTilemap->Container.GetTileBounds() - Vector2(1, 1));//,
				   //topRightChunk = pTilemap->Container.TileToChunkCoordinates(topRight);
		
		// If either bounding axis converges with its opposite, return a null bitmap and don't draw anything
		if (bottomLeft.X == topRight.X || bottomLeft.Y == topRight.Y)
			return std::make_pair((TILE_DATA_TYPE* const)nullptr, std::make_pair(Vector2Int(0,0), Vector2Int(0, 0)));

		// Width and height of texture
		Vector2Int textureSize = topRight - bottomLeft + Vector2Int(1,1); // This value should never be 0 or greater than the size of the tilemap
		
		// Buffer variables
		int tilesCount = textureSize.X * textureSize.Y;
		TILE_DATA_TYPE* const pTiles = (TILE_DATA_TYPE*)malloc(sizeof(TILE_DATA_TYPE) * tilesCount);
		// Variables used inside loops
		TILE_DATA_TYPE pixelColor;
		TilemapContainer::TILE_INFO tileInfo;
		for (int px = 0; px < textureSize.X; px++)
		{
			for (int py = 0; py < textureSize.Y; py++)
			{
				tileInfo = pTilemap->Container.GetTileInChunk(bottomLeft + Vector2(px, py));
				// Get the pixel color
				pixelColor = 0xff00ffff;
				if (tileInfo.second != nullptr)
					pixelColor = tileInfo.second->Color;
				// Assign the pixel color
				pTiles[px + py * textureSize.X] = pixelColor;
			}
		}
		return std::make_pair(pTiles, std::make_pair(bottomLeft, textureSize));
		





		//int tilesCount = pCamera->ScreenSize.X * pCamera->ScreenSize.Y;
		//TILE_DATA_TYPE* const pTiles = (TILE_DATA_TYPE*)malloc(sizeof(TILE_DATA_TYPE) * tilesCount);
		//for (int x = 0; x < pCamera->ScreenSize.X; x++)
		//{
		//	for (int y = 0; y < pCamera->ScreenSize.Y; y++)
		//	{
		//		Vector2Int tilePos = pTilemap->FromWorldToTile(pCamera->ViewportToWorld(pCamera->ScreenToViewport(Vector2(x, y))));
		//		std::pair<Chunk*, Tile*> tileInfo = pTilemap->Container.GetTileInChunk(tilePos);
		//		TILE_DATA_TYPE color = 0x0;
		//		if (tileInfo.second != nullptr)
		//			color = tileInfo.second->Color;
		//		pTiles[x + y * pCamera->ScreenSize.X] = color;
		//	}
		//}
		//return std::make_pair(pTiles, std::make_pair(Vector2Int(0, 0), pCamera->ScreenSize));


		//// TODO: In the future, this would be replaced with a dynamic quadtree used to store the tiles.
		//// TODO: Tile should inherit IQuadtreeData
		//std::vector<std::pair<Quadtree::Vector2, LeafData*>> tileColors = {};
		//for (int y = bottomLeftChunk.Y; y < topRightChunk.Y; y++)
		//{
		//	for (int x = bottomLeftChunk.X; x < topRightChunk.X; x++)
		//	{
		//		Chunk* pChunk = pTilemap->Container.GetChunk(x + y * pTilemap->Container.GetChunkBounds().X);
		//		if (!pChunk->Tiles.has_value())
		//			continue;

		//		for (int i = 0; i < pChunk->Tiles.value().size(); i++)
		//		{
		//			Vector2Int pos = TilemapContainer::CHUNK_SIZE * Vector2Int(pChunk->X, pChunk->Y)
		//							+ Vector2Int(i / TilemapContainer::CHUNK_SIZE.X,
		//									  i % TilemapContainer::CHUNK_SIZE.X);
		//			tileColors.push_back(std::make_pair(Quadtree::Vector2(pos.X, pos.Y), new LeafData(pChunk->Tiles.value()[i].Color)));
		//		}
		//	}
		//}

		//return CreateQuadtree(bottomLeft, topRight, tileColors.begin(), tileColors.end());
	}

	TilemapMesh::TilemapMesh(Game::GameObject::IGameObject* pTilemap) : mp_Tilemap(pTilemap)
	{
		/*nothing*/
	}

	void TilemapMesh::Render(GraphicsPipeline::GraphicsPipeline2D* pPipeline, GLuint vertexBufferName, GLuint pVertexArray)
	{
		//StaticQuadtree quadtree = CreateOnScreenTilesBuffer();
		auto buffer = CreateOnScreenTilesBuffer();

		// Get the shader and its information which allows us to communicate with specific glsl code
		ShaderTypes::ShaderType* pShader = pPipeline->TryGetShader<ShaderTypes::ShaderType>(GraphicsPipeline::GraphicsPipeline2D::GP2D_TILEMAP_SHADER);
		Shaders::TilemapShaderInformation* pShaderInformation = (Shaders::TilemapShaderInformation*)pShader->p_ShaderInformation;
		// Pass in vertex buffer data and buffer of tiles(it will handle null bitmaps)
		pShaderInformation->UpdateVertexData(pPipeline, vertexBufferName, pVertexArray, m_Verts, std::make_pair(buffer.first, buffer.second.second));

		// Calculate the texture coords
		Vector2 tilemapTextureOriginCoord = Vector2(0, 0);
		Vector2 tilemapTextureSizeCoord = Vector2(0, 0);
		if (buffer.first != nullptr)
		{
			tilemapTextureOriginCoord =
				pPipeline->ActiveCamera.WorldToViewport(
					((Tilemap*)mp_Tilemap)->FromTileToWorld(buffer.second.first)); // Convert the origin to viewport
			tilemapTextureSizeCoord =
				pPipeline->ActiveCamera.WorldToViewport(
					((Tilemap*)mp_Tilemap)->FromTileToWorld(buffer.second.second) - ((Tilemap*)mp_Tilemap)->Position + pPipeline->ActiveCamera.Origin) 
				- Vector2(.5,.5);
		}

		glUseProgram(pShader->GetProgram()); // Set the program
		// Set Uniform variables
		glUniform2f(pPipeline->GetUniformLocation(pShaderInformation->TextureOriginCoordName), tilemapTextureOriginCoord.X, tilemapTextureOriginCoord.Y); // Set the tilemap tex origin
		glUniform2f(pPipeline->GetUniformLocation(pShaderInformation->TextureSizeCoordName), tilemapTextureSizeCoord.X, tilemapTextureSizeCoord.Y); // Set the tilemap tex size in uv coord
		glUniform2i(pPipeline->GetUniformLocation(pShaderInformation->TextureSizeName), buffer.second.second.X, buffer.second.second.Y); // Set the tilemap tex width/height

		if (buffer.first != nullptr) // ShaderInfo::UpdateVertexData() will not create the texture if data is nullptr
		{
			glUniform1i(pPipeline->GetUniformLocation(pShaderInformation->TextureName), 0); // Texture unit 0
			glBindTexture(GL_TEXTURE_2D, pShaderInformation->p_TextureBufferName);
			glActiveTexture(GL_TEXTURE0);
		}

		// Draw quads
		glDrawArrays(GL_QUADS, 0, 4);

		// Release buffers
		if (buffer.first != nullptr)
		{
			pShader->p_ShaderInformation->Release();
			free(buffer.first);
		}
	}

	void TilemapMesh::Release()
	{
		((Game::GameObject::Tilemap::Tilemap*)mp_Tilemap)->p_Mesh = nullptr;
		delete(this);
	}
}