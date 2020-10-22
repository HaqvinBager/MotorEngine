#include "stdafx.h"
#include "ModelFactory.h"
#include <d3d11.h>
#include <fstream>
#include "..\DirectXTK\Inc\DDSTextureLoader.h"
#include "..\ModelLoader\FBXLoaderCustom.h"

#ifdef _DEBUG
#pragma comment (lib, "ModelLoader_Debug.lib")
#else
#pragma comment (lib, "ModelLoader_Release.lib")
#endif

CModelFactory* CModelFactory::GetInstance() {
	static CModelFactory* instance = new CModelFactory();
	return instance;
}

CModelFactory::CModelFactory() : myDevice(nullptr) {
}

CModelFactory::~CModelFactory() {
}

void CModelFactory::Init(ID3D11Device* aDevice) {
	myDevice = aDevice;
}

//CModel* CModelFactory::LoadModel(std::string aFilePath) { Triangle
//	HRESULT result;
//
//	//Start Vertex
//	struct Vertex {
//		float x, y, z, w;
//		float r, g, b, a;
//	} vertices[3] = {
//		{-0.8f, -0.8f, 0, 1, 1, 0, 0, 1},
//		{0.0f, 0.8f, 0, 1, 0, 1, 0, 1},
//		{0.8f, -0.8f, 0, 1, 0, 0, 1, 1}
//	};
//
//	D3D11_BUFFER_DESC bufferDescription = { 0 };
//	bufferDescription.ByteWidth = sizeof(vertices);
//	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
//	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//	D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
//	subresourceData.pSysMem = vertices;
//
//	ID3D11Buffer* vertexBuffer;
//	result = myDevice->CreateBuffer(&bufferDescription, &subresourceData, &vertexBuffer);
//	if (FAILED(result)) {
//		return nullptr;
//	}
//	//End Vertex
//	
//
//	//Start Shader
//	std::ifstream vsFile;
//	vsFile.open("VertexShader.cso", std::ios::binary);
//	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
//	ID3D11VertexShader* vertexShader;
//	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
//	if (FAILED(result)) {
//		return nullptr;
//	}
//	vsFile.close();
//
//	std::ifstream psFile;
//	psFile.open("PixelShader.cso", std::ios::binary);
//	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
//	ID3D11PixelShader* pixelShader;
//	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
//	if (FAILED(result)) {
//		return nullptr;
//	}
//	psFile.close();
//	//End Shader
//
//	//Start Layout
//	D3D11_INPUT_ELEMENT_DESC layout[] = {
//		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{"COLOR", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
//	};
//
//	ID3D11InputLayout* inputLayout;
//	result = myDevice->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &inputLayout);
//	if (FAILED(result)) {
//		return nullptr;
//	}
//	//End Layout
//
//	CModel* model = new CModel();
//	if (!model) {
//		return nullptr;
//	}
//
//	CModel::SModelData modelData;
//	modelData.myNumberOfVertecies = sizeof(vertices) / sizeof(Vertex);
//	modelData.myStride = sizeof(Vertex);
//	modelData.myOffset = 0;
//	modelData.myVertexBuffer = vertexBuffer;
//	modelData.myVertexShader = vertexShader;
//	modelData.myPixelShader = pixelShader;
//	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	modelData.myInputLayout = inputLayout;
//	model->Init(modelData);
//
//	myModelMap.emplace(aFilePath, model);
//
//	return model;
//}

