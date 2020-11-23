#include "stdafx.h"
#include "StringID.hpp"

CStringID::CStringID(const std::string& aString)
	: myID(0)
{
	// Get ID from file. If it does not exist in file, add it to the end of file

#ifdef _DEBUG
	myString = aString;
#endif
}

CStringID::~CStringID()
{}

const int CStringID::ID() const
{
	return myID;
}

#ifdef _DEBUG
const std::string& CStringID::String() const
{
	return myString;
}
#endif