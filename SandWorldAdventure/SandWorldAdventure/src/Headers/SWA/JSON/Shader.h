#pragma once
#include "Resource.h"

namespace SWA::JSON
{
	struct Shader
	{
		enum {
			SHADER_TYPE_NULL = 0,
			SHADER_TYPE_GEOMETRY,
		};

		Resource resource;
		int type;

		Shader() = default;

		// Convert from json
		Shader(const nlohmann::json& js)
		{
			resource = static_cast<Resource>(js.at("resource"));
			type = js.at("type").get<int>();
		}

		// Convert to json
		operator nlohmann::json()
		{
			return
			{
				{"resource", resource},
				{"type", type}
			};
		}
	};
}