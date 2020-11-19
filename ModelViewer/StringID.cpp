#include "StringID.hpp"

StringID::StringID(const std::string& /*aString*/)
	: myID(0)
{
	// Get ID from file. If it does not exist in file, add it to the end of file

#ifdef _DEBUG
	myString = aString;
#endif
}

StringID::~StringID()
{}

const unsigned int StringID::ID() const
{
	return myID;
}

#ifdef _DEBUG
const std::string& StringID::String() const
{
	return myString;
}
#endif