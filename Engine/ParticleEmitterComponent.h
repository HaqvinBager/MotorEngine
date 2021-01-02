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

	void Init(std::vector<CParticle*> someParticles);
	void SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Vector3 aRotation);
	void SetRotation(DirectX::SimpleMath::Quaternion aQuaternion);
	void SetPosition(DirectX::SimpleMath::Vector3 aPosition);
	void SetScale(float aScale);
	void Move(DirectX::SimpleMath::Vector3 aMovement);
	void Rotate(DirectX::SimpleMath::Vector3 aRotation);
	void Rotate(DirectX::SimpleMath::Quaternion aQuaternion);

	void Update(DirectX::SimpleMath::Vector3 aCameraPosition);

	void OnEnable() override;
	void OnDisable() override;
	void Reset();
public:
	std::vector<CParticle*>& GetParticleSet() { return myParticles; }
	std::vector<std::vector<CParticle::SParticleVertex>>& GetParticleVertices() { return myParticleVertices; }
	DirectX::SimpleMath::Matrix GetTransform() { return myTransform; }
	const std::vector<float> EmitterDurations() const { return myEmitterDurations; }
private:
	void SpawnParticles(unsigned int anIndex, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& particleData);
	void UpdateParticles(unsigned int anIndex, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& particleData);

private:
	std::vector<CParticle*> myParticles;
	DirectX::SimpleMath::Matrix myTransform;
	std::vector<std::vector<CParticle::SParticleVertex>> myParticleVertices;
	std::vector<std::queue<CParticle::SParticleVertex>> myParticlePools;
	std::vector<float> myEmitterTimers;
	std::vector<float> myEmitterDelays;
	std::vector<float> myEmitterDurations;
	std::vector<DirectX::SimpleMath::Vector3> myOffsets;
	float myScale = 10.0f;
};

