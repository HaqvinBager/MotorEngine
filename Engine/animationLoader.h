#pragma once

#include <filesystem>
#include <string>
#include <vector>

class CGameObject;

bool AddAnimationsToGameObject(CGameObject& aGameObject, const std::string& aGameObjectFilePath);
namespace AnimationLoaderInternal
{
	std::size_t number_of_files_in_directory(const std::filesystem::path& path);
	std::string GetSuffixFromString(const std::string& aString);
	bool Has_SKSuffix(const std::string& aString);
	std::vector<std::string> Get_ANFiles(const std::string& aFullFilePath);
}
