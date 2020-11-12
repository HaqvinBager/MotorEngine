#include "CUnityLoader.h"

int main()
{
	CUnityLoader loader;
	//loader.LoadModels("modelPaths.txt");

	std::vector<ObjectData> fromAscii = loader.LoadGameObjects("exportedLevelASCII.txt", EReadMode_ASCII);
	std::vector<ObjectData> fromBinary = loader.LoadGameObjects("exportedLevelbin.bin", EReadMode_Binary);

	// Do your magic with the data :)

	return 0;
}