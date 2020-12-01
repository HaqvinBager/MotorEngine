#pragma once
#include "SimpleMath.h"

class CParticle
{
public:
	struct SParticleVertex
	{
		DirectX::SimpleMath::Vector4 myPosition;
		DirectX::SimpleMath::Vector4 myMovement;
		DirectX::SimpleMath::Vector4 myColor;
		DirectX::SimpleMath::Vector2 mySize;
		float myLifeTime;
		float mySquaredDistanceToCamera;
		DirectX::SimpleMath::Vector4 myStartMovement;
		DirectX::SimpleMath::Vector4 myEndMovement;
	};

	struct SParticleData
	{
		UINT myNumberOfParticles = 0;
		UINT myStride = 0;
		UINT myOffset = 0;
		ID3D11Buffer* myParticleVertexBuffer = nullptr;
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11GeometryShader* myGeometryShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11SamplerState* mySampler = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
		ID3D11InputLayout* myInputLayout = nullptr;
		ID3D11ShaderResourceView* myTexture = nullptr;

		std::string myTexturePath = "";
		float myDelay = 0.0f;
		float myDuration = 0.0f;
		float mySpawnRate = 0.0f;
		float mySpawnAngle = 0.0f;
		float myParticleLifetime = 0.0f;
		float myParticleSpeed = 0.0f;
		DirectX::SimpleMath::Vector4 myParticleStartColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::SimpleMath::Vector4 myParticleEndColor = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::SimpleMath::Vector4 myOffsetPosition = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::SimpleMath::Vector4 myParticleStartDirection = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::SimpleMath::Vector4 myParticleEndDirection = DirectX::SimpleMath::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		float myParticleStartSize = 0.0f;
		float myParticleEndSize = 0.0f;

		float myLifetimeLowerBound = 0.0f;
		float myLifetimeUpperBound = 0.0f;
		float mySpeedLowerBound = 0.0f;
		float mySpeedUpperBound = 0.0f;
		DirectX::SimpleMath::Vector3 myDirectionLowerBound = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
		DirectX::SimpleMath::Vector3 myDirectionUpperBound = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	};

public:
	CParticle();
	~CParticle();

	void Init(SParticleData someParticleData);

public:
	SParticleData GetParticleData() { return myParticleData; }

private:
	SParticleData myParticleData;
};

