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

	struct SModelInstanceData {
		UINT myNumberOfVertices = 0;
		UINT myNumberOfIndices = 0;
		UINT myStride[2] = { 0, 0 };
		UINT myOffset[2] = { 0, 0 };
		ID3D11Buffer* myVertexBuffer = nullptr;
		ID3D11Buffer* myIndexBuffer = nullptr;
		ID3D11Buffer* myInstanceBuffer = nullptr;
		ID3D11VertexShader* myVertexShader = nullptr;
		ID3D11PixelShader* myPixelShader = nullptr;
		ID3D11SamplerState* mySamplerState = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY myPrimitiveTopology;
		ID3D11InputLayout* myInputLayout = nullptr;
		std::array<ID3D11ShaderResourceView*, 3> myTexture{ };
	};

	struct SInstanceType {
		DirectX::SimpleMath::Matrix myTransform;
	};

public:
	CModel();
	~CModel();

	void Init(SModelData data);
	SModelData& GetModelData();

public:
	void Init(SModelInstanceData aData);
	SModelInstanceData& GetModelInstanceData() { return myModelInstanceData; }

	void InstanceCount(int aCount);
	const int& InstanceCount() const { return myInstanceCount; }

	/*bool AddInstancedTransform(DirectX::SimpleMath::Matrix aTransform);
	std::vector<DirectX::SimpleMath::Matrix>& GetInstancedTransform() { return myTransforms; }*/

private:
	SModelData myModelData;

private:
	SModelInstanceData myModelInstanceData;
	int myInstanceCount;
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