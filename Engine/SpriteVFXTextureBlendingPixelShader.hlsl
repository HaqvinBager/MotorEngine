#include "SpriteVFXShaderStructs.hlsli"

PixelOutput main(GeometryToPixel input)
{
    float diff = (1.0f - level);
    if (verticalDirectionOfChange && (input.myUV.y < diff) || (!verticalDirectionOfChange && input.myUV.x > level))
    {
        discard;
    }

    PixelOutput returnValue;
    float4 textureColor = instanceTexture1.Sample(defaultSampler, (input.myUV.xy * uvScale1) + scrollSpeed1 * scrollTimer).rgba;
    float4 textureTwoColor = instanceTexture2.Sample(defaultSampler, (input.myUV.xy * uvScale2) + scrollSpeed2 * scrollTimer).rgba;
    float4 textureThreeColor = instanceTexture3.Sample(defaultSampler, (input.myUV.xy * uvScale3) + scrollSpeed3 * scrollTimer).rgba;
    float4 textureFourColor = instanceTexture4.Sample(defaultSampler, (input.myUV.xy * uvScale4) + scrollSpeed4 * scrollTimer).rgba;
    float4 textureFiveColor = instanceTexture5.Sample(defaultSampler, input.myUV.xy).rgba;

    textureColor = ((textureColor * textureTwoColor * 2.0f) * textureThreeColor * 2.0f) * textureFourColor * 2.0f;
    
    //Glow
    //textureColor.r = lerp(0.7f, 1.0f, textureColor.a);
    //textureColor.g = lerp(0.0f, 1.0f, textureColor.a * 0.75f);
    //textureColor.b = lerp(0.0f, 0.5f, textureColor.a);
    //textureColor.a = 1.0f;
    
    //float glowWidth = 0.1f;
    //float3 glowColor = { 1.0f, 0.45f, 0.45f };
    if (verticalDirectionOfChange && (input.myUV.y < (diff + glowWidth)))
    {
        float factor = ((diff + glowWidth) - input.myUV.y) / (glowWidth);
        textureColor.r = lerp(textureColor.r, glowColor.r, factor);
        textureColor.g = lerp(textureColor.g, glowColor.g, factor);
        textureColor.b = lerp(textureColor.b, glowColor.b, factor);
    }
    else if (!verticalDirectionOfChange && (input.myUV.x > (level - glowWidth)))
    {
        float factor = ((level - glowWidth) - input.myUV.x) / (glowWidth);
        textureColor.r = lerp(textureColor.r, glowColor.r, factor);
        textureColor.g = lerp(textureColor.g, glowColor.g, factor);
        textureColor.b = lerp(textureColor.b, glowColor.b, factor);
    }
    
    returnValue.myColor = textureColor;

    return returnValue;
}