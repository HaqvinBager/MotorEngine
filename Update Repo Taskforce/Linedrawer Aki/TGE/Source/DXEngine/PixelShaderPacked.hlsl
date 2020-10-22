#include "ShaderStructsPacked.hlsli"

#define FLT_EPSILON 1.192092896e-07f
#define nMipOffset 3

float3 LinearToGamma(float3 color)
{
    return pow(abs(color), 1.0f / 2.2f);
}
float3 GammaToLinear(float3 color)
{
    return pow(abs(color), 2.2f);
}
float Bias(float value, float bias)
{
    return (bias > 0.0f) ? pow(abs(value), log(bias) / log(0.5f)) : 0.0f;
}
float Gain(float value, float gain)
{
    return 0.5f * ((value < 0.5f) ? Bias(2.0f * value, 1.0f - gain) : (2.0f - Bias(2.0f - 2.0f * value, 1.0f - gain)));

}

float RoughnessFromPerceptualRoughness(float fPercetualRoughness)
{
    return fPercetualRoughness * fPercetualRoughness;
}
float PerceptualRoughnessFromRoughness(float fRoughness)
{
    return sqrt(max(0.0f, fRoughness));
}
float SpecularPowerFromPerceptualRoughness(float fPerceptualRoughness)
{
    float fRoughness = RoughnessFromPerceptualRoughness(fPerceptualRoughness);
    return (2.0f / max(FLT_EPSILON, fRoughness * fRoughness)) - 2.0;
}
float PerceptualRoughnessFromSpecularPower(float fSpecPower)
{
    float fRoughness = sqrt(2.0f / (fSpecPower + 2.0f));
    return PerceptualRoughnessFromRoughness(fRoughness);
}

int GetNumMips(TextureCube cubeTex)
{
    int iWidth = 0, iHeight = 0, numMips = 0;
    cubeTex.GetDimensions(0, iWidth, iHeight, numMips);
    return numMips;
}

float BurleyToMip(float fPerceptualRoughness, int nMips, float NormalDotRoughness)
{
    float fSpecPower = SpecularPowerFromPerceptualRoughness(fPerceptualRoughness);
    fSpecPower /= (4 * max(NormalDotRoughness, FLT_EPSILON));
    float fScale = PerceptualRoughnessFromSpecularPower(fSpecPower);
    return fScale * (nMips - 1 - nMipOffset);
}
float3 GetSpecularDominantDir(float3 vertexNormal, float3 vertexRoughness, float fRealRoughness)
{
    float fInvRealRough = saturate(1 - fRealRoughness);
    float lerpFactor = fInvRealRough * (sqrt(fInvRealRough) + fRealRoughness);
    return lerp(vertexNormal, vertexNormal, lerpFactor);
}
float GetReductionInMicrofacets(float perceptualRoughness)
{
    float roughness = RoughnessFromPerceptualRoughness(perceptualRoughness);
    return 1.0f / (roughness * roughness + 1.0f);
}
float EmpiricalSpecularAO(float ao, float perceptualRoughness)
{
    float fSmooth = 1 - perceptualRoughness;
    float fSpecAo = Gain(ao, 0.5f + max(0.0f, fSmooth * 0.4f));
    return min(1.0f, fSpecAo + lerp(0.0f, 0.5f, fSmooth * fSmooth * fSmooth * fSmooth));
}
float ApproximateSpecularSelfOcclusion(float3 vertexRoughness, float3 vertexNormalNormalized)
{
    const float fFadeParam = 1.3f;
    float rimMask = clamp(1 + fFadeParam * dot(vertexRoughness, vertexNormalNormalized), 0.0f, 1.0f);
    return rimMask;
}


PixelOutput PixelShader_AmbientOcclusion(VertexToPixel input)
{
    PixelOutput output;
    float ao = normalTexture.Sample(defaultSampler, input.myUV.xy).a;
    output.myColor.rgb = ao.xxx;
    output.myColor.a = 1.0f;
    return output;
}
PixelOutput PixelShader_Material(VertexToPixel input)
{
    PixelOutput output;
    float3 material         = materialTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    output.myColor.rgb      = material.rgb;
    output.myColor.a        = 1.0f;
    return output;
}
PixelOutput PixelShader_Metalness(VertexToPixel input)
{
    PixelOutput output;
    PixelOutput material = PixelShader_Material(input);
    output.myColor.rgb = material.myColor.rrr;
    output.myColor.a = 1.0f;
    return output;
}
PixelOutput PixelShader_PerceptualRoughness(VertexToPixel input)
{
    PixelOutput output;
    PixelOutput material = PixelShader_Material(input);
    output.myColor.rgb = material.myColor.ggg;
    output.myColor.a = 1.0f;
    return output;
}
PixelOutput PixelShader_Emissive(VertexToPixel input)
{
    PixelOutput output;
    PixelOutput material = PixelShader_Material(input);
    output.myColor.rgb = material.myColor.bbb;
    output.myColor.a = 1.0f;
    return output;
}

