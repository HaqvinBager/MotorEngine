#include "CubeShaderStructs.hlsli"

PixelOutPut main(VertexToPixel input)
{
	PixelOutPut returnValue;
	float4 textureColor = instanceTexture.Sample(defaultSampler, input.myUV.xy).rgba;
	returnValue.myColor = textureColor;
	return returnValue;
}