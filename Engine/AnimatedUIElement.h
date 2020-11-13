#pragma once

class CSpriteInstance;

class CAnimatedUIElement
{
	friend class CSpriteRenderer;
public:
	struct SAnimatedSpriteData
	{
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11GeometryShader* myGeometryShader = nullptr;
		ID3D11SamplerState* mySampler = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
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

		std::string texturePathOne = "";
		std::string texturePathTwo = "";
		std::string texturePathThree = "";
		std::string texturePathMask = "";
	};

public:
	CAnimatedUIElement();
	~CAnimatedUIElement();

	void Level(float aLevel);
	float Level() const;
	
	CSpriteInstance* GetInstance() const;

private:
	CSpriteInstance* mySpriteInstance;
	//std::vector<ID3D11ShaderResourceView*> myTextures;
	std::array<std::wstring, 4> myTexturePaths;
	ID3D11PixelShader* myPixelShader;
	float myLevel;
};

