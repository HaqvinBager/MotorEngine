#include "FullscreenShaderStructs.hlsli"

VertexToPixel main(VertexInput input) {
	float4 positions[3] = {
		float4(-1.0f, -1.0f, 0.0f, 1.0f),
		float4(-1.0f, 3.0f, 0.0f, 1.0f),
		float4(3.0f, -1.0f, 0.0f, 1.0f)
	};
	
	float2 uvs[3] = { 
		float2(0.0f, 1.0f),
		float2(0.0f, -1.0f),
		float2(2.0f, 1.0f)
	};
	
	VertexToPixel returnValue;
	returnValue.myPosition = positions[input.myIndex];
	returnValue.myUV = uvs[input.myIndex];
	return returnValue;
};