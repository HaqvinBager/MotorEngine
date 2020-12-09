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
// Gets the three last chars before file ending, ex: _SK from enemy_SK.fbx
std::string GetSuffixFromString(const std::string& aString);
bool AddAnimationsToGameObject(CGameObject& aGameObject, const std::string& aGameObjectFilePath, const EAnimatedObject anAnimatedObject);
void SetAnimations(CGameObject& aGameObject, const EAnimatedObject anAnimatedObject);
namespace AnimationLoaderInternal
{
	std::size_t number_of_files_in_directory(const std::filesystem::path& path);
	
	bool Has_SKSuffix(const std::string& aString);
	std::vector<std::string> Get_ANFiles(const std::string& aFullFilePath);
}
