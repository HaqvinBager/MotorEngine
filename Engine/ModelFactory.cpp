#include "stdafx.h"
#include "ModelFactory.h"
#include "Model.h"
#include "Engine.h"
#include <d3d11.h>
#include <fstream>
#include "DDSTextureLoader.h"
#include "FBXLoaderCustom.h"
#include "ModelMath.h"


#ifdef _DEBUG
#pragma comment(lib, "ModelLoader_Debug.lib")
#endif
#ifdef NDEBUG
#pragma comment(lib, "ModelLoader_Release.lib")
#endif

#define TRIMSHEET_STRING "ts_"
#define NUM_TRIM_SHEETS 2
#define TRIMSHEET_1 "ts_1_Dungeon"
#define TRIMSHEET_2 ""
#define TRIMSHEET_PATH "Assets/Trimsheets/"	// group 4
//#define TRIMSHEET_PATH "Model\\Trimsheet_test\\Trimsheets\\"		// group 3 ...? is it Assets/ now?


CModelFactory* CModelFactory::ourInstance = nullptr;
CModelFactory* CModelFactory::GetInstance()
{
	return ourInstance;
}

CModelFactory::CModelFactory() : myEngine(nullptr)
{
	ourInstance = this;
	myEngine = nullptr;
}

CModelFactory::~CModelFactory()
{
	ourInstance = nullptr;
	myEngine = nullptr;
}


bool CModelFactory::Init(CEngine& engine)
{
	myEngine = &engine;
	return true;
}

CModel* CModelFactory::GetModel(std::string aFilePath)
{
	if (myModelMap.find(aFilePath) == myModelMap.end())
	{
		return LoadModel(aFilePath);
	}
	return myModelMap.at(aFilePath);
}

CModel* CModelFactory::GetModelPBR(std::string aFilePath)
{
	if (myModelMapPBR.find(aFilePath) == myModelMapPBR.end())
	{
		return LoadModelPBR(aFilePath);
	}
	return myModelMapPBR.at(aFilePath);
}


