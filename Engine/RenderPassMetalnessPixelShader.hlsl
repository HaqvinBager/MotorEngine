#include "PBRAmbience.hlsli"
#include "PBRDirectionalLight.hlsli"

PixelOutPut main(VertexToPixel input)
{
    PixelOutPut output;
   
    float4 metalness = PixelShader_Metalness(input).myColor.rgba;
    output.myColor = metalness;
    
    return output;
}