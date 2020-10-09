#pragma once

// Do not change here!!

struct SModelData 
{
	unsigned int myVertexBufferSize;
	int myVertexCount;
	char* myVertexes;

	unsigned int myIndexBufferSize;
	unsigned int myIndexCount;
	unsigned int* myIndexes;
	int myShaderType;
	void* myScene;
	std::vector<std::string> myTextures;
};