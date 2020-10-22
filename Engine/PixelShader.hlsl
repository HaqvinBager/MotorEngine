#include "ShaderStructs.hlsli"

PixelOutPut main(VertexToPixel input)
{
    PixelOutPut returnValue;
    
    float3 textureColor = diffuseTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    float3 textureNormal = normalTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    textureNormal = (textureNormal * 2) - 1;
    
    float3x3 tangentSpaceMatrix = float3x3(normalize(input.myTangent.xyz), normalize(input.myBiNormal.xyz), normalize(input.myNormal.xyz));
    tangentSpaceMatrix = transpose(tangentSpaceMatrix);
    float3 pixelNormal = mul(tangentSpaceMatrix, textureNormal.xyz);
    
    float3 environmentColor = environmentTexture.SampleLevel(defaultSampler, pixelNormal, 8).rgb;
    float attenuation = saturate(dot(pixelNormal, toDirectionalLight.xyz));
    
    float3 ambient = textureColor * environmentColor;
    float3 diffuse = textureColor * attenuation * directionalLightColor.rgb;
    
    float3 returnColor = ambient + diffuse;
   
    returnValue.myColor = float4(returnColor.xyz, 1.0f);
    //returnValue.myColor = input.myColor;
    return returnValue;
}
    //float4(pixelNormal, 1);