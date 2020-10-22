#include "stdafx.h"
#include "ModelFactory.h"
#include "Model.h"
#include "Engine.h"
#include "DDSTextureLoader.h"
#include "FBXLoaderCustom.h"
#include "DL_Debug.h"
#include <chrono>

#ifdef _DEBUG
#pragma comment (lib, "ModelLoader_Debug.lib")
#endif
#ifdef NDEBUG
#pragma comment (lib, "ModelLoader_Release.lib")
#endif

CModelFactory* CModelFactory::ourInstance = nullptr;

CModelFactory::CModelFactory()
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

CModel* CModelFactory::LoadModel(std::string aFilePath)
{
    HRESULT result;
    
    const size_t lastSlashIndex = aFilePath.find_last_of("\\/");
    std::string modelDirectory = aFilePath.substr(0, lastSlashIndex + 1);
    std::string modelName = aFilePath.substr(lastSlashIndex + 1, aFilePath.size() - lastSlashIndex - 5);

    CFBXLoaderCustom modelLoader;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    CLoaderModel* loaderModel = modelLoader.LoadModel(aFilePath.c_str());
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    int duration = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

    DL_Debug::CDebug::GetInstance()->WriteLog("Model", (modelName + " took %i ms to load").c_str(), duration);
   
    CLoaderMesh* mesh = loaderModel->myMeshes[0];

    //Start Vertex Buffer
    D3D11_BUFFER_DESC bufferDesc = { 0 };
    bufferDesc.ByteWidth = mesh->myVertexBufferSize * mesh->myVertexCount;
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
    subresourceData.pSysMem = mesh->myVerticies;

    ID3D11Buffer* vertexBuffer;
    result = myEngine->myFramework->GetDevice()->CreateBuffer(&bufferDesc, &subresourceData, &vertexBuffer);
    if (FAILED(result))
    {
        return nullptr;
    }
    //End Vertex Buffer

    //Start Index Buffer
    D3D11_BUFFER_DESC indexBufferDesc = { 0 };
    indexBufferDesc.ByteWidth = sizeof(unsigned int) * static_cast<UINT>(mesh->myIndexes.size());
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexSubresourceData = { 0 };
    indexSubresourceData.pSysMem = mesh->myIndexes.data();

    ID3D11Buffer* indexBuffer;
    result = myEngine->myFramework->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
    if (FAILED(result))
    {
        return nullptr;
    }
    //End Vertex Buffer

    //Start Shader
    std::ifstream vsFile;
    vsFile.open("VertexShader.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
    ID3D11VertexShader* vertexShader;
    result = myEngine->myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader);
    if (FAILED(result))
    {
        return nullptr;
    }
    vsFile.close();

    std::ifstream psFile;
    psFile.open("PixelShader.cso", std::ios::binary);
    std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
    ID3D11PixelShader* pixelShader;
    result = myEngine->myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader);
    if (FAILED(result))
    {
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
    result = myEngine->myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler);
    if (FAILED(result))
    {
        return nullptr;
    }
    //End Sampler

    //Start Layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BINORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BONEID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    ID3D11InputLayout* inputLayout;
    result = myEngine->myFramework->GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout);
    if (FAILED(result))
    {
        return nullptr;
    }
    //End Layout

    //Start Textures
    //Diffuse / Albedo
    //std::string albedoPath = modelDirectory + loaderModel->myTextures[0];
    std::string albedoPath = modelDirectory + modelName + "_D.dds";
    wchar_t* wide_string = new wchar_t[albedoPath.length() + 1];
    std::copy(albedoPath.begin(), albedoPath.end(), wide_string);
    wide_string[albedoPath.length()] = 0;

    ID3D11ShaderResourceView* albedoShaderResourceView;
    result = DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), wide_string, nullptr, &albedoShaderResourceView);
    if (FAILED(result)) {
        return nullptr;
    }
    delete[] wide_string;

    //Material
    std::string materialPath = modelDirectory + modelName + "_M.dds";
    wide_string = new wchar_t[materialPath.length() + 1];
    std::copy(materialPath.begin(), materialPath.end(), wide_string);
    wide_string[materialPath.length()] = 0;

    ID3D11ShaderResourceView* materialShaderResourceView;
    result = DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), wide_string, nullptr, &materialShaderResourceView);
    if (FAILED(result)) {
        return nullptr;
    }
    delete[] wide_string;

    //Normal
    //std::string normalPath = modelDirectory + loaderModel->myTextures[5];
    std::string normalPath = modelDirectory + modelName + "_N.dds";
    wide_string = new wchar_t[normalPath.length() + 1];
    std::copy(normalPath.begin(), normalPath.end(), wide_string);
    wide_string[normalPath.length()] = 0;

    ID3D11ShaderResourceView* normalShaderResourceView;
    result = DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), wide_string, nullptr, &normalShaderResourceView);
    if (FAILED(result)) {
        return nullptr;
    }
    delete[] wide_string;
    //End Textures

    CModel* model = new CModel();
    if (!model)
    {
        return nullptr;
    }

    CModel::SModelData modelData;
    modelData.myNumberOfVertices = mesh->myVertexCount;
    modelData.myNumberOfIndices = static_cast<UINT>(mesh->myIndexes.size());
    modelData.myStride = mesh->myVertexBufferSize;
    modelData.myOffset = 0;
    modelData.myVertexBuffer = vertexBuffer;
    modelData.myIndexBuffer = indexBuffer;
    modelData.myVertexShader = vertexShader;
    modelData.myPixelShader = pixelShader;
    modelData.mySampler = sampler;
    modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    modelData.myInputLayout = inputLayout;
    modelData.myTexture[0] = albedoShaderResourceView;
    modelData.myTexture[1] = materialShaderResourceView;
    modelData.myTexture[2] = normalShaderResourceView;
    model->Init(modelData);

    myModelMap.emplace(aFilePath, model);

    return model;
}

