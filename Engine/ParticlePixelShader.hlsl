#include "ParticleShaderStructs.hlsli"

PixelOutput main(GeometryToPixel input)
{
    PixelOutput output;
    float4 textureColor = particleTexture.Sample(defaultSampler, input.myUV.xy).rgba;
    output.myColor.rgba = textureColor.rgba * input.myColor.rgba;
	return output;
}