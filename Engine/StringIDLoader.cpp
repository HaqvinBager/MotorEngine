#include "stdafx.h"
#include "StringIDLoader.h"

#include <string>
#include <cassert>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

const std::string CStringIDLoader::ourModelStringIDPath		= "Json/StringIDs_Models.json";
const std::string CStringIDLoader::ourAnimationStringIDPath	= "Json/StringIDs_Animation.json";
const std::string CStringIDLoader::ourNextAvailableID		= "Next Available ID";

const int CStringIDLoader::GetStringID(const std::string& aString, const CStringIDLoader::EStringIDFiles aFileKey)
{

	switch (aFileKey)
	{
		case EStringIDFiles::ModelFile:
		return GetStringIDFromFile(aString, ourModelStringIDPath, ourMinModelID, ourMaxModelID);
		break;
		case EStringIDFiles::AnimFile:
		return GetStringIDFromFile(aString, ourAnimationStringIDPath, ourMinAnimationID, ourMaxAnimationID);
		break;
		default:
		break;
	}
	return ourErrorID;

}

const int CStringIDLoader::GetStringIDFromFile(const std::string& aString, const std::string& anIDFilePath, const int aMinID, const int aMaxID)
{
	using namespace rapidjson;

	int stringID = ourErrorID;

	//std::string idFile = "StringID.json";
	std::ifstream inFileStream(anIDFilePath.c_str());
	if (!inFileStream.is_open())
	{
		StringBuffer stringBuffer;
		PrettyWriter<StringBuffer> prettyWriter(stringBuffer);
		prettyWriter.StartObject();
		prettyWriter.Key(ourNextAvailableID.c_str());
		prettyWriter.Int(aMinID);
		prettyWriter.EndObject();

		std::ofstream outFileStream(anIDFilePath.c_str());
		outFileStream << stringBuffer.GetString();

		outFileStream.close();
		inFileStream.open(anIDFilePath.c_str());
	}

	assert(inFileStream.is_open());
	IStreamWrapper inputStreamWrapper(inFileStream);
	Document document;
	document.ParseStream(inputStreamWrapper);

	assert(document.IsObject());
	stringID = document[ourNextAvailableID.c_str()].GetInt();
	for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		//std::string name = itr->name.GetString();
		if (itr->name.GetString() == aString)
		{
			stringID = itr->value.GetInt();
			return stringID;
		}
	}

	// Copy contents of document and increase "Next Available ID"
	if (stringID >= aMaxID)
	{
		assert(stringID >= aMaxID && "Available stringID has reached max, increase limit in StringIDLoader.h");
		return ourErrorID;
	}

	StringBuffer stringBuffer;
	PrettyWriter<StringBuffer> prettyWriter(stringBuffer);
	prettyWriter.StartObject();
	for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		prettyWriter.Key(itr->name.GetString());
		const int value = itr->value.GetInt();
		if (value == stringID)
			prettyWriter.Int(value + 1);
		else
			prettyWriter.Int(value);
	}
	inFileStream.close();
	// Add new string with its ID
	prettyWriter.Key(aString.c_str());
	prettyWriter.Int(stringID);
	prettyWriter.EndObject();

	std::ofstream outFileStream(anIDFilePath.c_str());
	outFileStream << stringBuffer.GetString();
	outFileStream.close();

	return stringID;
}