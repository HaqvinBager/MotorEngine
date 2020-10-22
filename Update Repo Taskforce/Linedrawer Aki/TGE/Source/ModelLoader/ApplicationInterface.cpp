#include "ApplicationInterface.h"
#include "FBXLoaderCustom.h"
#include "ConstModelStructs.h"

CDllMain::CDllMain()
{
	myLoader = nullptr;
}

void CDllMain::Create()
{
	myInstance = new CDllMain();
}

CDllMain* CDllMain::myInstance = 0;

void* CDllMain::LoadModel(const char* aModelPath)
{
	if (!myLoader)
	{
		myLoader = new CFBXLoaderCustom();
	}

	SModelData* modeldata = new SModelData();
	CLoaderModel* loaderModel = myLoader->LoadModel(aModelPath);
	if (loaderModel)
	{
		modeldata->myIndexBufferSize = (unsigned int)loaderModel->myMeshes[0]->myIndexes.size() * sizeof(unsigned int);
		modeldata->myIndexCount = (unsigned int)loaderModel->myMeshes[0]->myIndexes.size();
		modeldata->myIndexes = &loaderModel->myMeshes[0]->myIndexes[0];
		modeldata->myScene = (void*)loaderModel->myScene;

		modeldata->myVertexBufferSize = loaderModel->myMeshes[0]->myVertexBufferSize;
		modeldata->myVertexCount = loaderModel->myMeshes[0]->myVertexCount;
		modeldata->myVertexes = loaderModel->myMeshes[0]->myVerticies;
		modeldata->myShaderType = loaderModel->myMeshes[0]->myShaderType;
	}

	return (void*)modeldata;
}

DLL_EXPORT void* LoadModelCustom(const char* aModelPath)
{
	if (!CDllMain::myInstance)
	{
		CDllMain::Create();
	}
	return CDllMain::myInstance->LoadModel(aModelPath);
}


DLL_EXPORT void InitDll()
{
	CDllMain::Create();
}
