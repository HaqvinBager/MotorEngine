#include "PBRAmbience.hlsli"
#include "PBRDirectionalLight.hlsli"

PixelOutPut main(VertexToPixel input)
{
    PixelOutPut output;
   
    float4 normal = PixelShader_TextureNormal(input).myColor.rgba;
    output.myColor = normal;
    
    return output;
}