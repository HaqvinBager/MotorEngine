#include "pch.h"
#include "Model.hpp"

//#include <d3dcommon.h>
//#include <d3d11.h>
//#pragma comment (lib, "d3d11.lib")

namespace ModelSystem
{
	Model::Model()
	{
		myModelData = {};
	}
	Model::~Model()
	{}

	void Model::Init( SModelData& aModelData )
	{
		myModelData = aModelData;
	}

	const Model::SModelData& Model::GetModelData()
	{
		return myModelData;
	}
	const Model::SModelData& Model::GetModelData() const
	{
		return myModelData;
	}
}