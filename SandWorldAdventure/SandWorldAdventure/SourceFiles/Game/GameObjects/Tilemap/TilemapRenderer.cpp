#include "HeaderFiles/Game/GameObjects/Tilemap/TilemapRenderer.h"
#include "HeaderFiles/Game/GameObjects/Tilemap/Tilemap.h" // for tilemaprenderer.h
#include "HeaderFiles/Game/GameInstance.h"
#include "HeaderFiles/Render/Renderer.h"
#include "HeaderFiles/Vector2Int.h"
#include "HeaderFiles/Vector2.h"
#include <algorithm>

namespace SandboxEngine::Game::GameObject::Tilemap
{
	void TilemapRenderer::GetScreenBoundsInViewport(Vector2* pLeftBottom, Vector2* pRightTop)
	{
		*pLeftBottom = GameInstance::MainCamera.FromScreenToViewport(Vector2(0, 0));
		*pRightTop = GameInstance::MainCamera.FromScreenToViewport(
			Vector2(
				GameInstance::MainCamera.MainScreen.BitmapInfo.bmiHeader.biWidth,
				GameInstance::MainCamera.MainScreen.BitmapInfo.bmiHeader.biHeight));
	}

	void TilemapRenderer::CompileChunkForRender(Tilemap* pTilemap, Chunk* pChunk)
	{
		//TODO: Check whether position is valid

		//!TilemapContainer::IsChunkInitialized(pChunk) && 
		if (!pChunk->Tiles.has_value() || pChunk->Tiles.value().size() != TilemapContainer::CHUNK_SIZE.X * TilemapContainer::CHUNK_SIZE.Y)
			return;

		if (pTilemap->RenderInformation.ShouldResize || !pChunk->IsActive || !pChunk->Bitmap.has_value())
			ResizeChunkBitmap(pTilemap, pChunk);

		ActivateChunk(pTilemap, pChunk);
		CopyChunkBitmapToScreen(pTilemap, pChunk);
	}

	void TilemapRenderer::TryDeactivateChunks(Tilemap* pTilemap) // TODO: Could combine this with the activate chunks func // TODO: Dont need to keep track of all active chunks just check a box around them
	{
		Vector2 worldLeftBottom, worldRightTop;
		GetScreenBoundsInViewport(&worldLeftBottom, &worldRightTop);
		worldLeftBottom = GameInstance::MainCamera.FromViewportToWorld(worldLeftBottom);
		worldRightTop = GameInstance::MainCamera.FromViewportToWorld(worldRightTop);

		Chunk* pChunk = nullptr;
		for (int i = 0; i < pTilemap->RenderInformation.ActiveChunks.size(); i++)
		{
			Vector2 chunkPos = pTilemap->RenderInformation.ActiveChunks[i];
			Vector2 chunkWorldPos = pTilemap->FromTileToWorld(chunkPos * TilemapContainer::CHUNK_SIZE);
			if (chunkWorldPos.X + TilemapContainer::CHUNK_SIZE.X * pTilemap->TileSize.X < worldLeftBottom.X ||
				chunkWorldPos.Y + TilemapContainer::CHUNK_SIZE.Y * pTilemap->TileSize.Y < worldLeftBottom.Y ||
				worldRightTop.X < chunkWorldPos.X ||
				worldRightTop.Y < chunkWorldPos.Y)
			{
				//TODO: Check whether position is valid
				pChunk = pTilemap->Container.GetChunk(chunkPos.X + chunkPos.Y * pTilemap->Container.GetChunkBounds().X);
				DeactivateChunk(pTilemap, pChunk);
			}
		}
	}

	void TilemapRenderer::DeactivateChunk(Tilemap* pTilemap, Chunk* pChunk)
	{
		if (!pChunk->IsActive)
			return;
		pChunk->IsActive = false;

		// TODO: Possibly expensive operation
		std::vector<Vector2>::iterator it = std::find(pTilemap->RenderInformation.ActiveChunks.begin(), pTilemap->RenderInformation.ActiveChunks.end(), Vector2(pChunk->X, pChunk->Y));
		pTilemap->RenderInformation.ActiveChunks.erase(it);

		if (pChunk->Bitmap.has_value())
			pChunk->Bitmap.value().Release();
	}
	void TilemapRenderer::ActivateChunk(Tilemap* pTilemap, Chunk* pChunk)
	{
		if (pChunk->IsActive)
			return;
		pChunk->IsActive = true;
		pTilemap->RenderInformation.ActiveChunks.push_back(Vector2(pChunk->X, pChunk->Y));
	}

