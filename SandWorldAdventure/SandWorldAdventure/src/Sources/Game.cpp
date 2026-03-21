
// Game
#include "SWA/Game.h"
#include "SWA/RenderLayerNames.h"

#include "SWAEngine/Tilemap/TileBehavior/Types.h"

// Graphics
#include "GP2D/Pipeline/GenericPipeline.h"

#include "GP2D/Pipeline/Shader/GeometryShader.h"
#include "GP2D/Pipeline/Mesh/Mesh.h"

#include <gl/glew.h>

using namespace GP2D;
using namespace GP2D::Pipeline;

namespace SWA
{
	Player::Player* Game::p_MainPlayer = nullptr;
	SWAEngine::Tilemap::Tilemap* Game::p_Tilemap = nullptr;
	SWAEngine::Tilemap::TilemapMesh* Game::p_TilemapMesh = nullptr;

	void Game::AddTextures()
	{
		// TODO: could define all textures in a .json file and load it
		GenericPipeline::s_Textures.RegisterTexture("default_sprite", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/default_sprite.bmp"));
		GenericPipeline::s_Textures.RegisterTexture("gui_background", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/GUI/Background.bmp"));
		GenericPipeline::s_Textures.RegisterTexture("lava_24", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/GUI/Lava24.bmp"));

		GenericPipeline::s_Textures.RegisterTexture("empty_slot", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/GUI/Slots/EmptySlot.bmp"));
		GenericPipeline::s_Textures.RegisterTexture("sand_stone_tile_slot", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/GUI/Slots/SandStoneTileSlot.bmp"));
		GenericPipeline::s_Textures.RegisterTexture("sand_tile_slot", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/GUI/Slots/SandTileSlot.bmp"));
		GenericPipeline::s_Textures.RegisterTexture("wet_sand_tile_slot", Texture::LoadBMPTextureFromFile(_SWA_RESOURCES_DIR"Textures/GUI/Slots/WetSandTileSlot.bmp"));
	}

	void Game::AddShaders()
	{
		auto pNewShader = new Shader::GeometryShader("DefaultSpriteShader", _SWA_RESOURCES_DIR"Shaders/TextureShader.shader");
		pNewShader->UpdatePropertiesCallback = [](Shader::BaseShaderType* pShader, void* texName)
		{
			pShader->PropertyManager.TrySetTexture(texName != nullptr ? (const char*)texName : "default_sprite", "Tex");

			INT loc = glGetUniformLocation(pShader->GetProgram(), "ShadeColor");
			glUniform4f(loc, 1, 1, 1, 1);
		};
		GenericPipeline::s_Shaders.RegisterShader(pNewShader);

		pNewShader = new Shader::GeometryShader("TilemapShader", _SWA_DEFAULT_RESOURCES_DIR"TilemapShader.shader");
		GenericPipeline::s_Shaders.RegisterShader(pNewShader);
	}

	void Game::CreateRenderLayers()
	{
		// Create render layers
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Tilemap0, { "Tilemap0" });
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Objects, { "Objects" });
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Characters, { "Characters" });
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_GUI, { "GUI" });
		GenericPipeline::s_Hierarchy.InsertLayer(RENDERLAYERS_Debug, { "Debug" });
	}

	void Game::InitializeGame()
	{
		// TEst mesh
		auto pNewMesh = new Mesh::Mesh(true, (void*)"lava_24");
		pNewMesh->Origin = { -0.1f, -.5f };
		pNewMesh->Scale = { 0.25f, 0.25f };
		pNewMesh->Vertices = {
			{{0, 0}, {0, 0}},
			{{1, 0}, {1, 0}},
			{{1, 1}, {1, 1}},
			{{0, 1}, {0, 1}} };
		pNewMesh->Triangles = {
			0, 1, 2,
			0, 2, 3
		};
		pNewMesh->Shaders = {
			{0, 6, "DefaultSpriteShader"}
		};
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Objects).RegisterMesh(pNewMesh);

		// Create tilemap
		p_Tilemap = new SWAEngine::Tilemap::Tilemap({ 0,0 }, { 0.01f,0.01f });
		p_TilemapMesh = new SWAEngine::Tilemap::TilemapMesh(p_Tilemap, "TilemapShader");
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Tilemap0).RegisterMesh(p_TilemapMesh);

		p_Tilemap->SetTile({ 2, 2 }, { SWAEngine::Tilemap::TileBehavior::SOLID, 0xff0000ff, true });
		p_Tilemap->SetTile({ 0, 0 }, { SWAEngine::Tilemap::TileBehavior::SAND, 0xff0000ff, true });
		p_Tilemap->SetTile({ 1, 2 }, { SWAEngine::Tilemap::TileBehavior::SOLID, 0xff0000ff, true });
		p_Tilemap->SetTile({ 0, 1 }, { SWAEngine::Tilemap::TileBehavior::SAND, 0xff0000ff, true });
		p_Tilemap->SetTile({ 3, 3 }, { SWAEngine::Tilemap::TileBehavior::SOLID, 0xffFFffFF, true });

		// Create the player
		p_MainPlayer = new Player::Player();
		p_MainPlayer->SetPosition({ 1, 2 });
	}


	void Game::CreateResources()
	{
		// Register textures
		AddTextures();
		// Register shaders
		AddShaders();
	}
	void Game::Initialize()
	{
		// Set up graphics
		CreateRenderLayers();

		// Set up game
		InitializeGame();
	}
	void Game::Update(SWAEngine::Time time)
	{
		SWAEngine::BaseGameObject::UpdateObjects(time);
		p_Tilemap->Update(time);
	}
	void Game::Release()
	{
		SWAEngine::BaseGameObject::ReleaseObjects();
		p_Tilemap->Release();
	}
}