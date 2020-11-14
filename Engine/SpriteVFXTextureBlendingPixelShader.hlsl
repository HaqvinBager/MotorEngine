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
    float4 textureFiveColor = instanceTexture5.Sample(defaultSampler, input.myUV.xy).rgba;

    textureColor = ((textureColor * textureTwoColor * 2.0f) * textureThreeColor * 2.0f) * textureFourColor * 2.0f;
    
    //Glow
    //textureColor.r = lerp(0.7f, 1.0f, textureColor.a);
    //textureColor.g = lerp(0.0f, 1.0f, textureColor.a * 0.75f);
    //textureColor.b = lerp(0.0f, 0.5f, textureColor.a);
    //textureColor.a = 1.0f;
    
    //if ((input.myUV.y < (diff + 0.1f) && verticalDirectionOfChange))
    //{
    //    float factor = ((diff + 0.1f) - input.myUV.y) / (0.1f);
    //    textureColor.r = lerp(textureColor.r, 1.0f, input.myUV.y);
    //    textureColor.g = lerp(textureColor.g, 1.0f, input.myUV.y);
    //    textureColor.b = lerp(textureColor.b, 1.0f, input.myUV.y);
    //}

    //else if (input.myUV.x > (level - 0.1f) && !verticalDirectionOfChange)
    //{
    //    float factor = ((level - 0.1f) - input.myUV.x) / (0.1f);
    //    textureColor.r = lerp(textureColor.r, 1.0f, input.myUV.x);
    //    textureColor.g = lerp(textureColor.g, 1.0f, input.myUV.x);
    //    textureColor.b = lerp(textureColor.b, 1.0f, input.myUV.x);
    //}
    
    returnValue.myColor = textureColor;

    return returnValue;
}