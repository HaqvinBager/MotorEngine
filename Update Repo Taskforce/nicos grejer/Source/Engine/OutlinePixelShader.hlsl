#include "ShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    output.myColor = float4(1.0f, 0.2f, 0.2f, 1.0f);
    return output;
}