#include "pch.h"
#include "ModelFactory.hpp"

#include "DirectX11Framework.hpp"
#include "Model.hpp"
//DirectXTK
#include "DDSTextureLoader.h"
//ModelLoader
#include "FBXLoaderCustom.h"

//#include <d3d11.h>
//#pragma comment (lib, "d3d11.lib")
#include <fstream>
namespace ModelSystem
{
	ModelFactory* ModelFactory::myInstance = nullptr;

	void ModelFactory::Create()
	{
		assert(myInstance == nullptr && "ModelFactory already created!");
		if( !myInstance )
		{
			myInstance = new ModelFactory();
		}
	}

	ModelFactory& ModelFactory::GetInstance()
	{
		assert(myInstance != nullptr && "Instance has not been created.");
		return *myInstance;
	}

	void ModelFactory::Init(DirectX11Framework* aFramework)
	{
		assert(myInstance != nullptr && "Instance has not been created.");
		if( !myInstance )
		{
			return;
		}
		// Members that need to be inited.
		myDevice = aFramework->GetDevice();
	}

	void ModelFactory::Destroy()
	{
		assert(myInstance != nullptr && "No instance exists to Destroy!");
		if( myInstance )
		{
			SAFE_DELETE(myInstance)
		}
	}

	Model* ModelFactory::GetExistingModel( const std::string& /*aModelPath*/ )
	{
		// Todo add container for already loaded models
		return nullptr;
	}

	Model* ModelFactory::LoadModel( const std::string& aModelPath)
	{
		HRESULT hResult;

		struct SVertex
		{
			float myX, myY, myZ, myW;
			float myR, myG, myB, myA;
			float myU, myV;
		};

		CFBXLoaderCustom fbxLoader;
		CLoaderModel* loaderModel = fbxLoader.LoadModel(aModelPath.c_str());
		CLoaderMesh* loaderMesh = loaderModel->myMeshes[0];
		
		//Vertex
		D3D11_BUFFER_DESC vertexBufferDescription = {0};
		vertexBufferDescription.ByteWidth	=	loaderMesh->myVertexBufferSize * loaderMesh->myVertexCount;
		vertexBufferDescription.Usage		=	D3D11_USAGE_IMMUTABLE;
		vertexBufferDescription.BindFlags	=	D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = {0};
		vertexSubresourceData.pSysMem = loaderMesh->myVerticies;

		ID3D11Buffer* vertexBuffer = nullptr;
		hResult = myDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &vertexBuffer);
		if(FAILED(hResult))
		{
			return nullptr;
		}
		//!Vertex

		//Index
		D3D11_BUFFER_DESC indexBufferDescription = {0};
		indexBufferDescription.ByteWidth	= sizeof(unsigned int) * static_cast<UINT>(loaderMesh->myIndexes.size());
		indexBufferDescription.Usage		= D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.BindFlags	= D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubresourceData = {0};
		indexSubresourceData.pSysMem = loaderMesh->myIndexes.data();

