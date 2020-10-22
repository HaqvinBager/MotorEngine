#pragma once
#include <d3d11.h>

class CModel {
public:
    struct SModelData {
        UINT myNumberOfVertecies;
        UINT myNumberOfIndices;
        UINT myStride;
        UINT myOffset;
        ID3D11Buffer* myVertexBuffer;
        ID3D11Buffer* myIndexBuffer;
        ID3D11VertexShader* myVertexShader;
        ID3D11PixelShader* myPixelShader;
        D3D_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
        ID3D11InputLayout* myInputLayout;
        ID3D11ShaderResourceView* myTexture[3];
        ID3D11SamplerState* mySampler;
    };

	CModel();
	~CModel();
	void Init(SModelData aModelData);
    SModelData GetModelData();
	
private:
    SModelData myModelData;
};

