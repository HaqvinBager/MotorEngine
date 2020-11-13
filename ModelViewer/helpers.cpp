#include "helpers.h"

namespace ModelViewer
{
	std::string GetSuffixFromString(const std::string& aString)
	{
		//ex: CH_NPC_Boss_Attack_AN.fbx 7 from last
		return std::move(aString.substr(aString.length() - 7, 3));
	}

	bool CheckForIncorrectModelNumber(const size_t& aLoadModelNumber, const size_t& aMax)
	{
		return (static_cast<int>(aLoadModelNumber) > -1 && aLoadModelNumber < aMax);
	}
}
