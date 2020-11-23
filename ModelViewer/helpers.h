#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace ModelViewer
{
	std::size_t number_of_files_in_directory(const std::filesystem::path& path);
	// Returns the _NN part of the string ex CH_NPC_Boss_Attack_AN.fbx returns _AN
	std::string GetSuffixFromString(const std::string& aString);
	bool CheckForIncorrectModelNumber(const size_t& aLoadModelNumber, const size_t& aMax);
	std::vector<std::string> Get_ANFiles(const std::string& aFullFilePath);
}