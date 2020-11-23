#pragma once

#include <vector>
#include <filesystem>

#include <Scene.h>

namespace ModelViewer
{
	void LoadModelPaths(const std::string& aStartFolderPath, std::vector<std::string>& aFBXFilePaths, const bool aLoadSK = false);
	//Don't use this one. It's over engineered and slightly buggy. It's cool tho.
	void LoadModelPathsOverEngineeredButCool(const std::string& aStartFolderPath, std::vector<std::string>& aFBXFilePaths, const bool aLoadSK = false);
}