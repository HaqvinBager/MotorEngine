#include "stdafx.h"
#include "animationLoader.h"

#include "GameObject.h"
#include "AnimationComponent.h"

#include <fstream>
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"

bool AddAnimationsToGameObject(CGameObject& aGameObject, const std::string& aGameObjectFilePath, const EAnimatedObject anAnimatedObject)
{
	using namespace AnimationLoaderInternal;

	std::vector<std::string> animationPaths = Get_ANFiles(aGameObjectFilePath);
	aGameObject.AddComponent<CAnimationComponent>(aGameObject, aGameObjectFilePath, animationPaths);
	SetAnimations(aGameObject, anAnimatedObject);
	return true;
}

std::string GetSuffixFromString(const std::string& aString)
{
	//ex: CH_NPC_Boss_Attack_AN.fbx =>  7 from last => _AN
	return std::move(aString.substr(aString.length() - 7, 3));
}

void SetAnimations(CGameObject& aGameObject, const EAnimatedObject anAnimatedObject)
{
	using namespace rapidjson;

	std::ifstream inputStream;
	switch (anAnimatedObject)
	{
		case EAnimatedObject::Player:
			inputStream.open("Json/Animations_Player.json");
			ENGINE_BOOL_POPUP(inputStream.good(), "Looking for Json/Animations_Player.json. Could not be found.");
		break;
		
		case EAnimatedObject::Enemy:
			inputStream.open("Json/Animations_Enemy.json");
			ENGINE_BOOL_POPUP(inputStream.good(), "Looking for Json/Animations_Enemy.json. Could not be found.");
		break;

		case EAnimatedObject::Boss:
			inputStream.open("Json/Animations_Boss.json");
			ENGINE_BOOL_POPUP(inputStream.good(), "Looking for Json/Animations_Boss.json. Could not be found.");
		break;

		case EAnimatedObject::Destructible:
			inputStream.open("Json/Animations_Destructible.json");
			ENGINE_BOOL_POPUP(inputStream.good(), "Looking forJson/Animations_Destructible.json. Could not be found.");
		break;

		default:
		break;
	}

	// Yes... i know but... i am lazy and stressed :/ /Aki
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);
	CAnimationComponent* anim = aGameObject.GetComponent<CAnimationComponent>();
	anim->SetIdleID(document["Idle 1"].GetInt());
	anim->SetMovingID(document["Move"].GetInt());
	anim->SetDyingID(document["Dying"].GetInt());
	anim->SetAttack01ID(document["Attack 1"].GetInt());
	anim->SetAttack02ID(document["Attack 2"].GetInt());
	anim->SetAbility01ID(document["Ability 1"].GetInt());
	anim->SetAbility02ID(document["Ability 2"].GetInt());
	anim->SetAbility03ID(document["Ability 3"].GetInt());
	anim->SetAbility04ID(document["Ability 4"].GetInt());
	anim->SetAbility05ID(document["Ability 5"].GetInt());
	anim->SetAbility06ID(document["Ability 6"].GetInt());

	inputStream.close();
}

namespace AnimationLoaderInternal
{
	std::size_t number_of_files_in_directory(const std::filesystem::path& path)
	{// from : https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019
		using std::filesystem::directory_iterator;
		return std::distance(directory_iterator(path), directory_iterator{});
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