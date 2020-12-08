#pragma once

class CSpriteInstance;
class CScene;
struct SAnimatedSpriteData
{
	ID3D11PixelShader* myPixelShader = nullptr;
	std::array<ID3D11ShaderResourceView*, 5> myTexture;

	DirectX::SimpleMath::Vector2 scrollSpeed1;
	DirectX::SimpleMath::Vector2 scrollSpeed2;
	DirectX::SimpleMath::Vector2 scrollSpeed3;
	DirectX::SimpleMath::Vector2 scrollSpeed4;
	DirectX::SimpleMath::Vector2 scrollSpeed5;

	float uvScale1;
	float uvScale2;
	float uvScale3;
	float uvScale4;
	float uvScale5;

	float maskOffset;
	float randomOffset;

	DirectX::SimpleMath::Vector3 glowColor;
	float glowWidth;

	bool verticalDirectionOfChange;
};

class CAnimatedUIElement
{
	friend class CSpriteRenderer;
public:

public:
	CAnimatedUIElement(std::string aFilePath, CScene& aScene, bool addToScene = true);
	~CAnimatedUIElement();

	void Level(float aLevel);
	float Level() const;
	
	void SetPosition(DirectX::SimpleMath::Vector2 aPosition);

public:
	CSpriteInstance* GetInstance() const;
	SAnimatedSpriteData* GetVFXBaseData();

private:
	SAnimatedSpriteData* myData;
	CSpriteInstance* mySpriteInstance;
	float myLevel;
};

