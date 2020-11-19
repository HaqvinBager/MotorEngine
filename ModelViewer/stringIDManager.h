#pragma once

#define STRING_ID_MIN 2
#define STRING_ID_MAX 599999

#include <string>
#include <cassert>
#include <fstream>

#include "../External/rapidjson/document.h"

#include <iostream>
namespace StringID
{
	static unsigned int GetIDOfString(const std::string& aString)
	{
		aString;
		unsigned int stringID = 0;

		std::string idFile = "StringID.json";
		std::ifstream iFS(idFile.c_str());
		if (!iFS.is_open())
		{
			std::cout << "Creating file" << idFile.c_str() << std::endl;
			std::ofstream oFS(idFile.c_str());
			oFS.close();
		}
		iFS.open(idFile.c_str());
		assert(iFS.is_open());
		rapidjson::Document document;
		document.Parse(idFile.c_str());
		//std::string str((std::istreambuf_iterator<char>(iFS)), std::istreambuf_iterator<char>());
		//
		//document.Parse(str.c_str());
		//
		//rapidjson::Value::ConstMemberIterator root = document.MemberBegin();
		//rapidjson::Value::ConstMemberIterator oneStepDown = document.FindMember("Levels");
		//
		//rapidjson::Value& results = document["Levels"];
		//
		//std::string s = results[0]["LevelName"].GetString();

		return stringID;
	}
}
