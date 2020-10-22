#include "ParticleShaderStructs.hlsli"

VertexToGeometry main(VertexInput input)
{
    VertexToGeometry returnValue;
    float4 vertexObjectPos = input.myPosition.xyzw;
    float4 vertexWorldPos = mul(toWorld, vertexObjectPos);
    float4 vertexViewPos = mul(toCamera, vertexWorldPos);
    returnValue.myPosition = vertexViewPos;
    returnValue.myColor = input.myColor;
    returnValue.mySize = input.mySize;
    return returnValue;
}