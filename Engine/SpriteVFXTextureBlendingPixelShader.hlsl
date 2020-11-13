#include "SpriteVFXShaderStructs.hlsli"

PixelOutput main(GeometryToPixel input)
{
    float diff = (1.0f - level);
    if ((input.myUV.y < diff && verticalDirectionOfChange) || (input.myUV.x > level && !verticalDirectionOfChange))
    {
        discard;
    }

    PixelOutput returnValue;
    float4 textureColor = instanceTexture1.Sample(defaultSampler, (input.myUV.xy * uvScale1) + scrollSpeed1 * scrollTimer).rgba;
    float4 textureTwoColor = instanceTexture2.Sample(defaultSampler, (input.myUV.xy * uvScale2) + scrollSpeed2 * scrollTimer).rgba;
    float4 textureThreeColor = instanceTexture3.Sample(defaultSampler, (input.myUV.xy * uvScale3) + scrollSpeed3 * scrollTimer).rgba;
    float4 textureFourColor = instanceTexture4.Sample(defaultSampler, (input.myUV.xy * uvScale4) + scrollSpeed4 * scrollTimer).rgba;
    
    textureColor = ((textureColor * textureTwoColor * 2.0f) * textureThreeColor * 2.0f) * textureFourColor * 2.0f;
    
    //Glow
    //textureColor.r = lerp(0.7f, 1.0f, textureColor.a);
    //textureColor.g = lerp(0.0f, 1.0f, textureColor.a * 0.75f);
    //textureColor.b = lerp(0.0f, 0.5f, textureColor.a);
    //textureColor.a = 1.0f;
    
    returnValue.myColor = textureColor;

    if ((input.myUV.y < diff && verticalDirectionOfChange) || (input.myUV.x > level && !verticalDirectionOfChange))
    {
        returnValue.myColor.r *= 5.5f * diff;
        returnValue.myColor.g *= 5.5f * diff;
        returnValue.myColor.b *= 5.5f * diff;
    }

    return returnValue;
}