		ID3D11Buffer* indexBuffer = nullptr;
		hResult = myDevice->CreateBuffer(&indexBufferDescription, &indexSubresourceData, &indexBuffer);
		if (FAILED(hResult))
		{
			return nullptr;
		}
		//!Index
		//Shaders
		//	Vertex Shader
		std::ifstream vsFile;
		vsFile.open("VertexShader.cso",std::ios::binary);
		std::string vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};

		ID3D11VertexShader* vertexShader = nullptr;
		hResult = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		vsFile.close();
		//	!Vertex Shader
		//	Pixel Shader
		std::ifstream psFile;
		psFile.open("PixelShader.cso", std::ios::binary);
		std::string psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

		ID3D11PixelShader* pixelShader = nullptr;
		hResult = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		psFile.close();
		//	!Pixel Shader
		//!Shaders

		//Input Layout
		D3D11_INPUT_ELEMENT_DESC layout[] = 
		{
			 {"POSITION"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"NORMAL"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"TANGENT"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"BINORMAL"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"UV"			, 0, DXGI_FORMAT_R32G32_FLOAT	   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"BONEID"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"BONEWEIGHT"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			//,{"COLOR"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			
		};
		ID3D11InputLayout* inputLayout = nullptr;
		hResult = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		//!Input Layout

	//Texture loading
		size_t lastOfSlash = aModelPath.find_last_of("/");
		std::string texturePath(aModelPath.begin(), aModelPath.end() - (aModelPath.length() - lastOfSlash - 1));
	//Albedo
		std::string albedoPath = texturePath + loaderMesh->myModel->myTextures[0];
		std::wstring albedoFile(albedoPath.length(), L' ');
		std::copy(albedoPath.begin(), albedoPath.end(), albedoFile.begin());

		ID3D11ShaderResourceView* albedoShaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, albedoFile.c_str(), nullptr, &albedoShaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load albedo texture!");
			return nullptr;
		}
	//!Albedo

	//Normal
		std::string normalPath = texturePath + loaderMesh->myModel->myTextures[5];
		std::wstring normalFile(normalPath.length(), L' ');
		std::copy(normalPath.begin(), normalPath.end(), normalFile.begin());

		ID3D11ShaderResourceView* normalShaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, normalFile.c_str(), nullptr, &normalShaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load normal texture!");
			return nullptr;
		}
	//!Normal

	//Metalness
		std::string metalPath = texturePath + loaderMesh->myModel->myTextures[10];
		std::wstring metalFile(metalPath.length(), L' ');
		std::copy(metalPath.begin(), metalPath.end(), metalFile.begin());

		ID3D11ShaderResourceView* metalShaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, metalFile.c_str(), nullptr, &metalShaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load metalness texture!");
			return nullptr;
		}
	//!Metalness

	//Roughness
		std::string roughPath = texturePath + loaderMesh->myModel->myTextures[1];
		std::wstring roughFile(roughPath.length(), L' ');
		std::copy(roughPath.begin(), roughPath.end(), roughFile.begin());

		ID3D11ShaderResourceView* roughShaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, roughFile.c_str(), nullptr, &roughShaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load roughness texture!");
			return nullptr;
		}
	//!Roughness

	//Ambient Occlusion
		std::string aoPath = texturePath + loaderMesh->myModel->myTextures[2];
		std::wstring aoFile(aoPath.length(), L' ');
		std::copy(aoPath.begin(), aoPath.end(), aoFile.begin());

		ID3D11ShaderResourceView* aoShaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, aoFile.c_str(), nullptr, &aoShaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load ambient occlusion texture!");
			return nullptr;
		}
	//!Ambient Occlusion

	//Emissive
		std::string emissivePath = texturePath + loaderMesh->myModel->myTextures[3];
		std::wstring emissiveFile(emissivePath.length(), L' ');
		std::copy(emissivePath.begin(), emissivePath.end(), emissiveFile.begin());

		ID3D11ShaderResourceView* emissiveShaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, emissiveFile.c_str(), nullptr, &emissiveShaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load emissive texture!");
			return nullptr;
		}
	//!Emissive
	//!Texture loading

		Model* model = new Model();
		if( !model )
		{
			return nullptr;
		}

		Model::SModelData modelData;
		modelData.myNumberOfVertices    = (UINT)loaderMesh->myVertexCount;
		modelData.myNumberOfIndices		= static_cast<UINT>(loaderMesh->myIndexes.size());
		modelData.myStride				= loaderMesh->myVertexBufferSize;
		modelData.myOffset				= 0;
		modelData.myVertexBuffer		= vertexBuffer;
		modelData.myIndexBuffer			= indexBuffer;
		modelData.myVertexShader		= vertexShader;
		modelData.myPixelShader			= pixelShader;
		modelData.myPrimitiveTopology	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		modelData.myInputLayout			= inputLayout;
		modelData.myTextures.reserve(6/*loaderMesh->myModel->myTextures.size()*/);// 6!
		modelData.myTextures.shrink_to_fit();
		modelData.myTextures.emplace_back(albedoShaderResourceView);
		modelData.myTextures.emplace_back(normalShaderResourceView);
		modelData.myTextures.emplace_back(metalShaderResourceView);
		modelData.myTextures.emplace_back(roughShaderResourceView);
		modelData.myTextures.emplace_back(aoShaderResourceView);
		modelData.myTextures.emplace_back(emissiveShaderResourceView);
		model->Init(modelData);

		return model;
	}

	Model* ModelFactory::LoadModelTexturePacked( const std::string& aModelPath)
	{
		HRESULT hResult;

		struct SVertex
		{
			float myX, myY, myZ, myW;
			float myR, myG, myB, myA;
			float myU, myV;
		};

		CFBXLoaderCustom fbxLoader;
		CLoaderModel* loaderModel = fbxLoader.LoadModel(aModelPath.c_str());
		CLoaderMesh* loaderMesh = loaderModel->myMeshes[0];

		//Vertex
		D3D11_BUFFER_DESC vertexBufferDescription = {0};
		vertexBufferDescription.ByteWidth	=	loaderMesh->myVertexBufferSize * loaderMesh->myVertexCount;
		vertexBufferDescription.Usage		=	D3D11_USAGE_IMMUTABLE;
		vertexBufferDescription.BindFlags	=	D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = {0};
		vertexSubresourceData.pSysMem = loaderMesh->myVerticies;

		ID3D11Buffer* vertexBuffer = nullptr;
		hResult = myDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &vertexBuffer);
		if(FAILED(hResult))
		{
			return nullptr;
		}
		//!Vertex

		//Index
		D3D11_BUFFER_DESC indexBufferDescription = {0};
		indexBufferDescription.ByteWidth	= sizeof(unsigned int) * static_cast<UINT>(loaderMesh->myIndexes.size());
		indexBufferDescription.Usage		= D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.BindFlags	= D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexSubresourceData = {0};
		indexSubresourceData.pSysMem = loaderMesh->myIndexes.data();

		ID3D11Buffer* indexBuffer = nullptr;
		hResult = myDevice->CreateBuffer(&indexBufferDescription, &indexSubresourceData, &indexBuffer);
		if (FAILED(hResult))
		{
			return nullptr;
		}
		//!Index
		//Shaders
		//	Vertex Shader
		std::ifstream vsFile;
		vsFile.open("VertexShader.cso",std::ios::binary);
		std::string vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};

		ID3D11VertexShader* vertexShader = nullptr;
		hResult = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		vsFile.close();
		//	!Vertex Shader
		//	Pixel Shader
		std::ifstream psFile;
		psFile.open("PixelShader.cso", std::ios::binary);
		std::string psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

		ID3D11PixelShader* pixelShader = nullptr;
		hResult = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		psFile.close();
		//	!Pixel Shader
		//!Shaders

		//Input Layout
		D3D11_INPUT_ELEMENT_DESC layout[] = 
		{
			{"POSITION"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"NORMAL"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"TANGENT"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"BINORMAL"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"UV"			, 0, DXGI_FORMAT_R32G32_FLOAT	   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"BONEID"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"BONEWEIGHT"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			//,{"COLOR"		, 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}

		};
		ID3D11InputLayout* inputLayout = nullptr;
		hResult = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		//!Input Layout

		//Texture loading
		size_t lastOfSlash = aModelPath.find_last_of("/");
		std::string texturePath(aModelPath.begin(), aModelPath.end() - (aModelPath.length() - lastOfSlash - 1));
		//Albedo
		std::string albedoPath = texturePath/*"Textures/Characters/Enemy Robot/"*/ + loaderMesh->myModel->myTextures[0];
		std::wstring albedoFile(albedoPath.length(), L' ');
		std::copy(albedoPath.begin(), albedoPath.end(), albedoFile.begin());

		ID3D11ShaderResourceView* albedoShaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, albedoFile.c_str(), nullptr, &albedoShaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load albedo texture!");
			return nullptr;
		}
		//!Albedo

		//Normal
		std::string normalPath = texturePath/*"Textures/Characters/Enemy Robot/"*/ + loaderMesh->myModel->myTextures[5];
		std::wstring normalFile(normalPath.length(), L' ');
		std::copy(normalPath.begin(), normalPath.end(), normalFile.begin());

		ID3D11ShaderResourceView* normalShaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, normalFile.c_str(), nullptr, &normalShaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load normal texture!");
			return nullptr;
		}
		//!Normal
		//!Texture loading

		Model* model = new Model();
		if( !model )
		{
			return nullptr;
		}

		Model::SModelData modelData;
		modelData.myTextures.reserve(loaderMesh->myModel->myTextures.size());// replace with: myTextures.reserve(loaderMesh.myModel->myTextures.size()
		modelData.myTextures.shrink_to_fit();
		modelData.myNumberOfVertices    = (UINT)loaderMesh->myVertexCount;
		modelData.myNumberOfIndices		= static_cast<UINT>(loaderMesh->myIndexes.size());
		modelData.myStride				= loaderMesh->myVertexBufferSize;
		modelData.myOffset				= 0;
		modelData.myVertexBuffer		= vertexBuffer;
		modelData.myIndexBuffer			= indexBuffer;
		modelData.myVertexShader		= vertexShader;
		modelData.myPixelShader			= pixelShader;
		modelData.myPrimitiveTopology	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		modelData.myInputLayout			= inputLayout;
		modelData.myTextures.emplace_back(albedoShaderResourceView);
		modelData.myTextures.emplace_back(normalShaderResourceView);
		model->Init(modelData);

		return model;
	}

	Model* ModelFactory::GetCube()
	{
		if (myCube){
			return myCube;
		}

		HRESULT hResult;

		struct SVertex
		{
			float myX, myY, myZ, myW;
			float myR, myG, myB, myA;
			float myU, myV;
		} vertices[24] =
		{
			 { -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 }
			,{  1, -1, -1,     1,       1, 1, 0, 1,       1, 0 }
			,{ -1,  1, -1,     1,       1, 0, 1, 1,       0, 1 }
			,{  1,  1, -1,     1,       0, 1, 1, 1,       1, 1 }
			,{ -1, -1,  1,     1,       1, 0, 0, 1,       0, 0 }
			,{  1, -1,  1,     1,       0, 1, 0, 1,       1, 0 }
			,{ -1,  1,  1,     1,       0, 0, 1, 1,       0, 1 }
			,{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 }

			,{ -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 }
			,{ -1,  1, -1,     1,       1, 1, 0, 1,       1, 0 }
			,{ -1, -1,  1,     1,       1, 0, 1, 1,       0, 1 }
			,{ -1,  1,  1,     1,       0, 1, 1, 1,       1, 1 }
			,{  1, -1, -1,     1,       1, 0, 0, 1,       0, 0 }
			,{  1,  1, -1,     1,       0, 1, 0, 1,       1, 0 }
			,{  1, -1,  1,     1,       0, 0, 1, 1,       0, 1 }
			,{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 }

			,{ -1, -1, -1,     1,       1, 1, 1, 1,       0, 0 }
			,{  1, -1, -1,     1,       1, 1, 0, 1,       1, 0 }
			,{ -1, -1,  1,     1,       1, 0, 1, 1,       0, 1 }
			,{  1, -1,  1,     1,       0, 1, 1, 1,       1, 1 }
			,{ -1,  1, -1,     1,       1, 0, 0, 1,       0, 0 }
			,{  1,  1, -1,     1,       0, 1, 0, 1,       1, 0 }
			,{ -1,  1,  1,     1,       0, 0, 1, 1,       0, 1 }
			,{  1,  1,  1,     1,       0, 0, 0, 1,       1, 1 }
		};
		
		unsigned int indices[36] =
		{
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
		//Vertex
		D3D11_BUFFER_DESC vertexBufferDescription = {0};
		vertexBufferDescription.ByteWidth	=	sizeof(vertices);
		vertexBufferDescription.Usage		=	D3D11_USAGE_IMMUTABLE;
		vertexBufferDescription.BindFlags	=	D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubresourceData = {0};
		vertexSubresourceData.pSysMem = vertices;

		ID3D11Buffer* vertexBuffer = nullptr;
		hResult = myDevice->CreateBuffer(&vertexBufferDescription, &vertexSubresourceData, &vertexBuffer);
		if(FAILED(hResult))
		{
			return nullptr;
		}
		//!Vertex
		//Index
		D3D11_BUFFER_DESC indexBufferDescription = {0};
		indexBufferDescription.ByteWidth	= sizeof(indices);
		indexBufferDescription.Usage		= D3D11_USAGE_IMMUTABLE;
		indexBufferDescription.BindFlags	= D3D11_BIND_INDEX_BUFFER;
		
		D3D11_SUBRESOURCE_DATA indexSubresourceData = {0};
		indexSubresourceData.pSysMem = indices;

		ID3D11Buffer* indexBuffer = nullptr;
		hResult = myDevice->CreateBuffer(&indexBufferDescription, &indexSubresourceData, &indexBuffer);
		if (FAILED(hResult))
		{
			return nullptr;
		}
		//!Index
		//Shaders
		//	Vertex Shader
		std::ifstream vsFile;
		vsFile.open("CubeVertexShader.cso",std::ios::binary);
		std::string vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};

		ID3D11VertexShader* vertexShader = nullptr;
		hResult = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		vsFile.close();
		//	!Vertex Shader
		//	Pixel Shader
		std::ifstream psFile;
		psFile.open("CubePixelShader.cso", std::ios::binary);
		std::string psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};

		ID3D11PixelShader* pixelShader = nullptr;
		hResult = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		psFile.close();
		//	!Pixel Shader
		//!Shaders

		//Input Layout
		D3D11_INPUT_ELEMENT_DESC layout[] = 
		{
			 {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			,{"UV"		, 0, DXGI_FORMAT_R32G32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			//,{"UV"		, 0, DXGI_FORMAT_R32G32_FLOAT,		 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		ID3D11InputLayout* inputLayout = nullptr;
		hResult = myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
		if( FAILED( hResult ) )
		{
			return nullptr;
		}
		//!Input Layout

		//Texture loading
		std::wstring filename = L"Textures/thetexture.dds";
		ID3D11ShaderResourceView* shaderResourceView = nullptr;
		hResult = DirectX::CreateDDSTextureFromFile(myDevice, filename.c_str(), nullptr, &shaderResourceView);
		if (FAILED(hResult))
		{
			assert(false && "Failed to load texture!");
			return nullptr;
		}
		//!Texture loading

		//Model* model = new Model();
		myCube = new Model();
		if( !/*model*/myCube )
		{
			return nullptr;
		}
		Model::SModelData modelData;
		modelData.myNumberOfVertices	= sizeof(vertices) / sizeof(SVertex);
		modelData.myNumberOfIndices		= sizeof(indices) / sizeof(unsigned int);
		modelData.myStride				= sizeof(SVertex);
		modelData.myOffset				= 0;
		modelData.myVertexBuffer		= vertexBuffer;
		modelData.myIndexBuffer			= indexBuffer;
		modelData.myVertexShader		= vertexShader;
		modelData.myPixelShader			= pixelShader;
		modelData.myPrimitiveTopology	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		modelData.myInputLayout			= inputLayout;
		modelData.myTexture				= shaderResourceView;
		/*model*/myCube->Init(modelData);

		//return model;
		return myCube;
	}

	ModelFactory::ModelFactory()
		: myDevice(nullptr)
		, myCube(nullptr)
	{}
	ModelFactory::~ModelFactory()
	{
		//Temp
		SAFE_DELETE(myCube)
		//!Temp
	}
}

// Load model pre 2020 09 22
//Model* ModelFactory::LoadModel( const std::string& /*aModelPath*/)
//{
//	HRESULT hResult;
//
//	struct SVertex
//	{
//		float myX, myY, myZ, myW;
//		float myR, myG, myB, myA;
//	} vertices[3] = 
//	{
//		{-0.8f, -0.8f, 0, 1,  1, 0, 0, 1}
//		,{0,	  0.8f, 0, 1,  0, 1, 0, 1}
//		,{0.8f,  -0.8f, 0, 1,  0, 0, 1, 1}
//	};
//
//	D3D11_BUFFER_DESC bufferDescription = {0};
//	bufferDescription.ByteWidth =	sizeof(vertices);
//	bufferDescription.Usage		=	D3D11_USAGE_IMMUTABLE;
//	bufferDescription.BindFlags =	D3D11_BIND_VERTEX_BUFFER;
//
//	D3D11_SUBRESOURCE_DATA subresourceData = {0};
//	subresourceData.pSysMem = vertices;
//
//	ID3D11Buffer* vertexBuffer = nullptr;
//	hResult = myDevice->CreateBuffer(&bufferDescription, &subresourceData, &vertexBuffer);
//	if( FAILED( hResult ) )
//	{
//		return nullptr;
//	}
//
//	//Shaders
//	//	Vertex Shader
//	std::ifstream vsFile;
//	vsFile.open("VertexShader.cso",std::ios::binary);
//	std::string vsData = {std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>()};
//
//	ID3D11VertexShader* vertexShader = nullptr;
//	hResult = myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
//	if( FAILED( hResult ) )
//	{
//		return nullptr;
//	}
//	vsFile.close();
//	//	!Vertex Shader
//
//	//	Pixel Shader
//	std::ifstream psFile;
//	psFile.open("PixelShader.cso", std::ios::binary);
//	std::string psData = {std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>()};
//
//	ID3D11PixelShader* pixelShader = nullptr;
//	hResult = myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
//	if( FAILED( hResult ) )
//	{
//		return nullptr;
//	}
//	psFile.close();
//	//	!Pixel Shader
//	//!Shaders
//
//	//Input Layout
//	D3D11_INPUT_ELEMENT_DESC layout[] = 
//	{
//		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
//		,{"COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
//	};
//	ID3D11InputLayout* inputLayout = nullptr;
//	hResult = myDevice->CreateInputLayout(layout, 2, vsData.data(), vsData.size(), &inputLayout);
//	if( FAILED( hResult ) )
//	{
//		return nullptr;
//	}
//	//!Input Layout
//
//	Model* model = new Model();
//	if( !model )
//	{
//		return nullptr;
//	}
//	Model::SModelData modelData;
//	modelData.myNumberOfVertices	= sizeof(vertices) / sizeof(SVertex);
//	modelData.myStride				= sizeof(SVertex);
//	modelData.myOffset				= 0;
//	modelData.myVertexBuffer		= vertexBuffer;
//	modelData.myVertexShader		= vertexShader;
//	modelData.myPixelShader			= pixelShader;
//	modelData.myPrimitiveTopology	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	modelData.myInputLayout			= inputLayout;
//	model->Init(modelData);
//
//	return model;
//}