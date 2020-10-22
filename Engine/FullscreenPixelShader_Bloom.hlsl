#include "FullscreenShaderStructs.hlsli"

PixelOutput main(VertexToPixel input) {
	PixelOutput returnValue;
	float3 resource = fullscreenTexture1.Sample(defaultSampler, input.myUV.xy).rgb;
	float3 resource2 = fullscreenTexture2.Sample(defaultSampler, input.myUV.xy).rgb;
	
	//Snippet 1
	{
		returnValue.myColor.rgb = resource + resource2;
	}
	//Snippet 1
	
	//Snippet 2
	//{
	//	resource *= (1.0f - saturate(resource2));
	//	returnValue.myColor.rgb = resource + resource2;
	//}
	//Snippet 2
	
	//Snippet 3
	//{
	//float luminance = dot(resource, float3(0.2126f, 0.7152f, 0.0722f));
	//resource2 *= (1.0f - luminance);
	//returnValue.myColor.rgb = resource + resource2;
	//}
	//Snippet 3
	
	returnValue.myColor.a = 1.0f;
	return returnValue;
};