PixelOutput PixelShader_Normal(VertexToPixel input)
{
    float3 normal = normalTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    normal = (normal * 2) - 1;
    normal = normalize(normal); //new
    
    float3x3 tangentspacematrix = float3x3(normalize(input.myTangent.xyz)
                                         , normalize(input.myBinormal.xyz)
                                         , normalize(input.myNormal.xyz));
    tangentspacematrix = transpose(tangentspacematrix);
    normal = mul(tangentspacematrix, normal.xyz);
    normal = normalize(normal); //new
    
    PixelOutput output;
    output.myColor.xyz = normal.xyz;
    output.myColor.a = 1.0f;
    return output;
}

PixelOutput PixelShader_Albedo(VertexToPixel input)
{
    PixelOutput output;
    float3 albedo = albedoTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    albedo = GammaToLinear(albedo);
    output.myColor.rgb = albedo;
    output.myColor.a = 1.0f;
    return output;
}

float3 EvaluateAmbience(TextureCube lysBurleyCube, float3 vertexNormal, float3 orgNormal, float3 toCam, float perceptualRougness, float metalness, float3 albedo, float ao, float3 diffuseColor, float3 specColor)
{
    int numMips = GetNumMips(lysBurleyCube);
    const int nrBrdfMips = numMips - nMipOffset;
    float VertexDotNormal = clamp(dot(toCam, vertexNormal), 0.0f, 1.0f);
    const float3 vertexRoughnessOrg = 2 * vertexNormal * VertexDotNormal - toCam;
    
    float3 vertexRoughness = GetSpecularDominantDir(vertexNormal, vertexRoughnessOrg, RoughnessFromPerceptualRoughness(perceptualRougness));
    float RoughnessDotNormalSaturation = saturate(dot(vertexNormal, vertexRoughness));
    
    float l = BurleyToMip(perceptualRougness, numMips, RoughnessDotNormalSaturation);
    
    float3 specRad = lysBurleyCube.SampleLevel(defaultSampler, vertexRoughness, l).xyz;
    float3 diffRad = lysBurleyCube.SampleLevel(defaultSampler, vertexNormal, (float) (nrBrdfMips - 1)).xyz;
    
    
    float fT = 1.0f - RoughnessDotNormalSaturation;
    float fT5 = fT * fT;
    fT5 = fT5 * fT5 * fT;
    specColor = lerp(specColor, (float3) 1.0f, fT5);
    
    float fFade = GetReductionInMicrofacets(perceptualRougness);
    fFade *= EmpiricalSpecularAO(ao, perceptualRougness);
    fFade *= ApproximateSpecularSelfOcclusion(vertexRoughness, orgNormal);
    
    float3 ambientDiffuse = ao * diffuseColor * diffRad;
    float3 ambientSpecular = fFade * specColor * specRad;
    
    return ambientDiffuse + ambientSpecular;
}

static float PI = 3.14f;
float3 Diffuse(float3 pAlbedo)
{
    return pAlbedo / PI;
}
float NormalDistributionGGX(float a, float NormalDotH)
{
    // Isotropic ggx.
    float a2 = a * a;
    float NormalDotH2 = NormalDotH * NormalDotH;
    
    float denominator = NormalDotH2 * (a2 - 1.0f) + 1.0f;
    denominator *= denominator;
    denominator *= PI;
    
    return a2 / denominator;
}
float SpecularD(float a, float NormalDotH)
{
    return NormalDistributionGGX(a, NormalDotH);
}
float GeometricSmithSchlickGGX(float a, float NormalDotView, float NormalDotLight)
{
    // Smith Schlick ggx.
    float k = a * 0.5f;
    float GV = NormalDotView / (NormalDotView * (1 - k) + k);
    float GL = NormalDotLight / (NormalDotLight * (1 - k) + k);
    
    return GV * GL;
}
float SpecularG(float a, float NormalDotView, float NormalDotLight, float NormalDotH, float ViewDotH, float LightDotView)
{
    return GeometricSmithSchlickGGX(a, NormalDotView, NormalDotLight);
}
float3 FresnelSchlick(float3 specularColor, float3 h, float3 v)
{
    return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}
