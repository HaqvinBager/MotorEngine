#include "ShaderStructs.hlsli"

PixelOutPut main(VertexToPixel input)
{
    PixelOutPut output;
    output.myColor = myOutlineColor;
    return output;
}