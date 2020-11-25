#include "stdafx.h"
#include "animationLoader.h"

#include "GameObject.h"
#include "AnimationComponent.h"

bool AddAnimationsToGameObject(CGameObject& aGameObject, const std::string& aGameObjectFilePath)
{
	using namespace AnimationLoaderInternal;

	if (Has_SKSuffix(aGameObjectFilePath))
	{
		std::vector<std::string> animationPaths = Get_ANFiles(aGameObjectFilePath);
		aGameObject.AddComponent<CAnimationComponent>(aGameObject, aGameObjectFilePath, animationPaths);
		return true;
	}
	return false;
}

namespace AnimationLoaderInternal
{
	std::size_t number_of_files_in_directory(const std::filesystem::path& path)
	{// from : https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019
		using std::filesystem::directory_iterator;
		return std::distance(directory_iterator(path), directory_iterator{});
	}
	std::string GetSuffixFromString(const std::string& aString)
	{
		//ex: CH_NPC_Boss_Attack_AN.fbx =>  7 from last
		return std::move(aString.substr(aString.length() - 7, 3));
	}
	bool Has_SKSuffix(const std::string& aString)
	{
		return (GetSuffixFromString(aString) == "_SK");
	}

	std::vector<std::string> Get_ANFiles(const std::string& aFullFilePath)
	{
		const size_t lastSlashIndex = aFullFilePath.find_last_of("\\/");
		const std::string folderPath = aFullFilePath.substr(0, lastSlashIndex + 1);

		std::filesystem::path p(folderPath);
		std::filesystem::directory_iterator start(p);
		std::filesystem::directory_iterator end;

		std::vector<std::string> _ANPaths;
		_ANPaths.reserve(number_of_files_in_directory(p));
		for (auto& it = start; it != end; ++it)
		{
			if (it->path().extension() == ".fbx")
			{
				const std::string filePath = it->path().filename().string();
				if (GetSuffixFromString(filePath) == "_AN")
				{
					_ANPaths.emplace_back(folderPath + filePath);
				}
			}
		}
		_ANPaths.shrink_to_fit();
		return std::move(_ANPaths);
	}
}