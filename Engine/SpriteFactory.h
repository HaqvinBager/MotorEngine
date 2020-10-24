#pragma once

class CDirectXFramework;
class CSprite;

class CSpriteFactory
{
	friend class CEngine;
public:
	bool Init(CDirectXFramework* aFramework);

	CSprite* LoadSprite(std::string aTexturePath);
	CSprite* GetSprite(std::string aTexturePath);

	static CSpriteFactory* GetInstance();

private:
	ID3D11ShaderResourceView* GetShaderResourceView(ID3D11Device* aDevice, std::string aTexturePath);

private:
	CSpriteFactory();
	~CSpriteFactory();

private:
	static CSpriteFactory* ourInstance;
	std::map<std::string, CSprite*> mySpriteMap;
	CDirectXFramework* myFramework;

	ID3D11VertexShader* myVertexShader;
	ID3D11PixelShader* myPixelShader;
	ID3D11GeometryShader* myGeometryShader;
};