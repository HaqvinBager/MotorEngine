#include "stdafx.h"
#include "StringIDLoader.h"

//#define STRING_ID_MIN 2
//#define STRING_ID_MAX 599999

#include <string>
#include <cassert>
#include <fstream>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

const std::string CStringIDLoader::ourModelStringIDFile		= "Json/StringIDs_Models.json";
const std::string CStringIDLoader::ourAnimationStringIDFile	= "Json/StringIDs_Animation.json";
const std::string CStringIDLoader::ourNextAvailableID		= "Next Available ID";

const int CStringIDLoader::GetStringID(const std::string& aString, const std::string& aStringIDFile)
{
	using namespace rapidjson;

	int stringID = ourErrorID;
	
	//std::string idFile = "StringID.json";
	std::ifstream inFileStream(aStringIDFile.c_str());
	if (!inFileStream.is_open())
	{
		//CreateFile(aStringIDFile);
		
		const int minID = 2;

		StringBuffer stringBuffer;
		PrettyWriter<StringBuffer> prettyWriter(stringBuffer);
		prettyWriter.StartObject();
		prettyWriter.Key(ourNextAvailableID.c_str());
		prettyWriter.Int(minID);
		prettyWriter.EndObject();
		
		std::ofstream outFileStream(aStringIDFile.c_str());
		outFileStream << stringBuffer.GetString();
		
		outFileStream.close();
		inFileStream.open(aStringIDFile.c_str());
	}

	assert(inFileStream.is_open());
	IStreamWrapper inputStreamWrapper(inFileStream);
	Document document;
	document.ParseStream(inputStreamWrapper);

	assert(document.IsObject());
	stringID = document[ourNextAvailableID.c_str()].GetInt();
	for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr)
	{
		std::string name = itr->name.GetString();
		if (name == aString)
		{
			stringID = itr->value.GetInt();
			return stringID;
		}
	}

	// Copy contents of document and increase "Next Available ID"
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

	std::ofstream outFileStream(aStringIDFile.c_str());
	outFileStream << stringBuffer.GetString();
	outFileStream.close();

	return stringID;
}

bool CStringIDLoader::CreateFile(const std::string& aStringIDFile)
{
	const int minID = 2;

	using namespace rapidjson;
	StringBuffer stringBuffer;
	PrettyWriter<StringBuffer> prettyWriter(stringBuffer);
	prettyWriter.StartObject();
	prettyWriter.Key(ourNextAvailableID.c_str());
	prettyWriter.Int(minID);
	prettyWriter.EndObject();

	std::ofstream outFileStream(aStringIDFile.c_str());
	outFileStream << stringBuffer.GetString();

	outFileStream.close();

	return true;
}