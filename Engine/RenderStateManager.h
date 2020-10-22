#pragma once
#include <array>

class CRenderStateManager
{
public:
	enum class BlendStates
	{
		BLENDSTATE_DISABLE,
		BLENDSTATE_ALPHABLEND,
		BLENDSTATE_ADDITIVEBLEND,
		BLENDSTATE_COUNT
	};

	enum class DepthStencilStates
	{
		DEPTHSTENCILSTATE_DEFAULT,
		DEPTHSTENCILSTATE_ONLYREAD,
		DEPTHSTENCILSTATE_STENCILWRITE,
		DEPTHSTENCILSTATE_STENCILMASK,
		DEPTHSTENCILSTATE_COUNT
	};

	enum class RasterizerStates
	{
		RASTERIZERSTATE_DEFAULT,
		RASTERIZERSTATE_WIREFRAME,
		RASTERIZERSTATE_COUNT
	};

	enum class SamplerStates
	{
		SAMPLERSTATE_TRILINEAR,
		SAMPLERSTATE_POINT,
		SAMPLERSTATE_WRAP,
		SAMPLERSTATE_COUNT
	};

public:
	CRenderStateManager();
	~CRenderStateManager();

	bool Init(CDirectXFramework* aFramework);

	void SetBlendState(BlendStates aBlendstate);
	void SetDepthStencilState(DepthStencilStates aDepthStencilState, UINT aStencilRef = 0);
	void SetRasterizerState(RasterizerStates aRasterizerState);
	void SetSamplerState(SamplerStates aSamplerState);
	void SetAllStates(BlendStates aBlendState, DepthStencilStates aDepthStencilState, RasterizerStates aRasterizerState, SamplerStates aSamplerState);
	void SetAllDefault();

private:
	bool CreateBlendStates(ID3D11Device* aDevice);
	bool CreateDepthStencilStates(ID3D11Device* aDevice);
	bool CreateRasterizerStates(ID3D11Device* aDevice);
	bool CreateSamplerStates(ID3D11Device* aDevice);

private:
	ID3D11DeviceContext* myContext;
	std::array<ID3D11BlendState*, (size_t)BlendStates::BLENDSTATE_COUNT> myBlendStates;
	std::array<ID3D11DepthStencilState*, (size_t)DepthStencilStates::DEPTHSTENCILSTATE_COUNT> myDepthStencilStates;
	std::array<ID3D11RasterizerState*, (size_t)RasterizerStates::RASTERIZERSTATE_COUNT> myRasterizerStates;
	std::array<ID3D11SamplerState*, (size_t)SamplerStates::SAMPLERSTATE_COUNT> mySamplerStates;
};

