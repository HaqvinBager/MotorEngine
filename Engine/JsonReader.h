#pragma once
#include <fstream>
#include "../Includes/rapidjson/document.h"
#include "../Includes/rapidjson/istreamwrapper.h"

class CJsonReader
{
public:
	static rapidjson::Document LoadDocument(const std::string json_path)
	{
		std::ifstream stream(json_path);
		rapidjson::IStreamWrapper wrapper(stream);
		rapidjson::Document document;
		document.ParseStream(wrapper);
		return document;
	}
};