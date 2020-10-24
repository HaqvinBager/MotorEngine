#pragma once
class CSprite
{
public:
	struct SSpriteData
	{
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11GeometryShader* myGeometryShader = nullptr;
		ID3D11SamplerState* mySampler = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
		ID3D11ShaderResourceView* myTexture;
	};

public:
	CSprite();
	~CSprite();

	bool Init(SSpriteData &someSpriteData);

public:
	SSpriteData GetSpriteData() { return mySpriteData; }

private:
	SSpriteData mySpriteData;
};

