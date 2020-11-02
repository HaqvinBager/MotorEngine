#pragma once

class CSprite;

class CSpriteInstance
{
public:
	CSpriteInstance();
	~CSpriteInstance();

	bool Init(CSprite* aSprite);
	void SetPosition(DirectX::SimpleMath::Vector2 aPosition);
	void SetColor(DirectX::SimpleMath::Vector4 aColor);
	void SetUVRect(DirectX::SimpleMath::Vector4 aUVRect);
	void SetSize(DirectX::SimpleMath::Vector2 aSize);
	void SetShouldRender(bool aBool);

public:
	const DirectX::SimpleMath::Vector4 GetPosition() const { return myPosition; }
	const DirectX::SimpleMath::Vector4 GetColor() const { return myColor; }
	const DirectX::SimpleMath::Vector4 GetUVRect() const { return myUVRect; }
	const DirectX::SimpleMath::Vector2 GetSize() const { return mySize; }
	bool GetShouldRender() const { return myShouldRender; }
	CSprite* GetSprite() const { return mySprite; }

private:
	DirectX::SimpleMath::Vector4 myPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::SimpleMath::Vector4 myColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Vector4 myUVRect = { 0.0f, 0.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Vector2 mySize = { 1.0f, 1.0f };
	CSprite* mySprite;
	bool myShouldRender = true;
};

