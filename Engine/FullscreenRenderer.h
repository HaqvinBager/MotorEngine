#pragma once
#include <array>

struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
class CDirectXFramework;
class CRenderManager;

class CFullscreenRenderer {
public:
	enum class FullscreenShader {
		FULLSCREENSHADER_COPY,
		FULLSCREENSHADER_LUMINANCE,
		FULLSCREENSHADER_GAUSSIANHORIZONTAL,
		FULLSCREENSHADER_GAUSSIANVERTICAL,
		FULLSCREENSHADER_BLOOM,
		FULLSCREENSHADER_VIGNETTE,
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
	ID3D11SamplerState* mySampler;
	std::array<ID3D11PixelShader*, static_cast<size_t>(FullscreenShader::FULLSCREENSHADER_COUNT)> myPixelShaders;
};