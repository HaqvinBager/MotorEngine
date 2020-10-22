texture2D instanceTexture : register(t0);
SamplerState defaultSampler : register(s0);
//Filter			D3D11_FILTER_MIN_MAG_MIP_LINEAR
//AddresU			D3D11_TEXUTRE_ADDRESS_CLAMP
//AddresV			D3D11_TEXUTRE_ADDRESS_CLAMP
//AddresW			D3D11_TEXUTRE_ADDRESS_CLAMP
//MinLOD			-3.402823466e+38F (-FLT_MAX)
//MaxLOD			3.402823466e+38F (FLT_MAX)
//MipMapLODBias		0.0f
//MaxAnisotropy		1
//ComparisonFunc	D3D11_COMPARISON_NEVER
//BorderColor		float4(1.0f,1.0f,1.0f,1.0f)
//Texture	N/A

struct VertexInput
{
	float4 myPosition	:	POSITION;
	float4 myColor		:	COLOR;
	float2 myUV			:   UV;
};
struct VertexToPixel
{
	float4 myPosition	:	SV_POSITION;
	float4 myColor		:	COLOR;
    float2 myUV			:	UV;
};
struct PixelOutput
{
	float4 myColor		:	SV_TARGET;
};

//cbuffer FrameBuffer : register(b0)
//{
//	float4x4 toCamera;
//	float4x4 toProjection;
//}
//cbuffer ObjectBuffer : register(b1)
//{
//	float4x4 toWorld;
//}