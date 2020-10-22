#include "pch.h"
#include "Line.hpp"

Line::Line() 
	//: myDevice(nullptr)
{}

Line::~Line()
{
	//myDevice = nullptr;
}

void Line::Init(SLineData& aLineData)
{
	myLineData = aLineData;
}

//void Line::Set(const DirectX::SimpleMath::Vector3& aFrom, const DirectX::SimpleMath::Vector3& aTo, const DirectX::SimpleMath::Vector4& aColor)
//{
//	//if (!myDevice)
//	//{
//	//	return;
//	//}
//	//struct SVertex
//	//{
//	//	float myX, myY, myZ, myW;
//	//	float myR, myG, myB, myA;
//	//} vertices[2] = 
//	//{
//	//	 {aFrom.x, aFrom.y, aFrom.z, 1,  aColor.x, aColor.y, aColor.z, 1}
//	//	,{aTo.x, aTo.y, aTo.z, 1,		 aColor.x, aColor.y, aColor.z, 1}
//	//};
//	//
//	////D3D11_MAPPED_SUBRESOURCE mappedResource;
//	////ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
//	//////  Disable GPU access to the vertex buffer data.
//	////auto d3dContext = m_deviceResources->GetD3DDeviceContext();
//	////m_d3dContext->Map(vertexBuffer2.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
//	//////  Update the vertex buffer here.
//	////memcpy(mappedResource.pData, vertices, sizeof(vertices));
//	//////  Reenable GPU access to the vertex buffer data.
//	////m_d3dContext->Unmap(vertexBuffer2.Get(), 0);
//	//
//	//HRESULT hResult;
//	//
//	//D3D11_BUFFER_DESC bufferDescription = {0};
//	//bufferDescription.ByteWidth =	sizeof(vertices);
//	//bufferDescription.Usage		=	D3D11_USAGE_IMMUTABLE;
//	//bufferDescription.BindFlags =	D3D11_BIND_VERTEX_BUFFER;
//	//
//	//D3D11_SUBRESOURCE_DATA subresourceData = {0};
//	//subresourceData.pSysMem = vertices;
//	//
//	//hResult = myDevice->CreateBuffer(&bufferDescription, &subresourceData, &myLineData.myVertexBuffer);
//	//if( FAILED( hResult ) )
//	//{
//	//	return;
//	//}
//}

const Line::SLineData& Line::GetLineData()
{
	return myLineData;
}

const Line::SLineData& Line::GetLineData() const
{
	return myLineData;
}
