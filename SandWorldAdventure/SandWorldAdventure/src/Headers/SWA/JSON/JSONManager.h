#pragma once
#include "texture.h"

#include <SWAEngine/FileManager.h>
#include <nlohmann/json.hpp>

#include <string>
#include <stdexcept>

#include <GP2D/Pipeline/GenericPipeline.h>
#include <GP2D/Pipeline/Texture/BitmapTexture.h>
#include <iostream>

namespace SWA::JSON
{
	 const char* const LOCAL_TEXTURES_JSON_PATH = "textures.json";

	// Manages reading/writing JSON files for the game
	struct JSONManager 
	{
		// Loads the aliases and file paths for texture
		static void LoadTextures()
		{
			using GP2D::Pipeline::GenericPipeline;
			using GP2D::Pipeline::Texture::BitmapTexture;

			// Read file
			char* code; size_t size;
			if (SWAEngine::FileManager::FILE_MANAGER_FAILED & SWAEngine::FileManager::ReadFile(std::string(_SWA_RESOURCES_DIR).append(LOCAL_TEXTURES_JSON_PATH).c_str(), &code, &size))
				throw std::runtime_error("Failed to read texture file!");
			
			nlohmann::json textures = nlohmann::json::parse(code);
			for(nlohmann::json& element : textures)
			{
				if (!element.is_object())
					continue;
				// Get information
				texture tex = element; // Convert
				std::string fullPath = std::string(_SWA_RESOURCES_DIR) + tex.path;

				// Try get bitmap
				BitmapTexture bitmap = BitmapTexture::LoadBMPTextureFromFile(fullPath.c_str());
				if (bitmap.ColorsSize < 2)
				{
					fprintf(stdout, (std::string("failed to locate texture: ") + tex.path + "\n").c_str());
					continue;
				}

				// Register texture
				GenericPipeline::s_Textures.RegisterTexture(tex.alias.c_str(), std::move(bitmap));
			}
		}
	};
}