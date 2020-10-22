#include "PBRAmbience.hlsli"
#include "PBRDirectionalLight.hlsli"



float3 EvaluatePointLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float lightIntensity, float lightRange, float3 lightPos, float3 toEye, float3 pixelPos)
{
    float3 toLight = lightPos.xyz - pixelPos.xyz;
    float lightDistance = length(toLight);
    toLight = normalize(toLight);
    float NdL = saturate(dot(normal, toLight));
    
    float lambert = NdL;
    float NdV = saturate(dot(normal, toEye));
    float3 h = normalize(toLight + toEye);
    float NdH = saturate(dot(normal, h));
    float VdH = saturate(dot(toEye, h));
    float LdV = saturate(dot(toLight, toEye));
    float a = max(0.001f, roughness * roughness);
    
    float3 cDiff = Diffuse(albedoColor);
    float3 cSPec = Specular(specularColor, h, toEye, toLight, a, NdL, NdV, NdH, VdH, LdV);
    
    float linearAttenuation = lightDistance / lightRange;
    linearAttenuation = 1.0f - linearAttenuation;
    linearAttenuation = saturate(linearAttenuation);
    float physicalAttenuation = saturate(1.0f / (lightDistance * lightDistance));
    float attenuation = lambert * linearAttenuation * physicalAttenuation;
    
    return saturate(lightColor * lightIntensity * lambert * linearAttenuation * physicalAttenuation * ((cDiff * (1.0f - cSPec) + cSPec) * PI));
}

PixelOutPut main(VertexToPixel input)
{
    PixelOutPut output;
    
    float3 toEye = normalize(cameraPosition.xyz - input.myWorldPosition.xyz);
    float3 albedo = PixelShader_Albedo(input).myColor.rgb;
    float3 normal = PixelShader_Normal(input).myColor.xyz;
    
    float ambientocclusion = PixelShader_AmbientOcclusion(input).myColor.r;
    float metalness = PixelShader_Metalness(input).myColor.r;
    float perceptualroughness = PixelShader_PerceptualRoughness(input).myColor.r;
    float emissivedata = PixelShader_Emissive(input).myColor.r;
    
    float3 specularcolor = lerp((float3) 0.04, albedo, metalness);
    float3 diffusecolor = lerp((float3) 0.00, albedo, 1 - metalness);
    
    float3 ambience = EvaluateAmbience(environmentTexture, normal, input.myNormal.xyz, toEye, perceptualroughness, metalness, albedo, ambientocclusion, diffusecolor, specularcolor);
    float3 directionallight = EvaluateDirectionalLight(diffusecolor, specularcolor, normal, perceptualroughness, directionalLightColor.xyz, toDirectionalLight.xyz, toEye.xyz);
 
    float3 pointLights = 0;
    for (unsigned int index = 0; index < myNumberOfUsedPointLights; index++)
    {
        PointLight currentLight = myPointLights[index];
        pointLights += EvaluatePointLight(diffusecolor, specularcolor, normal, perceptualroughness, currentLight.myColorAndRange.rgb, currentLight.myPositionAndIntensity.w, currentLight.myColorAndRange.a, currentLight.myPositionAndIntensity.xyz, toEye, input.myWorldPosition.xyz);
    }
    
    float3 emissive = albedo * emissivedata;
    float3 radiance = ambience + directionallight + pointLights +  emissive;
   
    output.myColor.rgb = LinearToGamma(radiance);
    output.myColor.a = 1.0f;
    
    return output;
}