#pragma once


#include <tchar.h>

class CDllMain
{
public:
	CDllMain();
	static void Create();
	static CDllMain* myInstance;
	void* LoadModel(const char* aModelPath);

	class CFBXLoaderCustom* myLoader;
};

#define DLL_EXPORT extern "C" __declspec(dllexport) 

DLL_EXPORT void InitDll();
DLL_EXPORT void* LoadModelCustom(const char* aModelPath);