CModel* CModelFactory::GetModel(std::string aFilePath)
{
    if (myModelMap.find(aFilePath) == myModelMap.end())
    {
        return LoadModel(aFilePath);
    }
    return myModelMap.at(aFilePath);
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
    modelData.mySampler = nullptr;
    modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    modelData.myInputLayout = nullptr;
    modelData.myTexture[0] = nullptr;
    modelData.myTexture[1] = nullptr;
    modelData.myTexture[2] = nullptr;
    myOutlineModelSubset->Init(modelData);

    return myOutlineModelSubset;
}

CModel* CModelFactory::GetCube()
{
    constexpr float tint_r = 1.0f;
    constexpr float tint_g = 1.0f;
    constexpr float tint_b = 1.0f;
    constexpr float tint_a = 1.0f;

    //Start vertex
    struct Vertex
    {
        float x, y, z, w;
        float r, g, b, a;
        float u, v;
    } vertices[24] =
    {
        { -1, -1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 0 },
        {  1, -1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 0 },
        { -1,  1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 1 },
        {  1,  1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 1 },
        { -1, -1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 0 },
        {  1, -1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 0 },
        { -1,  1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 1 },
        {  1,  1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 1 },
                                                       
        { -1, -1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 0 },
        { -1,  1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 0 },
        { -1, -1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 1 },
        { -1,  1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 1 },
        {  1, -1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 0 },
        {  1,  1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 0 },
        {  1, -1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 1 },
        {  1,  1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 1 },
                                                        
        { -1, -1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 0 },
        {  1, -1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 0 },
        { -1, -1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 1 },
        {  1, -1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 1 },
        { -1,  1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 0 },
        {  1,  1, -1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 0 },
        { -1,  1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     0, 1 },
        {  1,  1,  1,     1,     tint_r, tint_g, tint_b, tint_a,     1, 1 }
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

    D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
    vertexBufferDesc.ByteWidth = sizeof(vertices);
    vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA vertexSubresourceData = { 0 };
    vertexSubresourceData.pSysMem = vertices;

    ID3D11Buffer* vertexBuffer;
    ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer), "Vertex Buffer could not be created.");

    D3D11_BUFFER_DESC indexBufferDesc = { 0 };
    indexBufferDesc.ByteWidth = sizeof(indices);
    indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexSubresourceData = { 0 };
    indexSubresourceData.pSysMem = indices;

    ID3D11Buffer* indexBuffer;
    ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer), "Index Buffer could not be created.");
    //End Vertex

    //Start Shader
    std::ifstream vsFile;
    vsFile.open("CubeVertexShader.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
    ID3D11VertexShader* vertexShader;
    ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader), "Vertex Shader could not be created.");
    vsFile.close();

    std::ifstream psFile;
    psFile.open("CubePixelShader.cso", std::ios::binary);
    std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
    ID3D11PixelShader* pixelShader;
    ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader), "Pixel Shader could not be created.");
    psFile.close();
    //End Shader

    //Start Sampler
    ID3D11SamplerState* sampler;
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateSamplerState(&samplerDesc, &sampler), "Sampler could not be created.");
    //End Sampler

    //Start Layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    ID3D11InputLayout* inputLayout;
    ENGINE_HR_MESSAGE(myEngine->myFramework->GetDevice()->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout), "Input Layout could not be created.");
    //End Layout

    //Start Textures
    std::wstring filename = L"VFXCloud.dds";
    ID3D11ShaderResourceView* textureOneResourceView;
    ENGINE_HR_MESSAGE(DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), filename.c_str(), nullptr, &textureOneResourceView), "Texture could not be loaded.");

    ID3D11ShaderResourceView* textureTwoResourceView;
    ENGINE_HR_MESSAGE(DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), filename.c_str(), nullptr, &textureTwoResourceView), "Texture could not be loaded.");

    filename = L"VFXCloud.dds";
    ID3D11ShaderResourceView* textureThreeResourceView;
    ENGINE_HR_MESSAGE(DirectX::CreateDDSTextureFromFile(myEngine->myFramework->GetDevice(), filename.c_str(), nullptr, &textureThreeResourceView), "Texture could not be created.");
    //End Textures

    CModel* model = new CModel();
    if (!model)
    {
        return nullptr;
    }

    CModel::SModelData modelData;
    modelData.myNumberOfVertices = sizeof(vertices) / sizeof(Vertex);
    modelData.myNumberOfIndices = sizeof(indices) / sizeof(unsigned int);
    modelData.myStride = sizeof(Vertex);
    modelData.myOffset = 0;
    modelData.myVertexBuffer = vertexBuffer;
    modelData.myIndexBuffer = indexBuffer;
    modelData.myVertexShader = vertexShader;
    modelData.myPixelShader = pixelShader;
    modelData.mySampler = sampler;
    modelData.myPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    modelData.myInputLayout = inputLayout;
    modelData.myTexture[0] = textureOneResourceView;
    modelData.myTexture[1] = textureTwoResourceView;
    modelData.myTexture[2] = textureThreeResourceView;
    model->Init(modelData);

    // Var lagras kuben? (map)

    return model;
}

CModelFactory* CModelFactory::GetInstance()
{
    return ourInstance;
}
