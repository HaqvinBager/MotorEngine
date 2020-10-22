#include "CubeShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
	PixelOutput returnValue;
    float4 textureColor = instanceTexture.Sample(defaultSampler, input.myUV.xy).rgba;
	returnValue.myColor = textureColor;
	//returnValue.myColor = input.myColor;
	return returnValue;
}

