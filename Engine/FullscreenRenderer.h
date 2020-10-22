#pragma once
#include <array>

struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
class CDirectXFramework;
class CRenderManager;

class CFullscreenRenderer {
public:
	enum FullscreenShader {
		FULLSCREENSHADER_COPY,
		FULLSCREENSHADER_LUMINANCE,
		FULLSCREENSHADER_GAUSSIANHORIZONTAL,
		FULLSCREENSHADER_GAUSSIANVERTICAL,
		FULLSCREENSHADER_BLOOM,
		FULLSCREENSHADER_COUNT,
	};

	friend CRenderManager;
	
private:
	CFullscreenRenderer();
	~CFullscreenRenderer();
	bool Init(CDirectXFramework* aFramework);
	void Render(FullscreenShader aEffect);

	ID3D11DeviceContext* myContext;
	ID3D11VertexShader* myVertexShader;
	std::array<ID3D11PixelShader*, FullscreenShader::FULLSCREENSHADER_COUNT> myPixelShaders;
};