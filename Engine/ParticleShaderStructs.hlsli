struct VertexInput
{
    float4 myPosition           : POSITION;
    float4 myMovement           : MOVEMENT;
    float4 myColor              : COLOR;
    float2 mySize               : SIZE;
    float myLifetime            : LIFETIME;
    float myDistanceToCamera    : DISTANCE;
    float4 myStartDirection     : STARTDIRECTION;
    float4 myEndDirection       : ENDDIRECTION;
};

struct VertexToGeometry
{
    float4 myPosition   : POSITION;
    float4 myColor      : COLOR;
    float2 mySize       : SIZE;
};

struct GeometryToPixel
{
    float4 myPosition   : SV_POSITION;
    float4 myColor      : COLOR;
    float2 myUV         : UV;
};

struct PixelOutput
{
    float4 myColor      : SV_TARGET;
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

Texture2D particleTexture : register(t0);
SamplerState defaultSampler : register(s0);