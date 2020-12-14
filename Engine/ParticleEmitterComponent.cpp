#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "RandomNumberGenerator.h"
#include "CameraComponent.h"
#include "Engine.h"

namespace SM = DirectX::SimpleMath;
#define ENGINE_SCALE 0.01f

CParticleEmitterComponent::CParticleEmitterComponent(CGameObject& aParent) : CBehaviour(aParent)
{
	SetScale(1.0f);
	myTransform.Translation(GameObject().myTransform->Position());
}

CParticleEmitterComponent::~CParticleEmitterComponent()
{
	myParticles.clear();
}

void CParticleEmitterComponent::Awake()
{
}

void CParticleEmitterComponent::Start()
{
}

void CParticleEmitterComponent::Update()
{
	if (!Enabled()) return;

	//DirectX::SimpleMath::Vector3 gameObjectPos = GameObject().myTransform->Position();
	//GameObject().myTransform->Position({ 0.0f, 0.0f, 0.0f });
	SetPosition(GameObject().myTransform->Position());
	DirectX::SimpleMath::Vector3 scale;
	DirectX::SimpleMath::Quaternion quat;
	DirectX::SimpleMath::Vector3 translation;
	GameObject().myTransform->GetMatrix().Decompose(scale, quat, translation);
	SetRotation(quat);
	//GameObject().myTransform->Position(gameObjectPos);
	Update(CEngine::GetInstance()->GetActiveScene().GetMainCamera()->GameObject().myTransform->Position());
}

void CParticleEmitterComponent::Init(std::vector<CParticle*> someParticles)
{
	myParticles = someParticles;
	for (unsigned int i = 0; i < myParticles.size(); ++i) {
	
		myParticleVertices.emplace_back(std::vector<CParticle::SParticleVertex>());
		myParticlePools.emplace_back(std::queue<CParticle::SParticleVertex>());

		myParticleVertices[i].reserve(myParticles[i]->GetParticleData().myNumberOfParticles);
		for (unsigned int j = 0; j < myParticles[i]->GetParticleData().myNumberOfParticles; ++j) {
			myParticlePools[i].push(CParticle::SParticleVertex());
		}
		
		myEmitterDelays.emplace_back(myParticles[i]->GetParticleData().myDelay);
		myEmitterDurations.emplace_back(myParticles[i]->GetParticleData().myDuration);
		myEmitterTimers.emplace_back(0.0f);
	}
}

void CParticleEmitterComponent::SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CParticleEmitterComponent::SetRotation(DirectX::SimpleMath::Vector3 aRotation)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform *= SM::Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
}

void CParticleEmitterComponent::SetRotation(DirectX::SimpleMath::Quaternion aQuaternion)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromQuaternion(
		aQuaternion
	);
	myTransform *= SM::Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
}

void CParticleEmitterComponent::SetPosition(DirectX::SimpleMath::Vector3 aPosition)
{
	myTransform.Translation(aPosition);
}

void CParticleEmitterComponent::SetScale(float aScale)
{
	myScale = aScale;
	SM::Vector3 scale;
	SM::Vector3 translation;
	SM::Quaternion rotation;
	myTransform.Decompose(scale, rotation, translation);
	myTransform = SM::Matrix::CreateFromQuaternion(rotation);
	myTransform *= SM::Matrix::CreateScale(myScale * ENGINE_SCALE);
	myTransform.Translation(translation);
}

void CParticleEmitterComponent::Move(DirectX::SimpleMath::Vector3 aMovement)
{
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CParticleEmitterComponent::Rotate(DirectX::SimpleMath::Vector3 aRotation)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(translation);
}

void CParticleEmitterComponent::Rotate(DirectX::SimpleMath::Quaternion aQuaternion)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromQuaternion(aQuaternion);
	myTransform.Translation(translation);
}

void CParticleEmitterComponent::Update(DirectX::SimpleMath::Vector3 aCameraPosition)
{
	for (unsigned int i = 0; i < myParticles.size(); ++i) 
	{
		CParticle::SParticleData particleData = myParticles[i]->GetParticleData();

		if ((myEmitterDelays[i] -= CTimer::Dt()) > 0.0f) { continue; }

		if ((myEmitterDurations[i] -= CTimer::Dt()) > 0.0f) {
			SpawnParticles(i, aCameraPosition, particleData);
		}

		UpdateParticles(i, aCameraPosition, particleData);

		//std::sort(myParticleVertices[i].begin(), myParticleVertices[i].end(),
		//	[](const CParticle::SParticleVertex& aFirstParticle, const CParticle::SParticleVertex& aSecondParticle)
		//	{
		//		return aFirstParticle.mySquaredDistanceToCamera > aSecondParticle.mySquaredDistanceToCamera;
		//	}
		//);

	}
}

void CParticleEmitterComponent::OnEnable()
{
	Enabled(true);
	for (unsigned int i = 0; i < myParticles.size(); ++i) {
		myEmitterDelays[i] = myParticles[i]->GetParticleData().myDelay;
		myEmitterDurations[i] = myParticles[i]->GetParticleData().myDuration;
	}
}

