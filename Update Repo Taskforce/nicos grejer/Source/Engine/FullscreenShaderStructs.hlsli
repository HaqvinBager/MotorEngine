struct VertexInput
{
    unsigned int myIndex : SV_VertexID;
};

struct VertexToPixel
{
    float4 myPosition   : SV_POSITION;
    float2 myUV         : UV;
};

struct PixelOutput
{
    float4 myColor      : SV_TARGET;
};

Texture2D fullscreenTexture1 : register(t0);
Texture2D fullscreenTexture2 : register(t1);
SamplerState defaultSampler : register(s0);

static const float2 resolution = float2(1600.0f, 900.0f) / 8.0f; //Send via cbuffer, 8.0f is downsampling constant
static const float gaussianKernel5[5] = { 0.06136f, 0.24477f, 0.38774f, 0.24477f, 0.06136f };