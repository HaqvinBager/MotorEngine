#include "stdafx.h"
#include "VFXRenderer.h"
#include "VFXComponent.h"
#include "DirectXFramework.h"
#include "Camera.h"
#include "VFXInstance.h"
#include "VFXBase.h"

CVFXRenderer::CVFXRenderer() : myContext(nullptr), myFrameBuffer(), myObjectBuffer(), myTextureScrollingBuffer(), myTime(0.0f) {
}

CVFXRenderer::~CVFXRenderer() {
}

bool CVFXRenderer::Init(CDirectXFramework* aFramework) {
    myContext = aFramework->GetContext();
    if (!myContext) {
        return false;
    }

    ID3D11Device* device = aFramework->GetDevice();
    if (!device) {
        return false;
    }

    D3D11_BUFFER_DESC bufferDescription = { 0 };
    bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    bufferDescription.ByteWidth = sizeof(SFrameBufferData);
    ENGINE_HR_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myFrameBuffer), "Framebuffer could not be created.");

    bufferDescription.ByteWidth = sizeof(SObjectBufferData);
    ENGINE_HR_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myObjectBuffer), "Objectbuffer could not be created");

    bufferDescription.ByteWidth = sizeof(STextureScrollingData);
    ENGINE_HR_MESSAGE(device->CreateBuffer(&bufferDescription, nullptr, &myTextureScrollingBuffer), "Scrollbuffer could not be created");

    return true;
}

void CVFXRenderer::Render(CCamera* aCamera, std::vector<CGameObject*>& aGameObjectList) 
{
	myFrameBufferData.myToCamera = aCamera->GetTransform().Invert();
	myFrameBufferData.myToProjection = aCamera->GetProjection();

	BindBuffer<SFrameBufferData>(myFrameBuffer, myFrameBufferData, "Frame Buffer");

	myContext->VSSetConstantBuffers(0, 1, &myFrameBuffer);
	myContext->PSSetConstantBuffers(0, 1, &myFrameBuffer);

	for (CGameObject* gameobject : aGameObjectList) {
		
		CVFXComponent* component = gameobject->GetComponent<CVFXComponent>();
		if (component == nullptr)
			continue;

		CVFXBase* vfxBase = component->GetVFXBase();
		if (vfxBase == nullptr)
			continue;
		
		CVFXBase::SVFXBaseData vfxBaseData = vfxBase->GetVFXBaseData();

		myObjectBufferData.myToWorld = component->GetTransform();
		BindBuffer<SObjectBufferData>(myObjectBuffer, myObjectBufferData, "Object Buffer");

		myTime += CTimer::Dt();
		if (myTime > FLT_MAX - 1.0f) {
			myTime -= FLT_MAX - 1.0f;
		}

		float scrollingScale = 0.15f;
		myTextureScrollingData.scrollSpeed1 = vfxBaseData.scrollSpeed1;
		myTextureScrollingData.scrollSpeed2 = vfxBaseData.scrollSpeed2;
		myTextureScrollingData.scrollSpeed3 = vfxBaseData.scrollSpeed3;
		myTextureScrollingData.scrollSpeed4 = vfxBaseData.scrollSpeed4;
		myTextureScrollingData.uvScale1 = vfxBaseData.uvScale1;
		myTextureScrollingData.uvScale2 = vfxBaseData.uvScale2;
		myTextureScrollingData.uvScale3 = vfxBaseData.uvScale3;
		myTextureScrollingData.uvScale4 = vfxBaseData.uvScale4;
		myTextureScrollingData.scrollTimer = myTime * scrollingScale; //..is now!
		myTextureScrollingData.opacityStrength = vfxBaseData.opacityStrength;
		BindBuffer<STextureScrollingData>(myTextureScrollingBuffer, myTextureScrollingData, "Texture Scrolling Buffer");

		myContext->IASetPrimitiveTopology(vfxBaseData.myPrimitiveTopology);
		myContext->IASetInputLayout(vfxBaseData.myInputLayout);

		myContext->IASetVertexBuffers(0, 1, &vfxBaseData.myVertexBuffer, &vfxBaseData.myStride, &vfxBaseData.myOffset);
		myContext->IASetIndexBuffer(vfxBaseData.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		myContext->VSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->VSSetShader(vfxBaseData.myVertexShader, nullptr, 0);

		myContext->GSSetShader(nullptr, nullptr, 0);

		myContext->PSSetConstantBuffers(1, 1, &myObjectBuffer);
		myContext->PSSetConstantBuffers(2, 1, &myTextureScrollingBuffer);
		myContext->PSSetShaderResources(0, 4, &vfxBaseData.myTexture[0]);
		myContext->PSSetShader(vfxBaseData.myPixelShader, nullptr, 0);

		myContext->PSSetSamplers(0, 1, &vfxBaseData.mySampler);

		myContext->DrawIndexed(vfxBaseData.myNumberOfIndices, 0, 0);

		////Reset PS Resources
		//ID3D11ShaderResourceView* nullView = NULL;
		//myContext->PSSetShaderResources(0, 1, &nullView);
		//myContext->PSSetShaderResources(1, 1, &nullView);
	}
}