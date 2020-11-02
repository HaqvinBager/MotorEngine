#pragma once
#include "SimpleMath.h"

struct ID3D11DeviceContext;

namespace DirectX {
	class SpriteBatch;
}

class CTextInstance;

class CTextRenderer
{
public:
	CTextRenderer();
	~CTextRenderer();

	bool Init(CDirectXFramework* aFramework);

	void Render(std::vector<CTextInstance*>& aTextInstanceList);

private:
	ID3D11DeviceContext* myContext;
	DirectX::SpriteBatch* mySpriteBatch;
};

