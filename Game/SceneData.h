#pragma once
struct SCameraData
{
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
	float myFieldOfView;
	float myStartInCameraMode;
	float myToggleFreeCamKey;
	float myFreeCamMoveSpeed;
	DirectX::SimpleMath::Vector3 myOffset;
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

struct SEventData {
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector2 myColliderData;
	int myEvent;
};

struct SEnemyData {
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
	DirectX::SimpleMath::Vector3 myScale;
	float myHealth;
	float myDamage;
	float myMoveSpeed;
	float myVisionRange;
	float myAttackRange;
	int myModelIndex;
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
	std::vector<SEventData> myEventData;
	std::vector<SEnemyData> myEnemyData;
	std::vector<SGameObjectData> myGameObjects;
	int mySceneIndex;

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