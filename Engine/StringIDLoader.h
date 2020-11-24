#pragma once

#define StringID_MinModelID 1
#define StringID_MaxModelID 5000
#define StringID_MinAnimID 5001
#define StringID_MaxAnimID 9999

class CStringIDLoader
{
public:
	CStringIDLoader() {};
	~CStringIDLoader() {};
	
	static const int GetStringID(const std::string& aString, const std::string& aStringIDFile);

public:
	const static std::string ourModelStringIDFile;
	const static std::string ourAnimationStringIDFile;
	const static int ourErrorID = -1;

private:
	static bool CreateFile(const std::string& aStringIDFile);

	const static std::string ourNextAvailableID;
};

