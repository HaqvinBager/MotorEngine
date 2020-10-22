#include "VFXShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
    VertexToPixel returnValue;

    float4 vertexObjectPos = input.myPosition.xyzw;
    float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
    float4 vertexViewPos = mul(toCamera, vertexWorldPos);
    float4 vertexProjectionPos = mul(toProjection, vertexViewPos);

    returnValue.myPosition = vertexProjectionPos;
    returnValue.myUV = input.myUV;
    return returnValue;
}