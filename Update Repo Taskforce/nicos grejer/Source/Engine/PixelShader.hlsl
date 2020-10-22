#include "PBRAmbiance.hlsli"
#include "PBRDirectionalLight.hlsli"

float3 EvaluatePointLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float lightIntensity, float lightRange, float3 lightPos, float3 toEye, float3 pixelPos)
{
    float3 toLight = lightPos.xyz - pixelPos.xyz;
    float lightDistance = length(toLight);
    toLight = normalize(toLight);
    float NdL = saturate(dot(normal, toLight));
    float lambert = NdL; //angle attenuation
    float NdV = saturate(dot(normal, toEye));
    float3 h = normalize(toLight + toEye);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(toEye, h));
    float LdV = saturate(dot(toLight, toEye));
    float a = max(0.001f, roughness * roughness);
    
    float3 cDiff = Diffuse(albedoColor);
    float3 cSpec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    return saturate(lightColor * lightIntensity * lambert * linearAttenuation * physicalAttenuation * ((cDiff * (1.0f - cSpec) + cSpec) * PI));
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    float3 toEye = normalize(cameraPosition.xyz - input.myWorldPosition.xyz);
    float3 albedo = PixelShader_Albedo(input).myColor.rgb;
    float3 normal = PixelShader_Normal(input).myColor.xyz;
    float ambientOcclusion = PixelShader_AmbientOcclusion(input).myColor.r;
    float metalness = PixelShader_Metalness(input).myColor.r;
    float perceptualRoughness = PixelShader_PerceptualRoughness(input).myColor.r;
    float emissiveData = PixelShader_Emissive(input).myColor.r;
    
    float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);
    
    float3 ambiance = EvaluateAmbiance(environmentCubemapTexture, normal, input.myNormal.xyz, toEye, perceptualRoughness, metalness, albedo, ambientOcclusion, diffuseColor, specularColor);
    float3 directionalLight = EvaluateDirectionalLight(diffuseColor, specularColor, normal, perceptualRoughness, directionalLightColor.xyz, toDirectionalLight.xyz, toEye.xyz);
    float3 emissive = albedo * emissiveData;
    
    float3 pointLights = float3(0.0f, 0.0f, 0.0f);
    
    for (unsigned int index = 0; index < myNumberOfUsedPointLights; index++)
    {
        PointLight currentLight = myPointLights[index];
        pointLights += EvaluatePointLight(diffuseColor, specularColor, normal, perceptualRoughness, currentLight.myColor.rgb, currentLight.myColor.a, currentLight.myRange, currentLight.myPosition.xyz, toEye, input.myWorldPosition.xyz);
    }
    
    float3 radiance = ambiance + directionalLight + pointLights + emissive;
    
    output.myColor.rgb = LinearToGamma(radiance);
    
    output.myColor.a = 1.0f;
    
    //output.myColor.rgb = albedo.rgb;
    //output.myColor.rgb = diffuseColor.rgb;
    //output.myColor.rgb = normal.rgb;
    //output.myColor.rgb = metalness;
    //output.myColor.rgb = ambientOcclusion;
    //output.myColor.rgb = perceptualRoughness;
    //output.myColor.rgb = emissive;
    return output;
    
    //PixelOutput returnValue;
    //float3 textureColor = albedoTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    //float3 textureNormal = normalTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    //textureNormal = (textureNormal * 2) - 1;
    
    //float3x3 tangentSpaceMatrix = float3x3(normalize(input.myTangent.xyz), normalize(input.myBinormal.xyz), normalize(input.myNormal.xyz));
    //tangentSpaceMatrix = transpose(tangentSpaceMatrix);
    //float3 pixelNormal = mul(tangentSpaceMatrix, textureNormal.xyz);
    
    //float3 environmentColor = environmentCubemapTexture.SampleLevel(defaultSampler, pixelNormal, 8).rgb;
    //float attenuation = saturate(dot(pixelNormal, toDirectionalLight.xyz));
    
    //float3 ambient = textureColor * environmentColor;
    //float3 diffuse = textureColor * attenuation * directionalLightColor.rgb;
    
    //float3 returnColor = ambient + diffuse;
    //returnValue.myColor = float4(returnColor.xyz, 1);
    //return returnValue;
}