	void TilemapRenderer::Render(Tilemap* pTilemap/*, ScreenState* pScreen*/)
	{
		// . Try deactivate, try Resize, Activate, and draw chunks

		TryDeactivateChunks(pTilemap);
		if (pTilemap->RenderInformation.ShouldResize)
			Resize(pTilemap);
		WhereAllVisibleChunks(pTilemap, TilemapRenderer::CompileChunkForRender);
		pTilemap->RenderInformation.ShouldResize = false;


		//// Render tilemap
		//Vector2 size = pTilemap->Container.GetChunksDimensions();
		//Vector2 screenSize = Renderer::FromScreenToWorld(pScreen, Vector2(pScreen->BitmapInfo.bmiHeader.biWidth, pScreen->BitmapInfo.bmiHeader.biHeight), false);
		//for (int i = 0; i < size.X * size.Y; i++)
		//{
		//	// Is chunk outside of screen?
		//	if ((i % (int)size.X) * TilemapContainer::CHUNK_SIZE.X * pTilemap->TileSize.X > screenSize.X ||
		//		(i / (int)size.X) * TilemapContainer::CHUNK_SIZE.Y * pTilemap->TileSize.Y > screenSize.Y)
		//		continue;

		//	Chunk* pChunk = pTilemap->Container.GetChunk(i);
		//	if (pChunk->NonNullTilesCount <= 0)
		//		continue;

		//	for (int ii = 0; ii < TilemapContainer::CHUNK_SIZE.X * TilemapContainer::CHUNK_SIZE.Y; ii++)
		//	{
		//		Tile* pTile = &pChunk->Tiles[ii];
		//		if (!pTile->IsNotNull)
		//			continue;
		//		// position of tile, normalized, and then scaled to screen size
		//		int column = ii % (int)TilemapContainer::CHUNK_SIZE.X + (i % (int)size.X) * (int)TilemapContainer::CHUNK_SIZE.X;
		//		int row = ii / (int)TilemapContainer::CHUNK_SIZE.X + (i / (int)size.X) * (int)TilemapContainer::CHUNK_SIZE.Y;
		//		Vector2 worldPos = pTilemap->FromTileToWorld(Vector2(column, row));

		//		Renderer::DrawRect(pScreen, worldPos.X, worldPos.Y, pTilemap->TileSize.X, pTilemap->TileSize.Y, pTile->Color);
		//	}
		//}
	}
	void TilemapRenderer::Resize(Tilemap* pTilemap/*, ScreenState* pScreen*/)
	{
		// TODO: We need to calculate exactly how long(BASED ON SCREEN SIZE) it should be and have an algorithim that returns the tile bounds with no overlap
		// Draw each tile

		Vector2 tilemapViewportTileSize = GameInstance::MainCamera.FromWorldToViewport(Vector2(pTilemap->TileSize.X * pTilemap->Container.GetTileBounds().X, pTilemap->TileSize.Y * pTilemap->Container.GetTileBounds().Y), false);
		Vector2 tilemapPixelSize = GameInstance::MainCamera.FromViewportToScreen(tilemapViewportTileSize, false);
		pTilemap->RenderInformation.ExactChunkPixelSize = Vector2(ceil(tilemapPixelSize.X), ceil(tilemapPixelSize.Y)) / pTilemap->Container.GetChunkBounds();
		pTilemap->RenderInformation.ExactTilePixelSize = Vector2(ceil(tilemapPixelSize.X), ceil(tilemapPixelSize.Y)) / pTilemap->Container.GetTileBounds();

		//pTilemap->RenderInformation.State.Resize(ceil(tilemapPixelSize.X), ceil(tilemapPixelSize.Y));
		//Tile* pTile = nullptr;
		//unsigned int color = 0;

		//// The direction of the rendering so that the overlap is left to right, bottom to top
		//for (int i = pTilemap->Container.GetTilesDimensions().X * pTilemap->Container.GetTilesDimensions().Y - 1; i >= 0; i--)
		//{
		//	int x = i % (int)pTilemap->Container.GetTilesDimensions().X, y = i / (int)pTilemap->Container.GetTilesDimensions().X;
		//	pTile = pTilemap->Container.GetTileInChunk(x, y);
		//	color = pTile->IsNotNull ? pTile->Color : TILEMAP_BITMAP_NULL_TILE_COLOR;

		//	ChangeTileData(pTilemap,x,y, color, false);
		//}
	}
	void TilemapRenderer::ResizeChunkBitmap(Tilemap* pTilemap, Chunk* pChunk)
	{
		// Resize the bitmap
		if (!pChunk->Bitmap.has_value())
			pChunk->Bitmap = Render::ScreenState(round(pTilemap->RenderInformation.ExactChunkPixelSize.X), round(pTilemap->RenderInformation.ExactChunkPixelSize.Y));
		else
			pChunk->Bitmap.value().Resize(round(pTilemap->RenderInformation.ExactChunkPixelSize.X), round(pTilemap->RenderInformation.ExactChunkPixelSize.Y));

		Tile* pTile;
		UINT color;
		for (int i = 0; i < TilemapContainer::CHUNK_SIZE.X; i++)
		{
			for (int j = 0; j < TilemapContainer::CHUNK_SIZE.Y; j++)
			{
				pTile = &pChunk->Tiles.value()[i + j * TilemapContainer::CHUNK_SIZE.X];
				color = TILEMAP_BITMAP_NULL_TILE_COLOR;
				if (pTile != nullptr && pTile->HasValue)
					color = pTile->Color;

				ChangeTileInChunkData(pTilemap, pChunk, i, j, color);
			}
		}
	}

