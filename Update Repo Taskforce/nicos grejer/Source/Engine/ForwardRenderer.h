#include <vector>
#include "SimpleMath.h"

struct ID3D11DeviceContext;
struct ID3D11Buffer;
class CDirectXFramework;
class CModelInstance;
class CModel;
class CCamera;
class CEnvironmentLight;
class CPointLight;

class CForwardRenderer
{
public:
	CForwardRenderer();
	~CForwardRenderer();

	bool Init(CDirectXFramework* aFramework);

	void Render(CEnvironmentLight* anEnvironmentLight, std::vector<std::pair<unsigned int, std::array<CPointLight*, 8>>> somePointLights, CCamera* aCamera, std::vector<CModelInstance*>& aModelList);
	void RenderOutline(CCamera* aCamera, CModelInstance* aModelInstance, CModel* someOutlineModelData);
private:
	template<class T>
	void BindBuffer(ID3D11Buffer* aBuffer, T& someBufferData, std::string aBufferType)
	{
		D3D11_MAPPED_SUBRESOURCE bufferData;
		ZeroMemory(&bufferData, sizeof(D3D11_MAPPED_SUBRESOURCE));
		std::string errorMessage = aBufferType + " could not be bound.";
		ENGINE_HR_MESSAGE(myContext->Map(aBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &bufferData), errorMessage.c_str());

		memcpy(bufferData.pData, &someBufferData, sizeof(T));
		myContext->Unmap(aBuffer, 0);
	}

private:
	struct SFrameBufferData
	{
		DirectX::SimpleMath::Matrix myToCamera;
		DirectX::SimpleMath::Matrix myToProjection;
		DirectX::SimpleMath::Vector4 myCameraPosition;
		DirectX::SimpleMath::Vector4 myDirectionalLightDirection;
		DirectX::SimpleMath::Vector4 myDirectionalLightColor;
		//UINT myEnvironmentLightMipCount;
		//UINT padding[3];
	} myFrameBufferData;
	
	struct SObjectBufferData
	{
		DirectX::SimpleMath::Matrix myToWorld;
		struct SPointLightBufferData 
		{
			DirectX::SimpleMath::Vector4 myColor;
			DirectX::SimpleMath::Vector3 myPosition;
			float myRange;
		} myPointLights[8];

		UINT myNumberOfUsedPointLights;
		int padding[3];
	} myObjectBufferData;

	struct STextureScrollingData {
		DirectX::SimpleMath::Vector2 scroll;
		DirectX::SimpleMath::Vector2 scroll2;
	} myTextureScrollingData;

private:
	ID3D11DeviceContext* myContext;
	ID3D11Buffer* myFrameBuffer;
	ID3D11Buffer* myObjectBuffer;
	ID3D11Buffer* myTextureScrollingBuffer;
};

