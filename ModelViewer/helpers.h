#pragma once

#include <string>

namespace ModelViewer
{
	// Returns the _NN part of the string ex CH_NPC_Boss_Attack_AN.fbx returns _AN
	std::string GetSuffixFromString(const std::string& aString);
	bool CheckForIncorrectModelNumber(const size_t& aLoadModelNumber, const size_t& aMax);
}