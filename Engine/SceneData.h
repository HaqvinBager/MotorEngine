#pragma once
struct SCameraData
{
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
	float myFieldOfView;
	float myStartInCameraMode;
	float myToggleFreeCamKey;
	float myFreeCamMoveSpeed;
};

struct SDirectionalLightData
{
	DirectX::SimpleMath::Vector3 myDirection;
	DirectX::SimpleMath::Vector3 myColor;
	float myIntensity;
};

struct SPointLightData
{
	int myInstanceID;
	DirectX::SimpleMath::Vector3 myPosition;
	float myRange;
	DirectX::SimpleMath::Vector3 myColor;
	float myIntensity;
};

struct SPlayerData
{
	int myInstanceID;
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
	DirectX::SimpleMath::Vector3 myScale;
	int myModelIndex;
	//Player Health osv
};

struct SGameObjectData
{
	int myInstanceID;
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
	DirectX::SimpleMath::Vector3 myScale;
	int myModelIndex;
};

//class SData { 
//public:
//	SData() { }
//	virtual ~SData() { }
//};

struct SInGameData 
{
	SCameraData myCamera;	
	SDirectionalLightData myDirectionalLight;
	std::vector<SPointLightData> myPointLightData;	
	SPlayerData myPlayerData;
	std::vector<SGameObjectData> myGameObjects;

	int SizeOf()
	{
		int size = 0;
		size += sizeof(myCamera);
		size += sizeof(myDirectionalLight);
		for (auto& light : myPointLightData)
			size += sizeof(light);
		size += sizeof(myPlayerData);
		for (auto& gameObject : myGameObjects)
			size += sizeof(gameObject);

		return size;
	}

};

struct SLoadScreenData
{
	SCameraData myCamera;
	SDirectionalLightData myDirectionalLight;
	SGameObjectData myGameObject;
};