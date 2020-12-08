#pragma once
#include "SimpleMath.h"

class CText;
class CScene;
class CTextInstance
{
public:
	CTextInstance(CScene& aScene, bool aAddToScene = true);
	CTextInstance();
	~CTextInstance();

	bool Init(CText* aText);
	void SetText(std::string aString);
	void SetPosition(DirectX::SimpleMath::Vector2 aPosition);
	void SetColor(DirectX::SimpleMath::Vector4 aColor);
	void SetPivot(DirectX::SimpleMath::Vector2 aPivot);
	void SetScale(DirectX::SimpleMath::Vector2 aScale);

public:
	CText* GetTextData() { return myTextData; }
	std::string GetText() { return myText; }
	DirectX::SimpleMath::Vector2 GetPosition() { return myPosition; }
	DirectX::SimpleMath::Vector4 GetColor() { return myColor; }
	DirectX::SimpleMath::Vector2 GetPivot() { return myPivot; }
	DirectX::SimpleMath::Vector2 GetScale() { return myScale; }

private:
	CText* myTextData;
	DirectX::SimpleMath::Vector4 myColor;
	DirectX::SimpleMath::Vector2 myPosition;
	DirectX::SimpleMath::Vector2 myPivot;
	DirectX::SimpleMath::Vector2 myScale;
	std::string myText;
};

