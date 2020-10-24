#include "SpriteShaderStructs.hlsli"

VertexToGeometry main(VertexInput input)
{
    VertexToGeometry output;
    output.myPosition = myPosition;
    output.myColor = myColor;
    output.myUVRect = myUVRect;
    output.mySize = mySize;
	return output;
}