struct VertexInput
{
    unsigned int myIndex : SV_VertexID;
};

struct VertexToGeometry
{
    float4 myPosition : POSITION;
    float4 myColor : COLOR;
    float4 myUVRect : UVRECT;
    float2 mySize : SIZE;
};

struct GeometryToPixel
{
    float4 myPosition : SV_POSITION;
    float4 myColor : COLOR;
    float2 myUV : UV;
};

struct PixelOutput
{
    float4 myColor : SV_TARGET;
};

cbuffer ObjectBuffer : register(b0)
{
    float4 myPosition;
    float4 myColor;
    float4 myUVRect;
    float2 mySize;
    float2 padding;
};

Texture2D spriteTexture : register(t0);
SamplerState defaultSampler : register(s0);