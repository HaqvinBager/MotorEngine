#include "FullscreenShaderStructs.hlsli"

VertexToPixel main(VertexInput input)
{
    float4 positions[3] =
    {
        //One tri
        float4(-1.0f, -1.0f, 0.0f, 1.0f),
        float4(-1.0f, 3.0f, 0.0f, 1.0f),
        float4(3.0f, -1.0f, 0.0f, 1.0f),
        
        //Two tris
		//float4(-1.0f, -1.0f, 0.0f, 1.0f), //bottom left
        //float4(-1.0f, 1.0f, 0.0f, 1.0f), //top left
        //float4(1.0f, 1.0f, 0.0f, 1.0f), //top right
        //float4(1.0f, 1.0f, 0.0f, 1.0f), //top right
        //float4(1.0f, -1.0f, 0.0f, 1.0f), //bottom right
        //float4(-1.0f, -1.0f, 0.0f, 1.0f), //bottom left
    };
    
    float2 uvs[3] =
    {
        float2(0.0f, 1.0f), //bottom left                  
        float2(0.0f, -1.0f), //high left
        float2(2.0f, 1.0f) //bottom far right
        
        //float2(0.0f, 1.0f),
        //float2(0.0f, 0.0f),
        //float2(1.0f, 0.0f),
        //float2(1.0f, 0.0f),
        //float2(1.0f, 1.0f),
        //float2(0.0f, 1.0f),
    };
    
    VertexToPixel returnValue;
    returnValue.myPosition = positions[input.myIndex];
    returnValue.myUV = uvs[input.myIndex];
	return returnValue;
}