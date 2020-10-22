#include "FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    float4 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgba;

    float2 uv = input.myUV.xy;
    uv -= 0.5f;
    uv *= 2.0f;
    float vignetteStrength = 1.0f / 10.0f; // 1/3 to 1/16
    float vignette = (uv.x * uv.x + uv.y * uv.y) * vignetteStrength;

    vignette = cos(vignette * (3.141592f * 2.0f));
    
    resource.x *= vignette;
    resource.y *= vignette;
    resource.z *= vignette;
    
    returnValue.myColor.rgb = resource.rgb;
    returnValue.myColor.a = 1.0f;
    
    return returnValue;
}