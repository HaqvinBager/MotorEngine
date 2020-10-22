struct VertexInput
{
	float4 myPosition	 : POSITION;
	float4 myColor		 : COLOR;
    float2 myUV			 : UV;
};

struct VertexToPixel
{
	float4 myPosition	: SV_POSITION;
	float4 myColor		: COLOR;
    float2 myUV         : UV;
};

struct PixelOutput
{
	float4 myColor		: SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
    float4x4 toCamera;
    float4x4 toProjection;
    float4 cameraPosition;
    float4 toDirectionalLight;
    float4 directionalLightColor;
}

cbuffer ObjectBuffer : register(b1)
{
	float4x4 toWorld;
}

cbuffer TextureScrollBuffer : register(b2)
{
    float2 scroll;
    float2 scroll2;
}

TextureCube environmentCubemapTexture : register(t0);
Texture2D instanceTexture : register(t1);
Texture2D instanceTexture2 : register(t2);
Texture2D instanceTexture3 : register(t3);
SamplerState defaultSampler : register(s0);