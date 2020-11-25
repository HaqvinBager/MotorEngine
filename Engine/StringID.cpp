#include "stdafx.h"
#include "StringID.hpp"


CStringID::CStringID(const std::string& aString, const CStringIDLoader::EStringIDFiles anIDFileKey)
{
	myID = CStringIDLoader::GetStringID(aString, anIDFileKey);

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