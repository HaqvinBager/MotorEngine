#include "ShaderStructs.hlsli"

PixelOutPut main(VertexToPixel input)
{
    PixelOutPut output;
    output.myColor = float4(0.45f, 0.45f, 1.0f, 1.0f);
    return output;
}