CModel* CModelFactory::LoadModel(std::string aFilePath) {
	HRESULT result;

	CFBXLoaderCustom modelLoader;
	CLoaderModel* loaderModel = modelLoader.LoadModel(aFilePath.c_str());
	CLoaderMesh* mesh = loaderModel->myMeshes[0];

	//Start Vertex
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = mesh->myVertexBufferSize * mesh->myVertexCount;
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
	subresourceData.pSysMem = mesh->myVerticies;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &subresourceData, &vertexBuffer);
	if (FAILED(result)) {
		return nullptr;
	}

	D3D11_BUFFER_DESC indexBufferDescription = { 0 };
	indexBufferDescription.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(mesh->myIndexes.size());
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA indexSubsourceData = { 0 };
	indexSubsourceData.pSysMem = mesh->myIndexes.data();

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexSubsourceData, &indexBuffer);

	if (FAILED(result)) {
		return nullptr;
	}
	//End Vertex

	//Start Shader
	std::ifstream vsFile;
	vsFile.open("VertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result)) {
		return nullptr;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("PixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	if (FAILED(result)) {
		return nullptr;
	}
	psFile.close();
	//End Shader

	//Start Sampler
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	result = myDevice->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(result)) {
		return nullptr;
	}
	//End Sampler

	//Start Layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BONEID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result)) {
		return nullptr;
	}
	//End Layout

	//Start Textures
	std::string directory;

	for (size_t i = aFilePath.size() - 1; i > 0; --i) {
		if (aFilePath[i] == '/') {
			directory = aFilePath.substr(0, i + 1);
			break;
		}
	}

	//std::string diffuse = directory + loaderModel->myTextures[];
	std::string diffuse = directory + "Particle_Chest_D.dds";
	std::wstring diffuseFilePath(diffuse.begin(), diffuse.end());
	ID3D11ShaderResourceView* diffuseShaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myDevice, diffuseFilePath.c_str(), nullptr, &diffuseShaderResourceView);
	if (FAILED(result)) {
		return nullptr;
	}

	//std::string material = directory + loaderModel->myTextures[];
	std::string material = directory + "Particle_Chest_M.dds";
	std::wstring materialFilePath(material.begin(), material.end());
	ID3D11ShaderResourceView* materialShaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myDevice, materialFilePath.c_str(), nullptr, &materialShaderResourceView);
	if (FAILED(result)) {
		return nullptr;
	}

	//std::string normal = directory + loaderModel->myTextures[];
	std::string normal = directory + "Particle_Chest_N.dds";
	std::wstring normalFilePath(normal.begin(), normal.end());
	ID3D11ShaderResourceView* normalShaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myDevice, normalFilePath.c_str(), nullptr, &normalShaderResourceView);
	if (FAILED(result)) {
		return nullptr;
	}
	//End Textures

	CModel* model = new CModel();
	if (!model) {
		return nullptr;
	}

	CModel::SModelData modelData;
	modelData.myNumberOfVertecies = mesh->myVertexCount;
	modelData.myNumberOfIndices = static_cast<UINT>(mesh->myIndexes.size());
	modelData.myStride = mesh->myVertexBufferSize;
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	modelData.myTexture[0] = diffuseShaderResourceView;
	modelData.myTexture[1] = materialShaderResourceView;
	modelData.myTexture[2] = normalShaderResourceView;
	modelData.mySampler = sampler;
	model->Init(modelData);

	myModelMap.emplace(aFilePath, model);

	return model;
}

