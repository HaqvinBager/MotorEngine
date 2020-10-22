#include "ShaderStructs.hlsli"

#define FLT_EPSILON 1.192092896e-07f
#define PI 3.14f
#define nMipOffset 3

PixelOutput PixelShader_AmbientOcclusion(VertexToPixel input) {
	PixelOutput output;
	float ao = normalTexture.Sample(defaultSampler, input.myUV.xy).a;
	output.myColor.rgb = ao.xxx;
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_Material(VertexToPixel input) {
	PixelOutput output;
	float3 material = materialTexture.Sample(defaultSampler, input.myUV.xy).rgb;
	output.myColor.rgb = material.rgb;
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_Metalness(VertexToPixel input) {
	PixelOutput output;
	PixelOutput material = PixelShader_Material(input);
	output.myColor.rgb = material.myColor.rrr;
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_PerceptualRoughness(VertexToPixel input) {
	PixelOutput output;
	PixelOutput material = PixelShader_Material(input);
	output.myColor.rgb = material.myColor.ggg;
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_Emissive(VertexToPixel input) {
	PixelOutput output;
	PixelOutput material = PixelShader_Material(input);
	output.myColor.rgb = material.myColor.bbb;
	output.myColor.a = 1.0f;
	return output;
}

float3 LinearToGamma(float3 aColor) {
	return pow(abs(aColor), 1.0 / 2.2);
}

float3 GammaToLinear(float3 aColor) {
	return pow(abs(aColor), 2.2);
}

PixelOutput PixelShader_Albedo(VertexToPixel input) {
	PixelOutput output;
	float3 albedo = diffuseTexture.Sample(defaultSampler, input.myUV.xy).rgb;
	albedo = GammaToLinear(albedo);
	output.myColor.rgb = albedo;
	output.myColor.a = 1.0f;
	return output;
}

PixelOutput PixelShader_Normal(VertexToPixel input) {
	float3 normal = normalTexture.Sample(defaultSampler, input.myUV.xy).rgb;
	normal = (normal * 2) - 1;
	normal = normalize(normal);
    
	float3x3 tangentSpaceMatrix = float3x3(normalize(input.myTangent.xyz), normalize(input.myBinormal.xyz), normalize(input.myNormal.xyz));
	normal = mul(normal.xyz, tangentSpaceMatrix);
	normal = normalize(normal);
    
	PixelOutput output;
	output.myColor.xyz = normal.xyz;
	output.myColor.a = 1.0f;
	return output;
}

float bias(float value, float b) {
	return (b > 0.0) ? pow(abs(value), log(b) / log(0.5)) : 0.0;
}

float gain(float value, float g) {
	return 0.5 * ((value < 0.5) ? bias(2.0 * value, 1.0 - g) : (2.0 - bias(2.0 - 2.0 * value, 1.0 - g)));
}

float RoughnessFromPerceptualRoughness(float fPerceptualRoughness) {
	return fPerceptualRoughness * fPerceptualRoughness;
}

float PerceptualRoughnessFromRoughness(float fRoughness) {
	return sqrt(max(0.0, fRoughness));
}

float SpecularPowerFromPerceptualRoughness(float fPerceptualRoughness) {
	float fRoughness = RoughnessFromPerceptualRoughness(fPerceptualRoughness);
	return (2.0 / max(FLT_EPSILON, fRoughness * fRoughness)) - 2.0;
}

float PerceptualRoughnessFromSpecularPower(float fSpecPower) {
	float fRoughness = sqrt(2.0 / (fSpecPower + 2.0));
	return PerceptualRoughnessFromRoughness(fRoughness);
}

int GetNumMips(TextureCube cubeTex) {
	int iWidth = 0, iHeight = 0, numMips = 0;
	cubeTex.GetDimensions(0, iWidth, iHeight, numMips);
	return numMips;
}

float BurleyToMip(float fPerceptualRoughness, int nMips, float NdotR) {
	float fSpecPower = SpecularPowerFromPerceptualRoughness(fPerceptualRoughness);
	fSpecPower /= (4 * max(NdotR, FLT_EPSILON));
	float fScale = PerceptualRoughnessFromSpecularPower(fSpecPower);
	return fScale * (nMips - 1 - nMipOffset);
}

float3 GetSpecularDominantDir(float3 vN, float3 vR, float fRealRoughness) {
	float fInvRealRough = saturate(1 - fRealRoughness);
	float lerpFactor = fInvRealRough * (sqrt(fInvRealRough) + fRealRoughness);
    
	return lerp(vN, vR, lerpFactor);
}

float GetReductionInMicrofacets(float perceptualRoughness) {
	float roughness = RoughnessFromPerceptualRoughness(perceptualRoughness);
	return 1.0 / (roughness * roughness + 1.0);
}

float EmpiricalSpecularAO(float ao, float perceptualRoughness) {
	float fSmooth = 1 - perceptualRoughness;
	float fSpecAo = gain(ao, 0.5 + max(0.0, fSmooth * 0.4));
    
	return min(1.0, fSpecAo + lerp(0.0, 0.5, fSmooth * fSmooth * fSmooth * fSmooth));
}

float ApproximateSpecularSelfOcclusion(float3 vR, float3 vertNormalNormalized) {
	const float fFadeParam = 1.3;
	float rimmMask = clamp(1 + fFadeParam * dot(vR, vertNormalNormalized), 0.0, 1.0);
	rimmMask *= rimmMask;
    
	return rimmMask;
}

float3 EvaluateAmbiance(TextureCube lysBurleyCube, float3 vN, float3 orgNormal, float3 toCam, float perceptualRoughness, float metalness, float3 albedo, float ao, float3 dfcol, float3 spccol) {
	int numMips = GetNumMips(lysBurleyCube);
	const int nrBrdfMips = numMips - nMipOffset;
	float VdotN = clamp(dot(toCam, vN), 0.0, 1.0f);
	const float3 vRorg = 2 * vN * VdotN - toCam;
    
	float3 vR = GetSpecularDominantDir(vN, vRorg, RoughnessFromPerceptualRoughness(perceptualRoughness));
	float RdotNsat = saturate(dot(vN, vR));
    
	float l = BurleyToMip(perceptualRoughness, numMips, RdotNsat);
    
	float3 specRad = lysBurleyCube.SampleLevel(defaultSampler, vR, l).xyz;
	float3 diffRad = lysBurleyCube.SampleLevel(defaultSampler, vN, (float) (nrBrdfMips - 1)).xyz;
    
	float fT = 1.0 - RdotNsat;
	float fT5 = fT * fT;
	fT5 = fT5 * fT5 * fT;
	spccol = lerp(spccol, (float3) 1.0, fT5);
    
	float fFade = GetReductionInMicrofacets(perceptualRoughness);
	fFade *= EmpiricalSpecularAO(ao, perceptualRoughness);
	fFade *= ApproximateSpecularSelfOcclusion(vR, orgNormal);
    
	float3 ambientDiffuse = ao * dfcol * diffRad;
	float3 ambientSpecular = fFade * spccol * specRad;
	return ambientDiffuse + ambientSpecular;
}

float3 Diffuse(float3 pAlbedo) {
	return pAlbedo / PI;
}

float NormalDistribution_GGX(float a, float NdH) {
    // Isotropic ggx
	float a2 = a * a;
	float NdH2 = NdH * NdH;
    
	float denominator = NdH2 * (a2 - 1.0f) + 1.0f;
	denominator *= denominator;
	denominator *= PI;
    
	return a2 / denominator;
}

float Specular_D(float a, float NdH) {
	return NormalDistribution_GGX(a, NdH);
}

float Geometric_Smith_Schlick_GGX(float a, float NdV, float NdL) {
    // Smith schlick-GGX
	float k = a * 0.5f;
	float GV = NdV / (NdV * (1 - k) + k);
	float GL = NdL / (NdL * (1 - k) + k);
    
	return GV * GL;
}

float Specular_G(float a, float NdV, float NdL, float NdH, float VdH, float LdV) {
	return Geometric_Smith_Schlick_GGX(a, NdV, NdL);
}

float3 Fresnel_Schlick(float3 specularColor, float3 h, float3 v) {
	return (specularColor + (1.0f - specularColor) * pow((1.0f - saturate(dot(v, h))), 5));
}

float3 Specular_F(float3 specularColor, float3 h, float3 v) {
	return Fresnel_Schlick(specularColor, h, v);
}

float3 Specular(float3 specularColor, float3 h, float3 v, float3 l, float a, float NdL, float NdV, float NdH, float VdH, float LdV) {
	return ((Specular_D(a, NdH) * Specular_G(a, NdV, NdL, NdH, VdH, LdV)) * Specular_F(specularColor, v, h)) / (4.0f * NdL * NdV + 0.0001f);
}

float3 EvaluateDirectionalLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float3 lightDir, float3 viewDir) {
    // Compute some useful values
	float NdL = saturate(dot(normal, lightDir));
	float lambert = NdL; // Angle attenuation
	float NdV = saturate(dot(normal, viewDir));
	float3 h = normalize(lightDir + viewDir);
	float NdH = saturate(dot(normal, h));
	float VdH = saturate(dot(viewDir, h));
	float LdV = saturate(dot(lightDir, viewDir));
	float a = max(0.001f, roughness * roughness);
    
	float3 cDiff = Diffuse(albedoColor);
	float3 cSpec = Specular(specularColor, h, viewDir, lightDir, a, NdL, NdV, NdH, VdH, LdV);
    
	return saturate(lightColor * lambert * (cDiff * (1.0f - cSpec) + cSpec) * PI);
}

float3 EvaluatePointLight(float3 albedoColor, float3 specularColor, float3 normal, float roughness, float3 lightColor, float lightIntensity, float lightRange, float3 lightPos, float3 toEye, float3 pixelPos) {
	//Compute some useful values
	float3 toLight = lightPos.xyz - pixelPos.xyz;
	float lightDistance = length(toLight);
	toLight = normalize(toLight);
	float NdL = saturate(dot(normal, toLight));
	float lambert = NdL; //Angle attenuation
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

PixelOutput main(VertexToPixel input) {
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
    
	float3 ambiance = EvaluateAmbiance(environmentTexture, normal, input.myNormal.xyz, toEye, perceptualRoughness, metalness, albedo, ambientOcclusion, diffuseColor, specularColor);
	float3 directionalLight = EvaluateDirectionalLight(diffuseColor, specularColor, normal, perceptualRoughness, directionalLightColor.xyz, toDirectionalLight.xyz, toEye.xyz);
	
	float3 pointLights = 0;
	for (unsigned int index = 0; index < myNumberOfUsedPointLights; index++) {
		PointLight currentLight = myPointLights[index];
		pointLights += EvaluatePointLight(diffuseColor, specularColor, normal, perceptualRoughness, currentLight.myColorAndRange.rgb, currentLight.myPositionAndIntensity.w, currentLight.myColorAndRange.a, currentLight.myPositionAndIntensity.xyz, toEye, input.myWorldPosition.xyz);
	}
	
	float3 emissive = albedo * emissiveData;
	
	//Boost bloom
	directionalLight *= 2.0f;
	//Boost bloom
	
	float3 radiance = ambiance + directionalLight + pointLights + emissive;
    
	output.myColor.rgb = LinearToGamma(radiance);
	output.myColor.a = 1.0f;
    
    //output.myColor.rgb = albedo.rgb;
    //output.myColor.rgb = normal.rgb;
    //output.myColor.rgb = metalness;
    //output.myColor.rgb = ambientOcclusion;
    //output.myColor.rgb = perceptualRoughness;
    //output.myColor.rgb = emissive;
    //output.myColor.rgb = diffuseColor.rgb;
    //output.myColor.rgb = specularColor.rgb;
	
	return output;
}