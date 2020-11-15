#pragma once
#include <array>
#include <d3d11.h>

class CAnimation;

class CModel {
public:
	struct SModelData {
		UINT myNumberOfVertices = 0;
		UINT myNumberOfIndices = 0;
		UINT myStride = 0;
		UINT myOffset = 0;
		ID3D11Buffer* myVertexBuffer = nullptr;
		ID3D11Buffer* myIndexBuffer = nullptr;
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11SamplerState* mySamplerState = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
		ID3D11InputLayout* myInputLayout = nullptr;
		std::array<ID3D11ShaderResourceView*, 3> myTexture { };
		//ID3D11Buffer* myBonesBuffer = nullptr;
		//std::vector<CAnimation*> myAnimations;
	};

public:
	CModel();
	~CModel();

	void Init(SModelData data);
	SModelData& GetModelData();

private:
	SModelData myModelData;
};

	// CAnimation is only used in CAnimationComponent
	//int AddAnimation(CAnimation* anAnimation);
	//int CModel::AddAnimation(CAnimation* aAnimation)
	//{
	//	myAnimations.push_back(aAnimation);
	//	return static_cast<int>(myAnimations.size());
	//}
	//const std::vector<CAnimation*>& GetAnimations() const { return myAnimations; }
	//private:
	//std::vector<CAnimation*> myAnimations;