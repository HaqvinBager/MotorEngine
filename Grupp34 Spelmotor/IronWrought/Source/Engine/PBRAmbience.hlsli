#include "SamplingFunctions.hlsli"

float RoughnessFromPerceptualRoughness(float fPerceptualRoughness)
{
    return fPerceptualRoughness * fPerceptualRoughness;
}

float PerceptualRoughnessFromRoughness(float fRoughness)
{
    return sqrt(max(0.0, fRoughness));
}

float SpecularPowerFromPerceptualRoughness(float fPerceptualRoughness)
{
    float fRoughness = RoughnessFromPerceptualRoughness(fPerceptualRoughness);
    return (2.0 / max(FLT_EPSILON, fRoughness * fRoughness)) - 2.0;
}

float PerceptualRoughnessFromSpecularPower(float fSpecPower)
{
    float fRoughness = sqrt(2.0 / (fSpecPower + 2.0));
    return PerceptualRoughnessFromRoughness(fRoughness);

}

int GetNumMips(TextureCube cubeTex)
{
    int iWidth = 0, iHeight = 0, numMips = 0;
    cubeTex.GetDimensions(0, iWidth, iHeight, numMips);
    return numMips;
}

float BurleyToMip(float fPerceptualRoughness, int nMips, float NdotR)
{
    float fSpecPower = SpecularPowerFromPerceptualRoughness(fPerceptualRoughness);
    fSpecPower /= (4 * max(NdotR, FLT_EPSILON));
    float fScale = PerceptualRoughnessFromSpecularPower(fSpecPower);
    return fScale * (nMips - 1 - nMipOffset);
}

float3 GetSpecularDominantDir(float3 vN, float3 vR, float fRealRoughness)
{
    float fInvRealRough = saturate(1 - fRealRoughness);
    float lerpFactor = fInvRealRough * (sqrt(fInvRealRough) + fRealRoughness);
    return lerp(vN, vR, lerpFactor);
}

float GetReductionInMicrofacets(float perceptualRoughness)
{
    float roughness = RoughnessFromPerceptualRoughness(perceptualRoughness);
    return 1.0 / (roughness * roughness + 1.0);
}

float EmpiricalSpecularAO(float ao, float perceptualRoughness)
{
    float fSmooth = 1 - perceptualRoughness;
    float fSpecAo = gain(ao, 0.5 + max(0.0, fSmooth * 0.4));
    return min(1.0, fSpecAo + lerp(0.0, 0.5, fSmooth * fSmooth * fSmooth * fSmooth));
}

float ApproximateSpecularSelfOcclusion(float3 vR, float3 vertNormalNormalized)
{
    const float fFadeParam = 1.3;
    float rimmask = clamp(1 + fFadeParam * dot(vR, vertNormalNormalized), 0.0, 1.0);
    rimmask *= rimmask;
    return rimmask;
}

float3 EvaluateAmbience(TextureCube lysBurleyCube, float3 vN, float3 org_normal, float3 to_cam, float perceptualRoughness, float metalness, float3 albledo, float ao, float3 dfcol, float3 spccol)
{
	int numMips = GetNumMips(lysBurleyCube);
	const int nrBrdfMips = numMips - nMipOffset;
	float VdotN = clamp(dot(to_cam, vN), 0.0, 1.0f);
	const float3 vRorg = 2 * vN * VdotN - to_cam;

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
	fFade *= ApproximateSpecularSelfOcclusion(vR, org_normal);

	float3 ambientDiffuse = ao * dfcol * diffRad;
	float3 ambientSpecular = fFade * spccol * specRad;
	return ambientDiffuse + ambientSpecular;
}