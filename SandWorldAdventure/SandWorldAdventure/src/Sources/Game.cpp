
// Game
#include "SWA/Game.h"
#include "SWA/RenderLayerNames.h"
#include "SWA/ShaderNames.h"
#include "SWA/Player/PlayerInventoryGUI.h"

#include "SWAEngine/Inventory/Inventory.h"
#include "SWAEngine/Inventory/BasicItem.h"

// Graphics
#include "GP2D/Pipeline/GenericPipeline.h"

#include "GP2D/Pipeline/Shader/GeometryShader.h"
#include "GP2D/Pipeline/Mesh/Mesh.h"

#include <gl/glew.h>

using namespace GP2D;
using namespace GP2D::Pipeline;

namespace SWA
{
	SWAEngine::Tilemap::Tilemap* Game::gp_Tilemap = nullptr;
	SWAEngine::Tilemap::TilemapMesh* Game::gp_TilemapMesh = nullptr;

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

			pShader->PropertyManager.TrySetUniform("ShadeColor",
				[](int loc)
			{
				glUniform4f(loc, 1, 1, 1, 1);
			});
		};
		GenericPipeline::s_Shaders.RegisterShader(pNewShader); // SWA_TEXTURE_SHADER

		pNewShader = new Shader::GeometryShader("TilemapShader", _SWA_DEFAULT_RESOURCES_DIR"TilemapShader.shader");
		GenericPipeline::s_Shaders.RegisterShader(pNewShader); // SWA_TILEMAP_SHADER
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
			0, 6, SWA_TEXTURE_SHADER
		};
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Objects).RegisterMesh(pNewMesh);

		// Create tilemap
		gp_Tilemap = new SWAEngine::Tilemap::Tilemap({ 0,0 }, { 0.1f,0.1f });
		gp_TilemapMesh = new SWAEngine::Tilemap::TilemapMesh(gp_Tilemap, SWA_TILEMAP_SHADER);
		GenericPipeline::s_Hierarchy.GetLayer(RENDERLAYERS_Tilemap0).RegisterMesh(gp_TilemapMesh);

		gp_Tilemap->SetTile({ 2, 2 }, { 0, 0xff0000ff, true });
		gp_Tilemap->SetTile({ 0, 0 }, { 0, 0xff0000ff, true });
		gp_Tilemap->SetTile({ 1, 2 }, { 0, 0xff0000ff, true });
		gp_Tilemap->SetTile({ 0, 1 }, { 0, 0xff0000ff, true });
		gp_Tilemap->SetTile({ 3, 3 }, { 0, 0xffFFffFF, true });

		// Create player inventory and gui
		SWAEngine::Inventory::Inventory<SWAEngine::Inventory::BasicItem> inventory = { {5,5}, SWAEngine::Inventory::BasicItem() };
		inventory.SetItemAt({ 1,1 }, { "lava_24", nullptr, NULL });
		inventory.SetItemAt({ 3,1 }, { "sand_tile_slot", nullptr, NULL });
		SWA::Player::PlayerInventoryGUI::Initialize(inventory);
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
		gp_Tilemap->Update();
	}
	void Game::Release()
	{
		gp_Tilemap->Release();
	}
}