CModel* CModelFactory::LoadModelPBR(std::string aFilePath)
{
	//HRESULT result;
	// THIS ABSOLUTELY AWESOME MATEY. BUT MAEK IT BETTER LATER LOL :):)??
	const size_t last_slash_idx = aFilePath.find_last_of("\\/");
	std::string modelDirectory = aFilePath.substr(0, last_slash_idx + 1);
	std::string modelName = aFilePath.substr(last_slash_idx + 1, aFilePath.size() - last_slash_idx - 5);

	CFBXLoaderCustom modelLoader;
	CLoaderModel* loaderModel = modelLoader.LoadModel(aFilePath.c_str());
	ENGINE_ERROR_BOOL_MESSAGE(loaderModel, aFilePath.append(" could not be loaded.").c_str());

	CLoaderMesh* mesh = loaderModel->myMeshes[0];
	
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = mesh->myVertexBufferSize * mesh->myVertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subVertexResourceData = { 0 };
	subVertexResourceData.pSysMem = mesh->myVerticies;

	if (vertexBufferDesc.ByteWidth == 0) {
		return nullptr;
	}
	ID3D11Buffer* vertexBuffer;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateBuffer(&vertexBufferDesc, &subVertexResourceData, &vertexBuffer), "Vertex Buffer could not be created.");

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(mesh->myIndexes.size());
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subIndexResourceData = { 0 };
	subIndexResourceData.pSysMem = mesh->myIndexes.data();

	ID3D11Buffer* indexBuffer;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateBuffer(&indexBufferDesc, &subIndexResourceData, &indexBuffer), "Index Buffer could not be created.");

	//VertexShader
	std::ifstream vsFile;
	if (mesh->myModel->myNumBones > 0)
	{
		vsFile.open("AnimatedVertexShader.cso", std::ios::binary);
	}
	else {
		vsFile.open("VertexShader.cso", std::ios::binary);
	}
	
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader), "Vertex Shader could not be created.");

	vsFile.close();


	//PixelShader
	std::ifstream psFile;
	psFile.open("PBRPixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	ID3D11PixelShader* pixelShader;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader), "Pixel Shader could not be created.");

	psFile.close();

	//Sampler
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler), "Sampler State could not be created.");

	//Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11InputLayout* inputLayout;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout), "Input Layout could not be created.");

	ID3D11Device* device = myEngine->myFramework->GetDevice();
	std::string modelDirectoryAndName = modelDirectory + modelName;

	//UPDATE THIS ON MONDAY
	//std::map<int, std::string> trimsheets;
	//trimsheets.emplace(static_cast<int>('1'), "ts_1_Dungeon");
	//trimsheets.emplace(static_cast<int>('2'), "ts_2_Something");

	// Check if model uses trimsheet.
	// suffix ts_#
	std::string suffix = aFilePath.substr(aFilePath.length() - 8, 4);
	if (suffix.substr(0, 3) == TRIMSHEET_STRING)
	{
														// Info
		int suffixNr	= static_cast<int>(suffix[3]);	// std::string suffix = "ts_1". "ts_#" ; # = an integer
		suffixNr		= abs(49 - suffixNr);			// 49 == static_cast<int>('1'). The ASCII value of '1' is 49. '1' == 49, '2' == 50, '9' == 58 => 49 - (int)'2' = -1 and 49 - '3' = -2
		if (suffixNr >= 0/*static_cast<int>(MIN_NUM_TRIMSHEETS_CHAR)*/ && suffixNr <= NUM_TRIM_SHEETS/*static_cast<int>(MAX_NUM_TRIMSHEETS_CHAR)*/)
		{
			std::array<std::string, NUM_TRIM_SHEETS> trimsheets = { TRIMSHEET_1, TRIMSHEET_2 };
			modelDirectoryAndName = TRIMSHEET_PATH + trimsheets[suffixNr];
		}
	}											
	// ! Check if model uses trimsheet

	ID3D11ShaderResourceView* diffuseResourceView = GetShaderResourceView(device, /*TexturePathWide*/(modelDirectoryAndName + "_D.dds"));
	ID3D11ShaderResourceView* materialResourceView = GetShaderResourceView(device, /*TexturePathWide*/(modelDirectoryAndName + "_M.dds"));
	ID3D11ShaderResourceView* normalResourceView = GetShaderResourceView(device, /*TexturePathWide*/(modelDirectoryAndName + "_N.dds"));

	//ID3D11ShaderResourceView* metalnessShaderResourceView = GetShaderResourceView(device, TexturePathWide(modelDirectory + loaderModel->myTextures[10]));
	//ID3D11ShaderResourceView* roughnessShaderResourceView = GetShaderResourceView(device, TexturePathWide(modelDirectory + loaderModel->myTextures[1]));
	//ID3D11ShaderResourceView* ambientShaderResourceView = GetShaderResourceView(device, TexturePathWide(modelDirectory + loaderModel->myTextures[2]));
	//ID3D11ShaderResourceView* emissiveShaderResourceView = GetShaderResourceView(device, TexturePathWide(modelDirectory + loaderModel->myTextures[3]));

	//Model
	CModel* model = new CModel();
	ENGINE_ERROR_BOOL_MESSAGE(model, "Empty model could not be loaded.");

	CModel::SModelData modelData;
	modelData.myNumberOfVertices = mesh->myVertexCount;
	modelData.myNumberOfIndices = static_cast<UINT>(mesh->myIndexes.size());
	modelData.myStride = mesh->myVertexBufferSize;
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.mySamplerState = sampler;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	modelData.myTexture[0] = diffuseResourceView;
	modelData.myTexture[1] = materialResourceView;
	modelData.myTexture[2] = normalResourceView;

	//if (mesh->myModel->myNumBones > 0)
	//{
	//	modelData.myBonesBuffer = bonesBuffer;
	//}
	//modelData.myAnimations

	//modelData.myTexture[3] = roughnessShaderResourceView;
	//modelData.myTexture[4] = ambientShaderResourceView;
	//modelData.myTexture[5] = emissiveShaderResourceView;

	model->Init(modelData);

	myModelMapPBR.emplace(aFilePath, model);
	return model;
}

