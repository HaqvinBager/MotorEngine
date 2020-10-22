struct VertexInput
{
    float4 myPosition	: POSITION;
    float4 myNormal		: NORMAL;
    float4 myTangent	: TANGENT;
    float4 myBinormal	: BINORMAL;
    float2 myUV			: UV;
};

struct VertexToPixel
{
	float4 myPosition	: SV_POSITION;
	float2 myUV			: UV;
};

struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

cbuffer FrameBuffer : register(b0)
{
	float4x4 toCamera;
	float4x4 toProjection;
}

cbuffer ObjectBuffer : register(b1)
{
	float4x4 toWorld;
}

cbuffer TextureScrollBuffer : register(b2)
{
    float2 scrollSpeed1;
    float2 scrollSpeed2;
    float2 scrollSpeed3;
    float2 scrollSpeed4;
    
    float uvScale1;
    float uvScale2;
    float uvScale3;
    float uvScale4;
    float scrollTimer;
    float opacityStrength;
    float2 padding;
}

Texture2D instanceTexture1 : register(t0);
Texture2D instanceTexture2 : register(t1);
Texture2D instanceTexture3 : register(t2);
Texture2D instanceTexture4 : register(t3);
SamplerState defaultSampler : register(s0);