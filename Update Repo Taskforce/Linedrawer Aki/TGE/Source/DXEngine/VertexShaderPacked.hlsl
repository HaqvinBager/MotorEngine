#include "ShaderStructsPacked.hlsli"

VertexToPixel main(VertexInput input)
{
    VertexToPixel returnValue;

    float4 vertexObjectPos      = input.myPosition.xyzw;
    float4 vertexWorldPos       = mul(toWorld, vertexObjectPos);
    float4 vertexViewPos        = mul(toCamera, vertexWorldPos);
    float4 vertexProjectionPos  = mul(toProjection, vertexViewPos);

    float3x3 toWorldRotation    = (float3x3) toWorld;
    float3 vertexWorldNormal    = mul(toWorldRotation, input.myNormal.xyz);
    float3 vertexWorldTangent   = mul(toWorldRotation, input.myTangent.xyz);
    float3 vertexWorldBinormal  = mul(toWorldRotation, input.myBinormal.xyz);
    
    returnValue.myPosition      = vertexProjectionPos;
    returnValue.myNormal        = float4(vertexWorldNormal, 0);
    returnValue.myTangent       = float4(vertexWorldTangent, 0);
    returnValue.myBinormal      = float4(vertexWorldBinormal, 0);
    returnValue.myUV            = input.myUV;
    returnValue.myWorldPosition = vertexWorldPos;
    //returnValue.myColor     = input.myColor;
    return returnValue;
}
float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}