CModel* CModelFactory::GetCube() {
	HRESULT result;

	constexpr float tintR = 1.0f;
	constexpr float tintG = 1.0f;
	constexpr float tintB = 1.0f;
	constexpr float tintA = 1.0f;

	//Start Vertex
	struct Vertex {
		float x, y, z, w;
		float r, g, b, a;
		float u, v;
	} vertices[24] =
	{
		{ -1, -1, -1,		1,       tintR, tintG, tintB, tintA,       0, 0 },
		{  1, -1, -1,		1,       tintR, tintG, tintB, tintA,       1, 0 },
		{ -1,  1, -1,		1,       tintR, tintG, tintB, tintA,       0, 1 },
		{  1,  1, -1,		1,       tintR, tintG, tintB, tintA,       1, 1 },
		{ -1, -1,  1,		1,       tintR, tintG, tintB, tintA,       0, 0 },
		{  1, -1,  1,		1,       tintR, tintG, tintB, tintA,       1, 0 },
		{ -1,  1,  1,		1,       tintR, tintG, tintB, tintA,       0, 1 },
		{  1,  1,  1,		1,       tintR, tintG, tintB, tintA,       1, 1 },

		{ -1, -1, -1,		1,       tintR, tintG, tintB, tintA,       0, 0 },
		{ -1,  1, -1,		1,       tintR, tintG, tintB, tintA,       1, 0 },
		{ -1, -1,  1,		1,       tintR, tintG, tintB, tintA,       0, 1 },
		{ -1,  1,  1,		1,       tintR, tintG, tintB, tintA,       1, 1 },
		{  1, -1, -1,		1,       tintR, tintG, tintB, tintA,       0, 0 },
		{  1,  1, -1,		1,       tintR, tintG, tintB, tintA,       1, 0 },
		{  1, -1,  1,		1,       tintR, tintG, tintB, tintA,       0, 1 },
		{  1,  1,  1,		1,       tintR, tintG, tintB, tintA,       1, 1 },

		{ -1, -1, -1,		1,       tintR, tintG, tintB, tintA,       0, 0 },
		{  1, -1, -1,		1,       tintR, tintG, tintB, tintA,       1, 0 },
		{ -1, -1,  1,		1,       tintR, tintG, tintB, tintA,       0, 1 },
		{  1, -1,  1,		1,       tintR, tintG, tintB, tintA,       1, 1 },
		{ -1,  1, -1,		1,       tintR, tintG, tintB, tintA,       0, 0 },
		{  1,  1, -1,		1,       tintR, tintG, tintB, tintA,       1, 0 },
		{ -1,  1,  1,		1,       tintR, tintG, tintB, tintA,       0, 1 },
		{  1,  1,  1,		1,       tintR, tintG, tintB, tintA,       1, 1 }
	};
	unsigned int indices[36] = {
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

	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.ByteWidth = sizeof(vertices);
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
	subresourceData.pSysMem = vertices;

	ID3D11Buffer* vertexBuffer;
	result = myDevice->CreateBuffer(&bufferDescription, &subresourceData, &vertexBuffer);
	if (FAILED(result)) {
		return nullptr;
	}

	D3D11_BUFFER_DESC indexBufferDescription = { 0 };
	indexBufferDescription.ByteWidth = sizeof(indices);
	indexBufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA indexSubsourceData = { 0 };
	indexSubsourceData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	result = myDevice->CreateBuffer(&indexBufferDescription, &indexSubsourceData, &indexBuffer);

	if (FAILED(result)) {
		return nullptr;
	}
	//End Vertex


	//Start Shader
	std::ifstream vsFile;
	vsFile.open("CubeVertexShader.cso", std::ios::binary);
	std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
	ID3D11VertexShader* vertexShader;
	result = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
	if (FAILED(result)) {
		return nullptr;
	}
	vsFile.close();

	std::ifstream psFile;
	psFile.open("CubePixelShader.cso", std::ios::binary);
	std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
	ID3D11PixelShader* pixelShader;
	result = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
	if (FAILED(result)) {
		return nullptr;
	}
	psFile.close();
	//End Shader

	//Start Sampler
	ID3D11SamplerState* sampler;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	result = myDevice->CreateSamplerState(&samplerDesc, &sampler);
	if (FAILED(result)) {
		return nullptr;
	}
	//End Sampler

	//Start Layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0,  DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ID3D11InputLayout* inputLayout;
	result = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
	if (FAILED(result)) {
		return nullptr;
	}
	//End Layout

	//Start Textures
	std::wstring filename = L"Texture.dds";
	ID3D11ShaderResourceView* shaderResourceView;
	result = DirectX::CreateDDSTextureFromFile(myDevice, filename.c_str(), nullptr, &shaderResourceView);
	if (FAILED(result)) {
		return nullptr;
	}
	//End Textures

	CModel* model = new CModel();
	if (!model) {
		return nullptr;
	}

	CModel::SModelData modelData;
	modelData.myNumberOfVertecies = sizeof(vertices) / sizeof(Vertex);
	modelData.myNumberOfIndices = sizeof(indices) / sizeof(unsigned int);
	modelData.myStride = sizeof(Vertex);
	modelData.myOffset = 0;
	modelData.myVertexBuffer = vertexBuffer;
	modelData.myIndexBuffer = indexBuffer;
	modelData.myVertexShader = vertexShader;
	modelData.myPixelShader = pixelShader;
	modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	modelData.myInputLayout = inputLayout;
	modelData.myTexture[0] = shaderResourceView;
	modelData.myTexture[1] = nullptr;
	modelData.myTexture[2] = nullptr;
	modelData.mySampler = sampler;
	model->Init(modelData);

	return model;
}

CModel* CModelFactory::GetModel(std::string aFilePath) {
	if (myModelMap.find(aFilePath) != myModelMap.end()) {
		return myModelMap[aFilePath];
	}
	return nullptr;
}

