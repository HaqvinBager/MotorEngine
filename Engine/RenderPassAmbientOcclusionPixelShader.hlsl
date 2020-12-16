#include "PBRAmbience.hlsli"
#include "PBRDirectionalLight.hlsli"

PixelOutPut main(VertexToPixel input)
{
    PixelOutPut output;
   
    float4 ao = PixelShader_AmbientOcclusion(input).myColor.rgba;
    output.myColor = ao;
    
    return output;
}