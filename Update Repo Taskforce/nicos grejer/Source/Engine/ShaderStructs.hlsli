struct VertexInput
{
    float4 myPosition   : POSITION;
    float4 myNormal     : NORMAL;
    float4 myTangent    : TANGENT;
    float4 myBinormal   : BINORMAL;
    float2 myUV			: UV;
    float4 myBoneID     : BONEID;
    float4 myBoneWeight : BONEWEIGHT;
};

struct VertexToPixel
{
    float4 myPosition       : SV_POSITION;
    float4 myWorldPosition  : WORLDPOSITION;
    float4 myNormal         : NORMAL;
    float4 myTangent        : TANGENT;
    float4 myBinormal       : BINORMAL;
    float2 myUV             : UV;
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
    struct PointLight
    {
        float4 myColor;
        float3 myPosition;
        float myRange;
    } myPointLights[8];
    unsigned int myNumberOfUsedPointLights;
    unsigned int padding[3];
}

TextureCube environmentCubemapTexture : register(t0);
Texture2D albedoTexture : register(t1);
Texture2D materialTexture : register(t2);
Texture2D normalTexture : register(t3);
SamplerState defaultSampler : register(s0);