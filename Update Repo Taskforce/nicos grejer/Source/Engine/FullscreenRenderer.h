#pragma once

class CDirectXFramework;

class CFullscreenRenderer
{
public:
	enum class FullscreenShader
	{
		FULLSCREENSHADER_COPY,
		FULLSCREENSHADER_LUMINANCE,
		FULLSCREENSHADER_GAUSSIANHORIZONTAL,
		FULLSCREENSHADER_GAUSSIANVERTICAL,
		FULLSCREENSHADER_BLOOM,
		FULLSCREENSHADER_VIGNETTE,
		FULLSCREENSHADER_COUNT
	};

public:
	CFullscreenRenderer();
	~CFullscreenRenderer();

	bool Init(CDirectXFramework* aFramework);

	void Render(FullscreenShader anEffect);

private:
	ID3D11DeviceContext* myContext;
	ID3D11VertexShader* myVertexShader;
	ID3D11SamplerState* mySampler;
	std::array<ID3D11PixelShader*, (size_t)FullscreenShader::FULLSCREENSHADER_COUNT> myPixelShaders;
};