CModel* CModelFactory::LoadModel(std::string aFilePath)
{
	const size_t last_slash_idx = aFilePath.find_last_of("\\/");
	std::string modelDirectory = aFilePath.substr(0, last_slash_idx + 1);

	CFBXLoaderCustom modelLoader;
	CLoaderModel* loaderModel = modelLoader.LoadModel(aFilePath.c_str());
	CLoaderMesh* mesh = loaderModel->myMeshes[0];

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = mesh->myVertexBufferSize * mesh->myVertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subVertexResourceData = { 0 };
	subVertexResourceData.pSysMem = mesh->myVerticies;

	ID3D11Buffer* vertexBuffer;

	if (vertexBufferDesc.ByteWidth == 0) {
		return nullptr;
	}
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateBuffer(&vertexBufferDesc, &subVertexResourceData, &vertexBuffer), "Vertex Buffer could not be created.");

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(mesh->myIndexes.size());
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subIndexResourceData = { 0 };
	subIndexResourceData.pSysMem = mesh->myIndexes.data();

	ID3D11Buffer* indexBuffer;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateBuffer(&indexBufferDesc, &subIndexResourceData, &indexBuffer), "Index Buffer could not be created.");

	//VertexShader
	std::ifstream vsFile;
	vsFile.open("VertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };

	ID3D11VertexShader* vertexShader;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader), "Vertex Shader could not be created.");

	vsFile.close();

	//PixelShader
	std::ifstream psFile;
	psFile.open("PixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };

	ID3D11PixelShader* pixelShader;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader), "Pixel Shader could not be created.");

	psFile.close();

	//Sampler
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler), "Sampler State could not be created.");

	//Layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11InputLayout* inputLayout;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout), "Input Layout could not be created.");

	ID3D11Device* device = myEngine->myFramework->GetDevice();
	ID3D11ShaderResourceView* albedoResourceView = GetShaderResourceView(device, /*TexturePathWide*/(modelDirectory + loaderModel->myTextures[0]));
	ID3D11ShaderResourceView* normalResourceView = GetShaderResourceView(device, /*TexturePathWide*/(modelDirectory + loaderModel->myTextures[5]));

	//Model
	CModel* model = new CModel();
	ENGINE_ERROR_BOOL_MESSAGE(model, "Empty model could not be loaded.");

	CModel::SModelData modelData;
	modelData.myNumberOfVertices = mesh->myVertexCount;
	modelData.myNumberOfIndices = static_cast<UINT>(mesh->myIndexes.size());
	modelData.myStride = mesh->myVertexBufferSize;
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.mySamplerState = sampler;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	modelData.myTexture[0] = albedoResourceView;
	modelData.myTexture[1] = normalResourceView;
	//modelData.myTexture[2] = metalnessShaderResourceView;
	//modelData.myTexture[3] = roughnessShaderResourceView;
	//modelData.myTexture[4] = ambientShaderResourceView;
	//modelData.myTexture[5] = emissiveShaderResourceView;

	model->Init(modelData);

	myModelMap.emplace(aFilePath, model);
	return model;
}

CModel* CModelFactory::GetCube()
{
	//Vertex Setup
	struct Vertex
	{
		float x, y, z, w;
		float r, g, b, a;
		float u, v;
	} verticies[24] = {
		//X Y Z		      W		   RGBA			     UV	
		{ -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 },
		{  1, -1, -1,     1,       1, 1, 0, 1,       1, 0 },
		{ -1,  1, -1,     1,       1, 0, 1, 1,       0, 1 },
		{  1,  1, -1,     1,       0, 1, 1, 1,       1, 1 },
		{ -1, -1,  1,     1,       1, 0, 0, 1,       0, 0 },
		{  1, -1,  1,     1,       0, 1, 0, 1,       1, 0 },
		{ -1,  1,  1,     1,       0, 0, 1, 1,       0, 1 },
		{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 },
		//X Y Z		      W		   RGBA				 UV	
		{ -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 },
		{ -1,  1, -1,     1,       1, 1, 0, 1,       1, 0 },
		{ -1, -1,  1,     1,       1, 0, 1, 1,       0, 1 },
		{ -1,  1,  1,     1,       0, 1, 1, 1,       1, 1 },
		{  1, -1, -1,     1,       1, 0, 0, 1,       0, 0 },
		{  1,  1, -1,     1,       0, 1, 0, 1,       1, 0 },
		{  1, -1,  1,     1,       0, 0, 1, 1,       0, 1 },
		{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 },
		//X Y Z		      W		   RGBA			     UV		
		{ -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 },
		{  1, -1, -1,     1,       1, 1, 0, 1,       1, 0 },
		{ -1, -1,  1,     1,       1, 0, 1, 1,       0, 1 },
		{  1, -1,  1,     1,       0, 1, 1, 1,       1, 1 },
		{ -1,  1, -1,     1,       1, 0, 0, 1,       0, 0 },
		{  1,  1, -1,     1,       0, 1, 0, 1,       1, 0 },
		{ -1,  1,  1,     1,       0, 0, 1, 1,       0, 1 },
		{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 }
	};
	//Index Setup
	unsigned int indicies[36] = {
		0,2,1,
		2,3,1,
		4,5,7,
		4,7,6,
		8,10,9,
		10,11,9,
		12,13,15,
		12,15,14,
		16,17,18,
		18,17,19,
		20,23,21,
		20,22,23
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(verticies);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResourceData = { 0 };
	subResourceData.pSysMem = verticies;

	ID3D11Buffer* vertexBuffer;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateBuffer(&vertexBufferDesc, &subResourceData, &vertexBuffer), "Vertex Buffer could not be created.");

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(indicies);
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA indexSubresourceData = { 0 };
	indexSubresourceData.pSysMem = indicies;

	ID3D11Buffer* indexBuffer;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer), "Index Buffer could not be created.");

	//VertexShader
	std::ifstream vsFile;
	vsFile.open("CubeVertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader), "Vertex Shader could not be created.");
	vsFile.close();

	//PixelShader
	std::ifstream psFile;
	psFile.open("CubePixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader), "Pixel Shader could not be created.");
	psFile.close();

	//Sampler
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler), "Sampler could not be created.");

	//Layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ID3D11InputLayout* inputLayout;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateInputLayout(layout, 3, vsData.data(), vsData.size(), &inputLayout), "Input Layout could not be created.");

	ID3D11ShaderResourceView* shaderResourceView = GetShaderResourceView(myEngine->myFramework->GetDevice(), "Texture.dds");

	//Model
	CModel* model = new CModel();
	ENGINE_ERROR_BOOL_MESSAGE(model, "Empty model could not be loaded.");

	CModel::SModelData modelData;
	modelData.myNumberOfVertices = sizeof(verticies) / sizeof(Vertex);
	modelData.myNumberOfIndices = sizeof(indicies) / sizeof(unsigned int);
	modelData.myStride = sizeof(Vertex);
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.mySamplerState = sampler;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	modelData.myTexture[0] = shaderResourceView;

	model->Init(modelData);
	myModelMap.emplace("Cube", model);
	return model;
}

