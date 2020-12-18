#include "SpriteVFXShaderStructs.hlsli"

PixelOutput main(GeometryToPixel input)
{
    float2 uv;
    uv = input.myUV.xy;

    float uvSquishFactor = 1 / (1 - (2 * maskOffset));
    float squishedLevel = level;
    squishedLevel /= uvSquishFactor;
    squishedLevel += maskOffset;
    float diff = (1.0f - squishedLevel);
    float maxLevel = 1.0f;
    maxLevel /= uvSquishFactor;
    maxLevel += maskOffset;
    
    if (verticalDirectionOfChange && (uv.y < diff) || (!verticalDirectionOfChange && uv.x > squishedLevel))
    {
        discard;
    }

    PixelOutput returnValue;    
    
    float2 uvLevelOffset = { 0.0f, 0.0f };
    if (verticalDirectionOfChange)
    {
        uvLevelOffset.y = -(maxLevel - squishedLevel);
    }
    else
    {
        uvLevelOffset.x = (maxLevel - squishedLevel);
    }
    
    float4 textureColor = instanceTexture1.Sample(defaultSampler, (uv * uvScale1) + randomOffset + scrollSpeed1 * scrollTimer).rgba;
    float4 textureTwoColor = instanceTexture2.Sample(defaultSampler, (uv * uvScale2) + randomOffset + scrollSpeed2 * scrollTimer).rgba;
    float4 textureThreeColor = instanceTexture3.Sample(defaultSampler, (uv * uvScale3) + randomOffset + scrollSpeed3 * scrollTimer).rgba;
    float4 textureFourColor = instanceTexture4.Sample(defaultSampler, (uv * uvScale4) + scrollSpeed4 * scrollTimer).rgba;
    float4 textureFiveColor = instanceTexture5.Sample(defaultSampler, (uv * uvScale5) + uvLevelOffset + scrollSpeed5 * scrollTimer).rgba;

    textureColor = (((textureColor * textureTwoColor * 2.0f) * textureThreeColor * 2.0f) * textureFourColor) * textureFiveColor;
    
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
    else if (!verticalDirectionOfChange && (input.myUV.x > (squishedLevel - glowWidth)))
    {
        factor = ((squishedLevel - glowWidth) - input.myUV.x) / (-glowWidth);
    }

    textureColor.r = lerp(textureColor.r, glowColor.r, factor);
    textureColor.g = lerp(textureColor.g, glowColor.g, factor);
    textureColor.b = lerp(textureColor.b, glowColor.b, factor);
    
    returnValue.myColor = textureColor;

    return returnValue;
}