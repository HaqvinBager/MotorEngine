#include "loadAssetsFromDirectory.h"

#include "helpers.h"

#include <iostream>
#include <map>

namespace ModelViewer
{
	std::size_t number_of_files_in_directory(const std::filesystem::path& path)
	{// from : https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019
		using std::filesystem::directory_iterator;
		return std::distance(directory_iterator(path), directory_iterator{});
	}
	void LoadModelPaths(const std::string& aStartFolderPath, std::vector<std::string>& aFBXFilePaths)
	{
		struct SFileInfo// Used for directories.
		{
			short myNumItems = 0;
			std::string myFullPath;
		};
	
		std::filesystem::path p(aStartFolderPath);
	
		std::filesystem::recursive_directory_iterator start(p);
		std::filesystem::recursive_directory_iterator end;
	
		std::map<std::string, SFileInfo> folders;
		std::vector<std::string> prevFolders;// Keys to use for std::map folders.
	
		/// Description of how <prevFolders> works
	
		///		prevFolders[depth]
		///		"Assets" depth = 0
		///			"3D" depth = 1
		///				"Character" depth = 2
		///					"Boss" depth = 3
		///						1 file(s) when all have been checked go up 1 depth.
		///					"MainCharacter" depth = 3
		///  
		/// Example =>	we are on depth 3, "Boss":
		///		folders[prevFolders[depth]].myFullPath = "Assets/3D/Character/Boss"
		///		folders[prevFolders[depth]].mySize = 1
		///		If we find an FBX inside of ../Boss: then aFBXFilePath = folders[prevFolders[depth]].myFullPath + / +"fbxFileName"
		///		(so when size = 0 we go up 1 depth)
		/// 
		/// </prevFolders>
	
		int depth = 0;// Key for prevFolders
	
		std::string folderPath = aStartFolderPath;
		std::map<std::string, short> foldersWithSize;
	
		prevFolders.emplace_back(aStartFolderPath);
	
		SFileInfo fileInfo;
		fileInfo.myNumItems		= static_cast<short>( number_of_files_in_directory(start->path()));
		fileInfo.myFullPath		= aStartFolderPath;
	
		folders.emplace(prevFolders[depth], fileInfo);
	
		for (auto i = start; i != end; ++i)
		{
			std::string filePath = i->path().filename().string();
	
			if (folders[prevFolders[depth]].myNumItems > 0)
			{
				folders[prevFolders[depth]].myNumItems -= 1;
			}
			else if (folders[prevFolders[depth]].myNumItems == 0)
			{
				if (depth != 0)
				{
					--depth;
					prevFolders.pop_back();
	
				}
				folderPath = folders[prevFolders[depth]].myFullPath;
	
				folders[prevFolders[depth]].myNumItems -= 1;
				if (folders[prevFolders[depth]].myNumItems <= 0)
				{
					if (depth != 0)
					{
						--depth;
						prevFolders.pop_back();
					}
	
					folderPath = folders[prevFolders[depth]].myFullPath;
				}
			}
	
			if (i->is_directory())
			{
				++depth;
				prevFolders.emplace_back(filePath);
	
				folderPath.append("/" + filePath);
	
				fileInfo.myNumItems	= static_cast<short>(number_of_files_in_directory(i->path()));
				fileInfo.myFullPath = folderPath;
	
				folders.emplace(filePath, fileInfo);
			}
			else
			{
				const size_t checkForDot = filePath.find(".");
				std::string fileExtension = filePath.substr(checkForDot, ((4 + checkForDot) < filePath.length() ? 4 : filePath.length() - 1 ));
				// todo filesystem::path has a function to check if it is a directory.
				if (fileExtension == ".fbx")
				{
					//ex: CH_NPC_Boss_Attack_AN.fbx 7 from last
					std::string suffix = GetSuffixFromString(filePath);
					if (suffix != "_AN")
					{
						aFBXFilePaths.emplace_back(folders[prevFolders[depth]].myFullPath + "/" + filePath);
					}
				}
			}
		}
	}
}