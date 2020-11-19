#include "stdafx.h"
#include "ParticleFactory.h"
#include "DirectXFramework.h"
#include "DDSTextureLoader.h"
#include "Particle.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

CParticleFactory* CParticleFactory::ourInstance = nullptr;

CParticleFactory::CParticleFactory()
{
    ourInstance = this;
    myDevice = nullptr;
}

CParticleFactory::~CParticleFactory()
{
    ourInstance = nullptr;
    myDevice = nullptr;
}

bool CParticleFactory::Init(CDirectXFramework* aFramework)
{
    myDevice = aFramework->GetDevice();
    if (!myDevice) {
        return false;
    }
    return true;
}

CParticle* CParticleFactory::GetParticle(std::string aFilePath)
{
    
    if (myParticles.find(aFilePath) == myParticles.end()) {
        return LoadParticle(aFilePath);
    }
    return myParticles.at(aFilePath);
}

CParticle* CParticleFactory::LoadParticle(std::string aFilePath)
{
    CParticle::SParticleData particleData;

    ReadJsonValues(aFilePath, particleData);

    //Start Vertex Buffer
    D3D11_BUFFER_DESC bufferDesc = { 0 };
    bufferDesc.ByteWidth = sizeof(CParticle::SParticleVertex) * /*UINT(ceil(particleLifeTime * particleSpawnRate))*//*particleData.myNumberOfParticles*/particleData.myNumberOfParticles;
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    ID3D11Buffer* vertexBuffer;
    ENGINE_HR_MESSAGE(myDevice->CreateBuffer(&bufferDesc, nullptr, &vertexBuffer), "Vertex Buffer could not be created");
    //End Vertex Buffer

    //Start Shader
    std::ifstream vsFile;
    vsFile.open("Shaders/ParticleVertexShader.cso", std::ios::binary);
    std::string vsData = { std::istreambuf_iterator<char>(vsFile), std::istreambuf_iterator<char>() };
    ID3D11VertexShader* vertexShader;
    ENGINE_HR_MESSAGE(myDevice->CreateVertexShader(vsData.data(), vsData.size(), nullptr, &vertexShader), "Vertex Shader could not be created.");
    vsFile.close();

    std::ifstream psFile;
    psFile.open("Shaders/ParticlePixelShader.cso", std::ios::binary);
    std::string psData = { std::istreambuf_iterator<char>(psFile), std::istreambuf_iterator<char>() };
    ID3D11PixelShader* pixelShader;
    ENGINE_HR_MESSAGE(myDevice->CreatePixelShader(psData.data(), psData.size(), nullptr, &pixelShader), "Pixel Shader could not be created.");
    psFile.close();

    std::ifstream gsFile;
    gsFile.open("Shaders/ParticleGeometryShader.cso", std::ios::binary);
    std::string gsData = { std::istreambuf_iterator<char>(gsFile), std::istreambuf_iterator<char>() };
    ID3D11GeometryShader* geometryShader;
    ENGINE_HR_MESSAGE(myDevice->CreateGeometryShader(gsData.data(), gsData.size(), nullptr, &geometryShader), "Geometry Shader could not be created.");
    gsFile.close();
    //End Shader

    //Start Sampler
    ID3D11SamplerState* sampler;
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    ENGINE_HR_MESSAGE(myDevice->CreateSamplerState(&samplerDesc, &sampler), "Sampler could not be created.");
    //End Sampler

    //Start Layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"MOVEMENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"LIFETIME", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"DISTANCE", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"STARTDIRECTION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"ENDDIRECTION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    ID3D11InputLayout* inputLayout;
    ENGINE_HR_MESSAGE(myDevice->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), vsData.data(), vsData.size(), &inputLayout), "Input Layout could not be created.");
    //End Layout

    ID3D11ShaderResourceView* shaderResourceView = GetShaderResourceView(myDevice, particleData.myTexturePath);

    CParticle* particleEmitter = new CParticle();
    if (!particleEmitter) {
        return nullptr;
    }

    particleData.myStride =                 sizeof(CParticle::SParticleVertex);
    particleData.myOffset =                 0;
    particleData.myParticleVertexBuffer =   vertexBuffer;
    particleData.myVertexShader =           vertexShader;
    particleData.myGeometryShader =         geometryShader;
    particleData.myPixelShader =            pixelShader;
    particleData.mySampler =                sampler;
    particleData.myPrimitiveTopology =      D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
    particleData.myInputLayout =            inputLayout;
    particleData.myTexture =                shaderResourceView;

    particleEmitter->Init(particleData);

    myParticles.emplace(aFilePath, particleEmitter);
    return particleEmitter;
}

ID3D11ShaderResourceView* CParticleFactory::GetShaderResourceView(ID3D11Device* aDevice, std::string aTexturePath) {
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
    //================================

    delete[] widePath;
    return shaderResourceView;
}

CParticleFactory* CParticleFactory::GetInstance()
{
    return ourInstance;
}

void CParticleFactory::ReadJsonValues(std::string aFilePath, CParticle::SParticleData& someParticleData)
{
    using namespace rapidjson;

    std::ifstream inputStream(aFilePath);
    IStreamWrapper inputWrapper(inputStream);
    Document document;
    document.ParseStream(inputWrapper);

    someParticleData.myNumberOfParticles =          document["Max Number Of Particles"].GetInt();
   
    someParticleData.mySpawnRate =                  document["Spawn Rate"].GetFloat();
    someParticleData.myParticleLifetime =           document["Lifetime"].GetFloat();
    someParticleData.myParticleSpeed =              document["Speed"].GetFloat(); 
    someParticleData.myParticleStartColor =       { document["Start Color R"].GetFloat(), document["Start Color G"].GetFloat(), document["Start Color B"].GetFloat(), document["Start Color A"].GetFloat() };
    someParticleData.myParticleEndColor =         { document["End Color R"].GetFloat(), document["End Color G"].GetFloat(), document["End Color B"].GetFloat(), document["End Color A"].GetFloat() };
    someParticleData.myParticleStartSize =          document["Start Size"].GetFloat();
    someParticleData.myParticleEndSize =            document["End Size"].GetFloat();
    someParticleData.myParticleStartDirection =   { document["Start Direction X"].GetFloat(), document["Start Direction Y"].GetFloat(), document["Start Direction Z"].GetFloat(), document["Start Direction W"].GetFloat() };
    someParticleData.myParticleEndDirection =     { document["End Direction X"].GetFloat(), document["End Direction Y"].GetFloat(), document["End Direction Z"].GetFloat(), document["End Direction W"].GetFloat() };
    
    someParticleData.myLifetimeLowerBound =         document["Lifetime Lower Bound"].GetFloat();
    someParticleData.myLifetimeUpperBound =         document["Lifetime Upper Bound"].GetFloat();
    someParticleData.mySpeedLowerBound =            document["Speed Lower Bound"].GetFloat();
    someParticleData.mySpeedUpperBound =            document["Speed Upper Bound"].GetFloat();
    someParticleData.myDirectionLowerBound =      { document["Direction Lower Bound X"].GetFloat(), document["Direction Lower Bound Y"].GetFloat(), document["Direction Lower Bound Z"].GetFloat() };
    someParticleData.myDirectionUpperBound =      { document["Direction Upper Bound X"].GetFloat(), document["Direction Upper Bound Y"].GetFloat(), document["Direction Upper Bound Z"].GetFloat() };
    someParticleData.myTexturePath =                document["Texture Path"].GetString();
}