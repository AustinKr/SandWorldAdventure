#pragma once
#include <nlohmann/json.hpp>
#include <SWAEngine/FileManager.h>
#include <SWA/JSON/texture.h>
#include <GP2D/Pipeline/GenericPipeline.h>
#include <GP2D/Pipeline/Texture/BitmapTexture.h>

#include <string>
#include <stdexcept>

namespace SWA::JSON
{
	// Manages reading/writing JSON files for the game
	struct JSONManager 
	{
		// Loads the json file
		static nlohmann::json LoadFromFile(const char* filePath)
		{
			// Read file
			char* code; size_t size;
			if (SWAEngine::FileManager::FILE_MANAGER_FAILED & 
				SWAEngine::FileManager::ReadFile(std::string(_SWA_RESOURCES_DIR).append(filePath).c_str(), &code, &size))
				throw std::runtime_error("Failed to read file!");

			nlohmann::json file = nlohmann::json::parse(code);
			return file;
		}

		static void LoadTextures(const char* filePath)
		{
			using GP2D::Pipeline::GenericPipeline;
			using GP2D::Pipeline::Texture::BitmapTexture;

			// Register textures
			for (nlohmann::json& element : LoadFromFile(filePath))
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