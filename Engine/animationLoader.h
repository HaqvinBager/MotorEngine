#pragma once

#include <filesystem>
#include <string>
#include <vector>

class CGameObject;

enum class EAnimatedObject
{
	Player,
	Enemy,
	Boss,
	Destructible
};

bool AddAnimationsToGameObject(CGameObject& aGameObject, const std::string& aGameObjectFilePath, const EAnimatedObject anAnimatedObject);
void SetAnimations(CGameObject& aGameObject, const EAnimatedObject anAnimatedObject);
namespace AnimationLoaderInternal
{
	std::size_t number_of_files_in_directory(const std::filesystem::path& path);
	std::string GetSuffixFromString(const std::string& aString);
	bool Has_SKSuffix(const std::string& aString);
	std::vector<std::string> Get_ANFiles(const std::string& aFullFilePath);
}