void CParticleEmitterComponent::OnDisable()
{
	Enabled(false);
	for (unsigned int i = 0; i < myParticles.size(); ++i) {
		size_t currentSize = myParticleVertices[i].size();
		for (unsigned int j = 0; j < currentSize; ++j) {
			myParticlePools[i].push(myParticleVertices[i].back());
			myParticleVertices[i].pop_back();
		}
	}
}

void CParticleEmitterComponent::SpawnParticles(unsigned int anIndex, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& someParticleData)
{
	myEmitterTimers[anIndex] += CTimer::Dt();
	if (myEmitterTimers[anIndex] > (1.0f / someParticleData.mySpawnRate) && (myParticleVertices[anIndex].size() < someParticleData.myNumberOfParticles)) {
		myParticleVertices[anIndex].emplace_back(myParticlePools[anIndex].front());
		myParticlePools[anIndex].pop();
		myParticleVertices[anIndex].back().myLifeTime = someParticleData.myParticleLifetime + Random(someParticleData.myLifetimeLowerBound, someParticleData.myLifetimeUpperBound);
		myParticleVertices[anIndex].back().myPosition = 
			{ myTransform._41 + ((someParticleData.myOffsetPosition.x + Random(someParticleData.myOffsetLowerBound.x, someParticleData.myOffsetUpperBound.x)) * (1.0f /ENGINE_SCALE))
			, myTransform._42 + ((someParticleData.myOffsetPosition.y + Random(someParticleData.myOffsetLowerBound.y, someParticleData.myOffsetUpperBound.y)) * (1.0f / ENGINE_SCALE))
			, myTransform._43 + ((someParticleData.myOffsetPosition.z + Random(someParticleData.myOffsetLowerBound.z, someParticleData.myOffsetUpperBound.z)) * (1.0f / ENGINE_SCALE))
			, 1.0f 
			};
		myParticleVertices[anIndex].back().myMovement = someParticleData.myParticleStartDirection;
		myParticleVertices[anIndex].back().myStartMovement = someParticleData.myParticleStartDirection + Random(someParticleData.myDirectionLowerBound, someParticleData.myDirectionUpperBound, 0.0f);
		myParticleVertices[anIndex].back().myEndMovement = someParticleData.myParticleEndDirection + Random(someParticleData.myDirectionLowerBound, someParticleData.myDirectionUpperBound, 0.0f);
		myParticleVertices[anIndex].back().myColor = someParticleData.myParticleStartColor;
		myParticleVertices[anIndex].back().mySize = { someParticleData.myParticleStartSize, someParticleData.myParticleStartSize };
		myParticleVertices[anIndex].back().mySquaredDistanceToCamera = SM::Vector3::DistanceSquared({ myTransform._41, myTransform._42, myTransform._43 }, aCameraPosition);
		myEmitterTimers[anIndex] -= (1.0f / someParticleData.mySpawnRate);
	}
}

void CParticleEmitterComponent::UpdateParticles(unsigned int anIndex, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& particleData)
{
	std::vector<unsigned int> indicesOfParticlesToRemove;
	for (UINT i = 0; i < myParticleVertices[anIndex].size(); ++i)
	{
		float quotient = myParticleVertices[anIndex][i].myLifeTime / particleData.myParticleLifetime;

		myParticleVertices[anIndex][i].myColor = SM::Vector4::Lerp
		(
			particleData.myParticleEndColor,
			particleData.myParticleStartColor,
			quotient
		);

		myParticleVertices[anIndex][i].mySize = SM::Vector2::Lerp
		(
			{ particleData.myParticleEndSize, particleData.myParticleEndSize },
			{ particleData.myParticleStartSize, particleData.myParticleStartSize },
			quotient
		);

		myParticleVertices[anIndex][i].myMovement = SM::Vector4::Lerp
		(
			particleData.myParticleEndDirection,
			particleData.myParticleStartDirection,
			quotient
		);

		SM::Vector4 direction = SM::Vector4::Lerp
		(
			myParticleVertices[anIndex][i].myEndMovement,
			myParticleVertices[anIndex][i].myStartMovement,
			quotient
		);

		myParticleVertices[anIndex][i].mySquaredDistanceToCamera = SM::Vector3::DistanceSquared
		(
			{ myParticleVertices[anIndex][i].myPosition.x,
			  myParticleVertices[anIndex][i].myPosition.y,
			  myParticleVertices[anIndex][i].myPosition.z },
			aCameraPosition
		);

		SM::Vector4 newPosition = direction * particleData.myParticleSpeed * CTimer::Dt() + myParticleVertices[anIndex][i].myPosition;
		myParticleVertices[anIndex][i].myPosition = newPosition;

		if ((myParticleVertices[anIndex][i].myLifeTime -= CTimer::Dt()) < 0.0f) {
			indicesOfParticlesToRemove.emplace_back(i);
		}
	}

	std::sort(indicesOfParticlesToRemove.begin(), indicesOfParticlesToRemove.end(), [](UINT a, UINT b) { return a > b; });
	for (UINT i = 0; i < indicesOfParticlesToRemove.size(); ++i) {
		std::swap(myParticleVertices[anIndex][indicesOfParticlesToRemove[i]], myParticleVertices[anIndex].back());
		myParticlePools[anIndex].push(myParticleVertices[anIndex].back());
		myParticleVertices[anIndex].pop_back();
	}
}