#include "stdafx.h"
#include "FileManager.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"

#include <fstream>

using namespace rapidjson;

std::string FileManager::LoadJsonString(const char* file, const char* key) {
	std::ifstream input_stream(file);
	IStreamWrapper input_wrapper(input_stream);
	Document document;
	document.ParseStream(input_wrapper);
	std::string value = document[key].GetString();
	return value;
}

float FileManager::LoadJsonFloat(const char* file, const char* key) {
	std::ifstream input_stream(file);
	IStreamWrapper input_wrapper(input_stream);
	Document document;
	document.ParseStream(input_wrapper);
	float value = document[key].GetFloat();
	return value;
}

int FileManager::LoadJsonInt(const char* file, const char* key) {
	std::ifstream input_stream(file);
	IStreamWrapper input_wrapper(input_stream);
	Document document;
	document.ParseStream(input_wrapper);
	int value = document[key].GetInt();
	return value;
}

bool FileManager::LoadJsonBool(const char *file, const char *key) {
	std::ifstream input_stream(file);
	IStreamWrapper input_wrapper(input_stream);
	Document document;
	document.ParseStream(input_wrapper);
	bool value = document[key].GetBool();
	return value;
}

rapidjson::Document FileManager::GetJsonDocument(const char *filename) {
	std::ifstream input_stream(filename);
	IStreamWrapper input_wrapper(input_stream);
	Document document;
	document.ParseStream(input_wrapper);
	return document;
}

void FileManager::SwapJsonValues(const char* file, const char* key1, const char* key2) {
	std::ifstream input_stream(file);
	IStreamWrapper input_wrapper(input_stream);
	Document document;
	document.ParseStream(input_wrapper);

	//FILE* fp = fopen(file, "wb"); // non-Windows use "w"
	FILE* fp;
	fopen_s(&fp, file, "wb");
	char writeBuffer[200];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);

	auto current = document.FindMember(key1);
	auto other = document.FindMember(key2);
	current->value.Swap(other->value);

	document.Accept(writer);
	fclose(fp);
}

//void FileManager::write_float(const char* file, const char* key, float element) {
//	FILE* fp;
//	fopen_s(&fp, file, "r");
//	char readBuffer[200];
//	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
//
//	Document d, d2;
//	d.ParseStream(is);
//	fclose(fp);
//
//	if (!d.HasMember(key)) {
//		d2.SetObject();
//		Value json_objects(kObjectType);
//		json_objects.AddMember(rapidjson::GenericStringRef(key), element, d2.GetAllocator());
//		d.AddMember(rapidjson::GenericStringRef(key), element, d2.GetAllocator());
//	} else {
//		d[key] = element;
//	}
//
//	FILE* outfile;
//	fopen_s(&outfile, file, "w");
//	char writeBuffer[200];
//	FileWriteStream os(outfile, writeBuffer, sizeof(writeBuffer));
//
//	Writer writer(os);
//	d.Accept(writer);
//	fclose(outfile);
//}

//void FileManager::write_int(const char* file, const char* key, int element) {
//	FILE* fp;
//	fopen_s(&fp, file, "r");
//	char readBuffer[200];
//	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
//
//	Document d, d2;
//	d.ParseStream(is);
//	fclose(fp);
//	
//	if (!d.HasMember(key)) {
//		d2.SetObject();
//		Value json_objects(kObjectType);
//		json_objects.AddMember(rapidjson::GenericStringRef(key), element, d2.GetAllocator());
//		d.AddMember(rapidjson::GenericStringRef(key), element, d2.GetAllocator());
//	} else {
//		d[key] = element;
//	}
//
//	FILE* outfile;
//	fopen_s(&outfile, file, "w");
//	char writeBuffer[200];
//	FileWriteStream os(outfile, writeBuffer, sizeof(writeBuffer));
//
//	Writer writer(os);
//	d.Accept(writer);
//	fclose(outfile);
//}
//
//void FileManager::write_bool(const char* file, const char* key, bool element) {
//	
//	FILE* fp;
//	fopen_s(&fp, file, "r");
//	char readBuffer[200];
//	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
//
//	Document d, d2;
//	d.ParseStream(is);
//	fclose(fp);
//
//	if (!d.HasMember(key)) {
//		d2.SetObject();
//		Value json_objects(kObjectType);
//		json_objects.AddMember(rapidjson::GenericStringRef(key), element, d2.GetAllocator());
//		d.AddMember(rapidjson::GenericStringRef(key), element, d2.GetAllocator());
//	} else {
//		d[key] = element;
//	}
//
//	FILE* outfile;
//	fopen_s(&outfile, file, "w");
//	char writeBuffer[200];
//	FileWriteStream os(outfile, writeBuffer, sizeof(writeBuffer));
//
//	Writer writer(os);
//	d.Accept(writer);
//	fclose(outfile);
//}
