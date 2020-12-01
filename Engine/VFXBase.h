#pragma once
class CVFXBase {
public:
	struct SVFXBaseData {
		UINT myNumberOfVertices = 0;
		UINT myNumberOfIndices = 0;
		UINT myStride = 0;
		UINT myOffset = 0;
		ID3D11Buffer* myVertexBuffer = nullptr;
		ID3D11Buffer* myIndexBuffer = nullptr;
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11SamplerState* mySampler = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
		ID3D11InputLayout* myInputLayout = nullptr;
		std::array<ID3D11ShaderResourceView*, 4> myTexture{};

		DirectX::SimpleMath::Vector2 scrollSpeed1;
		DirectX::SimpleMath::Vector2 scrollSpeed2;
		DirectX::SimpleMath::Vector2 scrollSpeed3;
		DirectX::SimpleMath::Vector2 scrollSpeed4;

		float uvScale1;
		float uvScale2;
		float uvScale3;
		float uvScale4;
		float opacityStrength;

		float myDelay = 0.0f;
		float myDuration = 0.0f;
		bool myIsActive = false;

		std::string texturePathOne = "";
		std::string texturePathTwo = "";
		std::string texturePathThree = "";
		std::string texturePathMask = "";
		std::string meshPath = "";
	};

	CVFXBase();
	~CVFXBase();

	bool Init(SVFXBaseData someData);

	SVFXBaseData& GetVFXBaseData();
private:
	SVFXBaseData myVFXBaseData;
};

