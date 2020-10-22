TextureCube environmentTexture      : register(t0);
Texture2D albedoTexture             : register(t1);
Texture2D normalTexture             : register(t2);
Texture2D metalTexture              : register(t3);
Texture2D roughTexture              : register(t4);
Texture2D aoTexture                 : register(t5);
Texture2D emissiveTexture           : register(t6);
SamplerState defaultSampler         : register(s0);
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
//Texture	        N/A

cbuffer FrameBuffer : register(b0)
{
    float4x4 toCamera;
    float4x4 toProjection;

    float4 directionalLightColor;
    float4 toDirectionalLight;
    
    float4 cameraPosition;
}
cbuffer ObjectBuffer : register(b1)
{
    float4x4 toWorld;
    struct PointLight
    {
        float4 myPosition;
        float4 myColor;
        float  myRange;
        // float myPadding[3];
    }myPointLights[8];
    unsigned int myNumberOfUsedPointLights;
    unsigned int myPointLightPadding[3];
}

struct VertexInput
{
    float4 myPosition   :   POSITION;
    float3 myNormal     :   NORMAL;
    float3 myTangent    :   TANGENT;
    float3 myBinormal   :   BINORMAL;
    float2 myUV         :   UV;
    //float4 myColor      :   COLOR;
};
struct VertexToPixel
{
    float4 myPosition       :   SV_POSITION;
    float4 myNormal         :   NORMAL;
    float4 myTangent        :   TANGENT;
    float4 myBinormal       :   BINORMAL;
    float2 myUV             :   UV;
    float4 myWorldPosition  :   POSITION;
    //float4 myColor      :   COLOR;
};
struct PixelOutput
{
    float4 myColor      :   SV_TARGET;
};