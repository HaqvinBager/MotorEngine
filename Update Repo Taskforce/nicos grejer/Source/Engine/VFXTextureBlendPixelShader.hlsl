#include "VFXShaderStructs.hlsli"

PixelOutput main(VertexToPixel input)
{
    PixelOutput returnValue;
    float4 textureColor         = instanceTexture1.Sample(defaultSampler, (input.myUV.xy * uvScale1) + scrollSpeed1 * scrollTimer).rgba;
    float4 textureTwoColor      = instanceTexture2.Sample(defaultSampler, (input.myUV.xy * uvScale2) + scrollSpeed2 * scrollTimer).rgba;
    float4 textureThreeColor    = instanceTexture3.Sample(defaultSampler, (input.myUV.xy * uvScale3) + scrollSpeed3 * scrollTimer).rgba;
    float4 textureFourColor     = instanceTexture4.Sample(defaultSampler, (input.myUV.xy * uvScale4) + scrollSpeed4 * scrollTimer).rgba;
    
    //Generate alpha
    textureColor.a = (textureColor.r + textureColor.g + textureColor.b) * opacityStrength;
    textureTwoColor.a = (textureTwoColor.r + textureTwoColor.g + textureTwoColor.b) * opacityStrength;
    textureThreeColor.a = (textureThreeColor.r + textureThreeColor.g + textureThreeColor.b) * opacityStrength;
    textureFourColor.a = (textureFourColor.r + textureFourColor.g + textureFourColor.b) * opacityStrength;
    textureColor.a = ((textureColor.a * textureTwoColor.a * 2.0f) * textureThreeColor.a * 2.0f) * textureFourColor.a * 2.0f;
   
    //Glow
    //textureColor.r = lerp(0.7f, 1.0f, textureColor.a);
    //textureColor.g = lerp(0.0f, 1.0f, textureColor.a * 0.75f);
    //textureColor.b = lerp(0.0f, 0.5f, textureColor.a);
    
    returnValue.myColor = textureColor;
    return returnValue;
}