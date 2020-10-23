#pragma once
struct ID3D11Buffer;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;

struct ID3D11Device;

#include <d3dcommon.h>
//enum D3D_PRIMITIVE_TOPOLOGY;
//typedef D3D_PRIMITIVE_TOPOLOGY D3D11_PRIMITIVE_TOPOLOGY;

#include "SimpleMath.h"

/// <To do>
///		Add to DebugTools namespace
/// </To do>

class CLine
{
public:
	struct SLineData
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
	};

	CLine();
	~CLine();

	void Init(SLineData& aLineData);

	// Currently does not work well. 
	// memory usage increases on every call. (since we are creating a new buffer everytime)
	//void Set(const DirectX::SimpleMath::Vector3& aFrom, const DirectX::SimpleMath::Vector3& aTo, const DirectX::SimpleMath::Vector4& aColor);

	const SLineData& GetLineData();
	const SLineData& GetLineData() const;
public:
	// Will remove this, find another solution. This is mostly for testing
	//void SetDevice(ID3D11Device* aDevice) { myDevice = aDevice; }

private:
	SLineData myLineData;
	//ID3D11Device* myDevice;// yikes
};	

