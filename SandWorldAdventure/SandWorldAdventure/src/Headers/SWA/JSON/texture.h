#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace SWA::JSON
{
	struct texture 
	{
		std::string alias;
		std::string path;

		texture() = default;

		// Convert from json
		texture(const nlohmann::json& js)
		{
			alias = js.at("alias").get<std::string>();
			path = js.at("path").get<std::string>();
		}

		// Convert to json
		operator nlohmann::json()
		{
			return
			{
				{"alias", alias},
				{"path", path}
			};
		}
	};
}