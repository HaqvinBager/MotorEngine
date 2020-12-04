#pragma once
#include "SimpleMath.h"

class CText;
class CScene;
class CTextInstance
{
public:
	CTextInstance(CScene& aScene);
	~CTextInstance();

	bool Init(CText* aText);
	void SetText(std::string aString);
	void SetPosition(DirectX::SimpleMath::Vector2 aPosition);
	void SetColor(DirectX::SimpleMath::Vector4 aColor);
	void SetPivot(DirectX::SimpleMath::Vector2 aPivot);

public:
	CText* GetTextData() { return myTextData; }
	std::string GetText() { return myText; }
	DirectX::SimpleMath::Vector2 GetPosition() { return myPosition; }
	DirectX::SimpleMath::Vector4 GetColor() { return myColor; }
	DirectX::SimpleMath::Vector2 GetPivot() { return myPivot; }

private:
	CText* myTextData;
	DirectX::SimpleMath::Vector4 myColor;
	DirectX::SimpleMath::Vector2 myPosition;
	DirectX::SimpleMath::Vector2 myPivot;
	std::string myText;
};

