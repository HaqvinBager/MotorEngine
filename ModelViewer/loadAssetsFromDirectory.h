#pragma once

#include <vector>
#include <filesystem>

namespace ModelViewer
{
	std::size_t number_of_files_in_directory(const std::filesystem::path& path);
	void LoadModelPaths(const std::string& aStartFolderPath, std::vector<std::string>& aFBXFilePaths);
}