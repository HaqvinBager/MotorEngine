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
	int myInstanceID;
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
	float myDamageCooldown;
	float myVisionRange;
	float myAttackRange;
	int myModelIndex;
};

struct SDestructibleData {
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
	DirectX::SimpleMath::Vector3 myScale;
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

struct SEnvironmentFXData
{
	int myInstanceID;
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
	DirectX::SimpleMath::Vector3 myScale;
};

struct SParticleFXData
{
	int myInstanceID;
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
};

struct SBossData
{
	int myInstanceID;
	int myModelIndex;
	DirectX::SimpleMath::Vector3 myPosition;
	DirectX::SimpleMath::Vector3 myRotation;
	DirectX::SimpleMath::Vector3 myScale;
	DirectX::SimpleMath::Vector2 myStageOne;
	DirectX::SimpleMath::Vector2 myStageTwo;
	DirectX::SimpleMath::Vector2 myStageThree;
};

struct SInGameData 
{
	SCameraData myCamera;	
	SDirectionalLightData myDirectionalLight;
	std::vector<SPointLightData> myPointLightData;	
	SPlayerData myPlayerData;
	std::vector<SEventData> myEventData;
	std::vector<SEnemyData> myEnemyData;
	std::vector<SDestructibleData> myDestructibleData;
	std::vector<SGameObjectData> myGameObjects;
	//int mySceneIndex;
	std::vector<SEnvironmentFXData> myEnvironmentFXs;
	std::vector<SParticleFXData> myParticleFXs;
	SBossData myBossData;
	
	std::unordered_map<int, std::string> myEventStringMap;
	std::unordered_map<int, std::string> myEnvironmentFXStringMap;
	std::unordered_map<int, std::vector<std::string>> myParticleFXStringMap;

	std::string myBinPath;

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

	std::string myBinPath;
};