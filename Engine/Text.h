#pragma once

namespace DirectX {
	class SpriteFont;
}

class CText
{
public:
	struct STextData {
		DirectX::SpriteFont* mySpriteFont = nullptr;
	};

	CText();
	~CText();

	bool Init(STextData someData);

	STextData& GetTextData();
private:
	STextData myTextData;
};

