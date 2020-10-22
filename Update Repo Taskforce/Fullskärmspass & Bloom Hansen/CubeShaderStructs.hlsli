struct VertexInput {
    float4 myPosition   :   POSITION;
    float4 myColor      :   COLOR;
    float2 myUV         :   UV;
};

struct VertexToPixel {
    float4 myPosition   :   SV_POSITION;
    float4 myColor      :   COLOR;
    float2 myUV         :   UV;
};

struct PixelOutput {
    float4 myColor      :   SV_TARGET;
};

cbuffer FrameBuffer : register(b0) {
	float4 cameraPosition;
	float4x4 toCamera;
	float4x4 toProjection;
	float4 toDirectionalLight;
	float4 directionalLightColor;
	unsigned int environmentLightMipCount;
	unsigned int trash[3];
}

cbuffer ObjectBuffer : register(b1) {
	float4x4 toWorld;
    
	struct PointLight {
		float4 myPositionAndIntensity;
		float4 myColorAndRange;
	} myPointLights[8];

	unsigned int myNumberOfUsedPointLights;
	unsigned int myTrash[3];
}

TextureCube environmentTexture : register(t0);
Texture2D instanceTexture : register(t1);
Texture2D materialTexture : register(t2);
Texture2D normalTexture : register(t3);
SamplerState defaultSampler : register(s0);
