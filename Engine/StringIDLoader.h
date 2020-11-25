#pragma once

class CStringIDLoader
{
public:
	enum class EStringIDFiles
	{
		ModelFile,
		AnimFile
	};

	const static int ourErrorID = -1;

public:	
	static const int GetStringID(const std::string& aString, const CStringIDLoader::EStringIDFiles aFileKey);

private:
	CStringIDLoader() {};
	~CStringIDLoader() {};
	static const int GetStringIDFromFile(const std::string& aString, const std::string& anIDFilePath, const int aMinID, const int aMaxID);

private:
	const static std::string ourModelStringIDPath;
	const static std::string ourAnimationStringIDPath;
	const static std::string ourNextAvailableID;
	const static int ourMinModelID		= 2;
	const static int ourMaxModelID		= 5000;
	const static int ourMinAnimationID	= 5001;
	const static int ourMaxAnimationID	= 9999;
};

