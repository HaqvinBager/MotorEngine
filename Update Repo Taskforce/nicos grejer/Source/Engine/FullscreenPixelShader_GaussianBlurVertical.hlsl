#include "FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    
    float texelSize = 1.0f / resolution.y;
    float3 blurColor = float3(0.0f, 0.0f, 0.0f);
    
    unsigned int kernelSize = 5;
    float start = (((float) (kernelSize) - 1.0f) / 2.0f) * -1.0f;
    for (unsigned int index = 0; index < kernelSize; index++)
    {
        float2 uv = input.myUV.xy + float2(0.0f, texelSize * (start + (float) index));
        float3 resource = fullscreenTexture1.Sample(defaultSampler, uv).rgb;
        blurColor += resource * gaussianKernel5[index];
    }
    
    returnValue.myColor.rgb = blurColor;
    returnValue.myColor.a = 1.0f;
    return returnValue;
}