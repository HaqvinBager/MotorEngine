#pragma once
#include "SimpleMath.h"
#include "Particle.h"
#include <queue>

class CParticleInstance
{
public:
	CParticleInstance();
	~CParticleInstance();

	void Init(CParticle* aParticle);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Vector3 aRotation);
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);

	void Update(float aDeltaTime, DirectX::SimpleMath::Vector3 aCameraPosition);

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
	float myStartDelay;
};