float3 SpecularF(float3 specularColor, float3 h, float3 v)
{
    return FresnelSchlick(specularColor, h, v);
}
float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float a, float NormalDotLight, float NormalDotView, float NormalDotH, float ViewDotH, float LightDotView)
{
    return ((SpecularD(a, NormalDotH) * SpecularG(a, NormalDotView, NormalDotLight, NormalDotH, ViewDotH, LightDotView)) * SpecularF(specularColor, v, h)) / (4.0f * NormalDotLight * NormalDotView + 0.0001f);

}
float3 EvaluateDirectionalLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float3 lightDir, float3 viewDir)
{
    //Compute some useful values.
    float NormalDotLight = saturate(dot(normal, lightDir));
    float lambert = NormalDotLight; //angle attenuation.
    float NormalDotView = saturate(dot(normal, viewDir));
    float3 h = normalize(lightColor + viewDir);
    float NormalDotH = saturate(dot(normal, h));
    float ViewDotH = saturate(dot(viewDir, h));
    float LightDotView = saturate(dot(lightDir, viewDir));
    float a = max(0.000f, roughness * roughness);
    
    float3 colDiff = Diffuse(albedoColor);
    float3 colSpec = Specular(specularColor, h, viewDir, lightDir, a, NormalDotLight, NormalDotView, NormalDotH, ViewDotH, LightDotView);
    
    return saturate(lightColor * lambert * (colDiff * (1.0f - colSpec) + colSpec) * PI);
}

PixelOutput main(VertexToPixel input)
{
    PixelOutput output;
    
    float3 toEye = normalize(cameraPosition.xyz - input.myPosition.xyz);
    float3 albedo = PixelShader_Albedo(input).myColor.rgb;
    float3 normal = PixelShader_Normal(input).myColor.xyz;
    float metalness = PixelShader_Metalness(input).myColor.r;
    float perceptualRoughness = PixelShader_PerceptualRoughness(input).myColor.r;
    float ambientocclusion = PixelShader_AmbientOcclusion(input).myColor.r;
    float emissiveData = PixelShader_Emissive(input).myColor.r;
    
    float3 specularColor = lerp((float3) 0.04, albedo, metalness);
    float3 diffuseColor = lerp((float3) 0.00, albedo, 1 - metalness);
    
    float3 ambience = EvaluateAmbience(environmentTexture, normal, input.myNormal.xyz, toEye, perceptualRoughness, metalness, albedo, ambientocclusion, diffuseColor, specularColor);
    float3 directionalLight = EvaluateDirectionalLight(diffuseColor, specularColor, normal, perceptualRoughness, directionalLightColor.xyz, toDirectionalLight.xyz, toEye.xyz);
    float3 emissive = albedo * emissiveData;
    float3 radiance = ambience + directionalLight + emissive;
    
    output.myColor.rgb = LinearToGamma(radiance);
    output.myColor.a = 1.0f;
    return output;
}
// Pre PBR
/*
    // Uncoment desired texture for display
	//float3 textureColor     = albedoTexture.Sample(defaultSampler, input.myUV.xy).rgb;
	//float3 textureColor     = metalTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    //float3 textureColor     = roughTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    //float3 textureColor     = aoTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    //float3 textureColor     = emissiveTexture.Sample(defaultSampler, input.myUV.xy).rgb;
	//float3 textureColor     = normalTexture.Sample(defaultSampler, input.myUV.xy).rgb;
    
	float3 textureNormal    = normalTexture.Sample(defaultSampler, input.myUV.xy).rgb;
           textureNormal   = (textureNormal * 2) - 1;
    
    float3x3 tangentspacematrix = float3x3(normalize(input.myTangent.xyz)
                                         , normalize(input.myBinormal.xyz)
                                         , normalize(input.myNormal.xyz));
    tangentspacematrix = transpose(tangentspacematrix);
    float3 pixelNormal = mul(tangentspacematrix, textureNormal.xyz);
    
	float3 environmentColor = environmentTexture.SampleLevel(defaultSampler, pixelNormal, 8).rgb;// SampleLevel 8-9 is high, Shinier objects should have lower values.
	float attenuation = saturate(dot(pixelNormal, toDirectionalLight.xyz));//Saturate clamps value to be between 0 and 1 (Lambert light calc can give negative values).
    
	float3 ambient = textureColor * environmentColor;
	float3 diffuse = textureColor * attenuation * directionalLightColor.rgb;
    
	float3 returnColor = ambient + diffuse;
	
    output.myColor =float4(returnColor.xyz, 1).rgba;// use light
    //output.myColor = float4(textureColor.rgb, 1); // display only a single texture
    return output;
*/