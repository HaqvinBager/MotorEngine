#pragma once

class CSpriteInstance;

struct SAnimatedSpriteData
{
	ID3D11PixelShader* myPixelShader = nullptr;
	std::array<ID3D11ShaderResourceView*, 4> myTexture;

	DirectX::SimpleMath::Vector2 scrollSpeed1;
	DirectX::SimpleMath::Vector2 scrollSpeed2;
	DirectX::SimpleMath::Vector2 scrollSpeed3;
	DirectX::SimpleMath::Vector2 scrollSpeed4;

	float uvScale1;
	float uvScale2;
	float uvScale3;
	float uvScale4;

	DirectX::SimpleMath::Vector3 glowColor;
	float glowWidth;

	bool verticalDirectionOfChange;
};

class CAnimatedUIElement
{
	friend class CSpriteRenderer;
public:

public:
	CAnimatedUIElement();
	~CAnimatedUIElement();

	void Level(float aLevel);
	float Level() const;
	
	CSpriteInstance* GetInstance() const;
	SAnimatedSpriteData* GetVFXBaseData();

private:
	SAnimatedSpriteData* myData;
	CSpriteInstance* mySpriteInstance;
	float myLevel;
};

