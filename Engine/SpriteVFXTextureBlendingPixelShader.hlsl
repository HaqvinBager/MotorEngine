#include "SpriteVFXShaderStructs.hlsli"

PixelOutput main(GeometryToPixel input)
{
    float diff = (1.0f - level);
    if (verticalDirectionOfChange && (input.myUV.y < diff) || (!verticalDirectionOfChange && input.myUV.x > level))
    {
        discard;
    }

    PixelOutput returnValue;
    
    float2 uv;
    float2 centeredUV = input.myUV.xy * 2.0 - 1.0;
    float z = sqrt(1.0 - saturate(dot(centeredUV.xy, centeredUV.xy)));
    float2 spherifiedUV = centeredUV / (z + 1.0);
    uv = spherifiedUV * 0.5 + 0.5;
    //uv = input.myUV.xy;
    
    
    float4 textureColor = instanceTexture1.Sample(defaultSampler, (uv * uvScale1) + scrollSpeed1 * scrollTimer).rgba;
    float4 textureTwoColor = instanceTexture2.Sample(defaultSampler, (uv * uvScale2) + scrollSpeed2 * scrollTimer).rgba;
    float4 textureThreeColor = instanceTexture3.Sample(defaultSampler, (uv * uvScale3) + scrollSpeed3 * scrollTimer).rgba;
    float4 textureFourColor = instanceTexture4.Sample(defaultSampler, (uv * uvScale4) + scrollSpeed4 * scrollTimer).rgba;

    textureColor = ((textureColor * textureTwoColor * 2.0f) * textureThreeColor * 2.0f) * textureFourColor * 2.0f;
    
    ///
    float2 uvtest = uv;
    uvtest.y = uv.y + (2 * textureThreeColor.r - 1) * 0.05;
    ///
    
    //Glow
    //textureColor.r = lerp(0.7f, 1.0f, textureColor.a);
    //textureColor.g = lerp(0.0f, 1.0f, textureColor.a * 0.75f);
    //textureColor.b = lerp(0.0f, 0.5f, textureColor.a);
    //textureColor.a = 1.0f;
    
    float factor = 0.0f;
    if (verticalDirectionOfChange && (input.myUV.y < (diff + glowWidth)))
    {
        factor = ((diff + glowWidth) - input.myUV.y) / (glowWidth);
    }
    else if (!verticalDirectionOfChange && (input.myUV.x > (level - glowWidth)))
    {
        factor = ((level - glowWidth) - input.myUV.x) / (-glowWidth);
    }
    textureColor.r = lerp(textureColor.r, glowColor.r, factor);
    textureColor.g = lerp(textureColor.g, glowColor.g, factor);
    textureColor.b = lerp(textureColor.b, glowColor.b, factor);
    
    returnValue.myColor = textureColor;

    return returnValue;
}