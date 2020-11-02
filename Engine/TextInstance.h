#pragma once
#include "SimpleMath.h"

class CText;

class CTextInstance
{
public:
	CTextInstance();
	~CTextInstance();

	bool Init(CText* aText);
	void SetText(std::string aString);
	void SetPosition(DirectX::SimpleMath::Vector2 aPosition);
	void SetColor(DirectX::SimpleMath::Vector4 aColor);

public:
	CText* GetTextData() { return myTextData; }
	std::string GetText() { return myText; }
	DirectX::SimpleMath::Vector2 GetPosition() { return myPosition; }
	DirectX::SimpleMath::Vector4 GetColor() { return myColor; }

private:
	CText* myTextData;
	std::string myText;
	DirectX::SimpleMath::Vector2 myPosition;
	DirectX::SimpleMath::Vector4 myColor;
};