	void TilemapRenderer::ChangeTileData(Tilemap* pTilemap, int column, int row, unsigned int color)
	{
		Vector2 chunkPos = pTilemap->Container.TileToChunkCoordinates(Vector2Int(column, row));
		ChangeTileInChunkData(pTilemap,
			pTilemap->Container.GetChunk(chunkPos.X + chunkPos.Y * pTilemap->Container.GetChunkBounds().X),
			column - chunkPos.X * TilemapContainer::CHUNK_SIZE.X,
			row - chunkPos.Y * TilemapContainer::CHUNK_SIZE.Y, color);
	}
	void TilemapRenderer::ChangeTileInChunkData(Tilemap* pTilemap, Chunk* pChunk, int column, int row, unsigned int color)
	{
		Vector2 exactPixelPosition = pTilemap->RenderInformation.ExactTilePixelSize * Vector2(column, row);

		Render::Renderer::DrawRectPixels(&pChunk->Bitmap.value(),
			floor(exactPixelPosition.X),
			floor(exactPixelPosition.Y),
			round(pTilemap->RenderInformation.ExactTilePixelSize.X),
			round(pTilemap->RenderInformation.ExactTilePixelSize.Y), color);
	}
	void TilemapRenderer::CopyChunkBitmapToScreen(Tilemap* pTilemap, Chunk* pChunk)
	{
		if (!pChunk->Bitmap.has_value() || pChunk->Bitmap.value().Data == nullptr || pChunk->Bitmap.value().BitmapInfo.bmiHeader.biWidth <= 1)
			return;

		Render::ScreenState* pMainScreen = &GameInstance::MainCamera.MainScreen;// &MasterWindow::ScreenState;
		if (pMainScreen->Data == nullptr || pMainScreen->BitmapInfo.bmiHeader.biWidth <= 1)
			return;

		Vector2 chunkWorldPos = pTilemap->Position + Vector2(pChunk->X, pChunk->Y) * TilemapContainer::CHUNK_SIZE * pTilemap->TileSize;
		Vector2 chunkViewportPos = GameInstance::MainCamera.FromWorldToViewport(chunkWorldPos);
		Vector2 chunkScreenPos = GameInstance::MainCamera.FromViewportToScreen(chunkViewportPos);

		Render::Renderer::CopyState(&pChunk->Bitmap.value(), chunkScreenPos.X, chunkScreenPos.Y, &GameInstance::MainCamera.MainScreen, 0, 0, TILEMAP_BITMAP_NULL_TILE_COLOR);
	}

	void TilemapRenderer::ReleaseChunks(Tilemap* pTilemap)
	{
		Chunk* pChunk = nullptr;
		for (int i = 0; i < pTilemap->RenderInformation.ActiveChunks.size(); i++)
		{
			Vector2 chunkPos = pTilemap->RenderInformation.ActiveChunks[i];
			pChunk = pTilemap->Container.GetChunk(chunkPos.X + chunkPos.Y * pTilemap->Container.GetChunkBounds().X);
			if (pChunk->Bitmap.has_value())
				pChunk->Bitmap.value().Release();
		}
	}

	void TilemapRenderer::WhereAllVisibleChunks(Tilemap* pTilemap, void (*func)(Tilemap*, Chunk*))
	{
		Vector2 worldLeftBottom, worldRightTop;
		GetScreenBoundsInViewport(&worldLeftBottom, &worldRightTop);
		worldLeftBottom = GameInstance::MainCamera.FromViewportToWorld(worldLeftBottom);
		worldRightTop = GameInstance::MainCamera.FromViewportToWorld(worldRightTop);

		Vector2 chunkLeftBottom = (worldLeftBottom - pTilemap->Position) / TilemapContainer::CHUNK_SIZE;
		Vector2 chunkRightTop = (worldRightTop - pTilemap->Position) / TilemapContainer::CHUNK_SIZE;

		//TODO: Could take pTilemap->Container.GetChunksDimensions().X as a variable
		for (int c = floor(std::clamp(chunkLeftBottom.X, 0.0, (double)pTilemap->Container.GetChunkBounds().X)); c < floor(std::clamp(chunkRightTop.X, 0.0, (double)pTilemap->Container.GetChunkBounds().X - 1)) + 1; c++)
		{
			for (int r = floor(std::clamp(chunkLeftBottom.Y, 0.0, (double)pTilemap->Container.GetChunkBounds().Y)); r < floor(std::clamp(chunkRightTop.Y, 0.0, (double)pTilemap->Container.GetChunkBounds().Y - 1)) + 1; r++)
			{
				// TODO: Check chunk is at valid position
				Chunk* pChunk = pTilemap->Container.GetChunk(c + r * pTilemap->Container.GetChunkBounds().X);
				func(pTilemap, pChunk);
			}
		}
	}
}