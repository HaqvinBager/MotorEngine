#pragma once

#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"
#include "rapidjson\ostreamwrapper.h"
#include "rapidjson\stringbuffer.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace rapidjson;

namespace Tga2D {
	class CText;
	class CSprite;
}

class FileManager {
public:
	FileManager() = delete;
	static std::string LoadJsonString(const char* file, const char* key);
	static float LoadJsonFloat(const char* file, const char* key);
	static int LoadJsonInt(const char* file, const char* key);
	static bool LoadJsonBool(const char *file, const char *key);
	static rapidjson::Document GetJsonDocument(const char *filename);
	static void SwapJsonValues(const char* file, const char* key1, const char* key2);
	//static void write_string(const char* file, const char* key, std::string element);
	//static void write_float(const char* file, const char* key, float element);
	//static void write_int(const char* file, const char* key, int element);
	//static void write_bool(const char* file, const char* key, bool element);
};

