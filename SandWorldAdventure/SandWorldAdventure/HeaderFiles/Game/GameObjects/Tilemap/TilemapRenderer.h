#pragma once
#include "HeaderFiles/Game/GameObjects/Tilemap/Chunk.h"

#ifndef TILEMAP_H
#define TILEMAP_H

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class Tilemap;
}
#endif

const unsigned int TILEMAP_BITMAP_NULL_TILE_COLOR = 0xff00ff;

namespace SandboxEngine::Game::GameObject::Tilemap
{
	class TilemapRenderer
	{
	private:
		static void GetScreenBoundsInViewport(Vector2* pLeftBottom, Vector2* pRightTop);

		static void CompileChunkForRender(Tilemap* pTilemap, Chunk* pChunk);
		static void TryDeactivateChunks(Tilemap* pTilemap);

		static void DeactivateChunk(Tilemap* pTilemap, Chunk* pChunk);
		static void ActivateChunk(Tilemap* pTilemap, Chunk* pChunk);
	public:

		// Copies the tilemap bitmap to the screen - 0xff00ff is likely to be null
		static void Render(Tilemap* pTilemap /*, ScreenState* pScreen*/);
		//TODO: Resize should probably be private func
		/// <summary>
		///  Used when the screen or tilemap is resized and gets new pixel sizes
		/// </summary>
		static void Resize(Tilemap* pTilemap/*, ScreenState* pScreen*/);
		/// <summary>
		/// Resizes the bitmap and fills out the pixel information based on contained tiles
		/// </summary>
		/// <param name="pTilemap"></param>
		/// <param name="column"></param>
		/// <param name="row"></param>
		static void ResizeChunkBitmap(Tilemap* pTilemap, Chunk* pChunk);

		/// <summary>
		/// Changes the state of the tile in the bitmap. Converts to chunk coordinates.
		/// </summary>
		/// <param name="pTilemap"></param>
		/// <param name="pChunk"></param>
		/// <param name="column"></param>
		/// <param name="row"></param>
		/// <param name="color"></param>
		static void ChangeTileData(Tilemap* pTilemap, int column, int row, unsigned int color);
		static void ChangeTileInChunkData(Tilemap* pTilemap, Chunk* pChunk, int column, int row, unsigned int color);

		static void CopyChunkBitmapToScreen(Tilemap* pTilemap, Chunk* pChunk);

		static void ReleaseChunks(Tilemap* pTilemap);

		/// <summary>
		/// Apply a function to each visible chunk
		/// </summary>
		/// <param name="pTilemap"></param>
		/// <param name="func"></param>
		static void WhereAllVisibleChunks(Tilemap* pTilemap, void (*func)(Tilemap*, Chunk*));

	};
}