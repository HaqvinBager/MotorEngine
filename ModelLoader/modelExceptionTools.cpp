#include "modelExceptionTools.h"

namespace ModelExceptionTools
{
	bool IsDestructibleModel(const std::string& aModelPath)
	{
		// Ex : EN_P_L1Crate_01_19G4_01_19_DX_SK.fbx
		return (aModelPath.substr(aModelPath.length() - 10, 3) == "_DX");
	}
}