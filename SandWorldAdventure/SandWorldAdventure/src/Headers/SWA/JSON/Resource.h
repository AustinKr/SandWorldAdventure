#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace SWA::JSON
{
	struct Resource 
	{
		std::string alias;
		std::string path;

		Resource() = default;

		// Convert from json
		Resource(const nlohmann::json& js)
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