CModel* CModelFactory::GetOutlineModelSubset()
{
	if (myOutlineModelSubset) {
		return myOutlineModelSubset;
	}

	//Start Shader
	std::ifstream vsFile;
	vsFile.open("OutlineVertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader), "Vertex Shader could not be created.");
	vsFile.close();

	std::ifstream psFile;
	psFile.open("OutlinePixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader), "Pixel Shader could not be created.");
	psFile.close();
	//End Shader

	myOutlineModelSubset = new CModel();

	CModel::SModelData modelData;
	modelData.myNumberOfVertices = 0;
	modelData.myNumberOfIndices = 0;
	modelData.myStride = 0;
	modelData.myOffset = 0;
	modelData.myVertexBuffer = nullptr;
	modelData.myIndexBuffer = nullptr;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.mySamplerState = nullptr;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = nullptr;
	modelData.myTexture[0] = nullptr;
	modelData.myTexture[1] = nullptr;
	modelData.myTexture[2] = nullptr;
	myOutlineModelSubset->Init(modelData);

	return myOutlineModelSubset;
}


ID3D11ShaderResourceView* CModelFactory::GetShaderResourceView(ID3D11Device* aDevice, std::string aTexturePath)
{
	ID3D11ShaderResourceView* shaderResourceView;

	wchar_t* widePath = new wchar_t[aTexturePath.length() + 1];
	std::copy(aTexturePath.begin(), aTexturePath.end(), widePath);
	widePath[aTexturePath.length()] = 0;

	////==ENABLE FOR TEXTURE CHECKING==
	//ENGINE_HR_MESSAGE(DirectX::CreateDDSTextureFromFile(aDevice, widePath, nullptr, &shaderResourceView), aTexturePath.append(" could not be found.").c_str());
	////===============================

	//==DISABLE FOR TEXTURE CHECKING==
	HRESULT result;
	result = DirectX::CreateDDSTextureFromFile(aDevice, widePath, nullptr, &shaderResourceView);
	if (FAILED(result))
		DirectX::CreateDDSTextureFromFile(aDevice, L"ErrorTexture.dds", nullptr, &shaderResourceView);
		//return nullptr;
	//================================

	delete[] widePath;
	return shaderResourceView;
}

//wchar_t* CModelFactory::TexturePathWide(std::string aTexturePath) const
//{
//	wchar_t* albedo_path_wide = new wchar_t[aTexturePath.length() + 1];
//	std::copy(aTexturePath.begin(), aTexturePath.end(), albedo_path_wide);
//	albedo_path_wide[aTexturePath.length()] = 0;
//	return std::move(albedo_path_wide);
//}


/*ID3D11ShaderResourceView* albedoResourceView;
std::string albedo_path = modelDirectory + loaderModel->myTextures[0];
wchar_t* albedo_path_wide = new wchar_t[albedo_path.length() + 1];
std::copy(albedo_path.begin(), albedo_path.end(), albedo_path_wide);
albedo_path_wide[albedo_path.length()] = 0;
result = DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), albedo_path_wide, nullptr, &albedoResourceView);
if (FAILED(result))
{
	return nullptr;
}
delete[] albedo_path_wide;


std::string normal_path = modelDirectory + loaderModel->myTextures[5];
wchar_t* normal_path_wide = new wchar_t[normal_path.length() + 1];
std::copy(normal_path.begin(), normal_path.end(), normal_path_wide);
normal_path_wide[normal_path.length()] = 0;

ID3D11ShaderResourceView* normalResourceView;
result = DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), normal_path_wide, nullptr, &normalResourceView);
if (FAILED(result))
	return nullptr;

delete[] normal_path_wide;*/
