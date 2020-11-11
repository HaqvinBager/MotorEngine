#pragma once
#include "Behaviour.h"
#include "Particle.h"
#include <SimpleMath.h>
#include <queue>

class CGameObject;

class CParticleEmitterComponent : public CBehaviour
{
public:
	CParticleEmitterComponent(CGameObject& aParent);
	~CParticleEmitterComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void Init(CParticle* aParticle);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Vector3 aRotation);
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);
	void SetScale(float aScale);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);

	void Update(float aDeltaTime, DirectX::SimpleMath::Vector3 aCameraPosition);

	void OnEnable() override;
	void OnDisable() override;
public:
	CParticle* GetParticle() { return myParticle; }
	std::vector<CParticle::SParticleVertex>& GetParticleVertices() { return myParticleVertices; }
	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; }

private:
	void SpawnParticles(float aDeltaTime, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& particleData);
	void UpdateParticles(float aDeltaTime, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& particleData);

private:
	CParticle* myParticle;
	DirectX::SimpleMath::Matrix myTransform;
	std::vector<CParticle::SParticleVertex> myParticleVertices;
	std::queue<CParticle::SParticleVertex> myParticlePool;
	float myEmitterTimer;
	float myScale = 1.0f;
};

