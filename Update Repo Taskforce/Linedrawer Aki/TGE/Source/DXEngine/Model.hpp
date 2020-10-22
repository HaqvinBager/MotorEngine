#pragma once
/// <Todo>
/// 1 Add Texture member to SModelData
/// </Todo>

struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11ShaderResourceView;

#include <d3dcommon.h>
//enum D3D_PRIMITIVE_TOPOLOGY;
//typedef D3D_PRIMITIVE_TOPOLOGY D3D11_PRIMITIVE_TOPOLOGY;

namespace ModelSystem
{
	class ModelInstance;

	class Model
	{
	public:
		struct SModelData
		{
			UINT						myNumberOfVertices	= 0;
			UINT						myNumberOfIndices	= 0;
			UINT						myStride			= 0;
			UINT						myOffset			= 0;
			ID3D11Buffer*				myVertexBuffer		= nullptr;
			ID3D11Buffer*				myIndexBuffer		= nullptr;
			ID3D11VertexShader*			myVertexShader		= nullptr;
			ID3D11PixelShader*			myPixelShader		= nullptr;
			D3D_PRIMITIVE_TOPOLOGY		myPrimitiveTopology = static_cast<D3D_PRIMITIVE_TOPOLOGY>(0);
			ID3D11InputLayout*			myInputLayout		= nullptr;
			ID3D11ShaderResourceView*	myTexture			= nullptr;

			std::vector<ID3D11ShaderResourceView*> myTextures;
		};

		Model();
		~Model();

		void Init(SModelData& aModelData);

		const SModelData& GetModelData();
		const SModelData& GetModelData() const;

	private:
		SModelData myModelData;
	};	
}