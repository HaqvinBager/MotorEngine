#pragma once
#include <string>
/// What to do:
///  Wrap std::string
/// In debug: returns ID but can get the string that is used
/// In release returns ID
/// ID is unique per string, ID is an int 2 - 59999
/// ID is saved in a document. The document is a rapid JSON file. Is accessed only at the start/ load.
///		The document has
///			model_path ex: "enemy_robot_idle.fbx"
///						id: 349.
/// Uses a singleton that reads the assets folder and checks for models?
/// Every model it finds it adds to the list, creating an ID for it that is unique (previous ID++)
/// 
/// StringID.json
///		"string1"
///			id : 2
///		"string2"
///			id : 3
///		"string3"
///			id : 4
///		"myNextAvailableID"
///			id : 5
/// 
class CStringID
{
public:
	CStringID(const std::string& aString);
	~CStringID();

	const std::string& String() const;
#ifdef  _DEBUG
#endif
	const int ID() const;

private:
	std::string myString;
#ifdef _DEBUG
#endif
	int myID;
};