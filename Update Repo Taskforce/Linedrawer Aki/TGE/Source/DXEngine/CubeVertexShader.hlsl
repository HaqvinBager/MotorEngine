#include "CubeShaderStructs.hlsli"

cbuffer FrameBuffer : register(b0)
{
	float4x4 toCamera;
	float4x4 toProjection;
}
cbuffer ObjectBuffer : register(b1)
{
	float4x4 toWorld;
}

VertexToPixel main(VertexInput input)
{
	VertexToPixel returnValue;

	float4 vertexObjectPos		= input.myPosition.xyzw;
	float4 vertexWorldPos		= mul(toWorld, vertexObjectPos);
	float4 vertexViewPos		= mul(toCamera, vertexWorldPos);
	float4 vertexProjectionPos	= mul(toProjection, vertexViewPos);

	returnValue.myPosition	= vertexProjectionPos;
	returnValue.myColor		= input.myColor;
    returnValue.myUV		= input.myUV;
	return returnValue;
}