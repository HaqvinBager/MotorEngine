#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "Camera.h"
#include "RandomNumberGenerator.h"

namespace SM = DirectX::SimpleMath;
#define ENGINE_SCALE 0.01f

CParticleEmitterComponent::CParticleEmitterComponent(CGameObject& aParent) : CComponent(aParent), myParticle(nullptr), myEmitterTimer(0.0f)
{
	SetScale(1.0f);
	myTransform.Translation(GetParent().myTransform->Position());
}

CParticleEmitterComponent::~CParticleEmitterComponent()
{
	myParticle = nullptr;
}

void CParticleEmitterComponent::Awake()
{
}

void CParticleEmitterComponent::Start()
{
}

void CParticleEmitterComponent::Update()
{
	SetPosition(GetParent().myTransform->Position());
	Update(CTimer::Dt(), CScene::GetInstance()->GetMainCamera()->GetTransform().Translation());
}

void CParticleEmitterComponent::Init(CParticle* aParticle)
{
	myParticle = aParticle;
	myParticleVertices.reserve(aParticle->GetParticleData().myNumberOfParticles);
	for (unsigned int i = 0; i < aParticle->GetParticleData().myNumberOfParticles; ++i) {
		myParticlePool.push(CParticle::SParticleVertex());
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

void CParticleEmitterComponent::Update(float aDeltaTime, DirectX::SimpleMath::Vector3 aCameraPosition)
{
	CParticle::SParticleData particleData = myParticle->GetParticleData();

	SpawnParticles(aDeltaTime, aCameraPosition, particleData);
	UpdateParticles(aDeltaTime, aCameraPosition, particleData);

	std::sort(myParticleVertices.begin(), myParticleVertices.end(),
		[](const CParticle::SParticleVertex& aFirstParticle, const CParticle::SParticleVertex& aSecondParticle)
		{
			return aFirstParticle.mySquaredDistanceToCamera > aSecondParticle.mySquaredDistanceToCamera;
		}
	);
}

void CParticleEmitterComponent::SpawnParticles(float aDeltaTime, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& someParticleData)
{
	myEmitterTimer += aDeltaTime;
	if (myEmitterTimer > (1.0f / someParticleData.mySpawnRate) && (myParticleVertices.size() < someParticleData.myNumberOfParticles)) {
		myParticleVertices.emplace_back(myParticlePool.front());
		myParticlePool.pop();
		myParticleVertices.back().myLifeTime = someParticleData.myParticleLifetime + Random(someParticleData.myLifetimeLowerBound, someParticleData.myLifetimeUpperBound);
		myParticleVertices.back().myPosition = { myTransform._41, myTransform._42, myTransform._43, 1.0f };
		myParticleVertices.back().myMovement = someParticleData.myParticleStartDirection;
		myParticleVertices.back().myStartMovement = someParticleData.myParticleStartDirection + Random(someParticleData.myDirectionLowerBound, someParticleData.myDirectionUpperBound, 0.0f);
		myParticleVertices.back().myEndMovement = someParticleData.myParticleEndDirection + Random(someParticleData.myDirectionLowerBound, someParticleData.myDirectionUpperBound, 0.0f);
		myParticleVertices.back().myColor = someParticleData.myParticleStartColor;
		myParticleVertices.back().mySize = { someParticleData.myParticleStartSize, someParticleData.myParticleStartSize };
		myParticleVertices.back().mySquaredDistanceToCamera = SM::Vector3::DistanceSquared({ myTransform._41, myTransform._42, myTransform._43 }, aCameraPosition);
		myEmitterTimer -= (1.0f / someParticleData.mySpawnRate);
	}
}

void CParticleEmitterComponent::UpdateParticles(float aDeltaTime, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& particleData)
{
	std::vector<unsigned int> indicesOfParticlesToRemove;
	for (UINT i = 0; i < myParticleVertices.size(); ++i)
	{
		float quotient = myParticleVertices[i].myLifeTime / particleData.myParticleLifetime;

		myParticleVertices[i].myColor = SM::Vector4::Lerp
		(
			particleData.myParticleEndColor,
			particleData.myParticleStartColor,
			quotient
		);

		myParticleVertices[i].mySize = SM::Vector2::Lerp
		(
			{ particleData.myParticleEndSize, particleData.myParticleEndSize },
			{ particleData.myParticleStartSize, particleData.myParticleStartSize },
			quotient
		);

		myParticleVertices[i].myMovement = SM::Vector4::Lerp
		(
			particleData.myParticleEndDirection,
			particleData.myParticleStartDirection,
			quotient
		);

		SM::Vector4 direction = SM::Vector4::Lerp
		(
			myParticleVertices[i].myEndMovement,
			myParticleVertices[i].myStartMovement,
			quotient
		);

		myParticleVertices[i].mySquaredDistanceToCamera = SM::Vector3::DistanceSquared
		(
			{ myParticleVertices[i].myPosition.x,
			  myParticleVertices[i].myPosition.y,
			  myParticleVertices[i].myPosition.z },
			aCameraPosition
		);

		SM::Vector4 newPosition = direction * particleData.myParticleSpeed * aDeltaTime + myParticleVertices[i].myPosition;
		myParticleVertices[i].myPosition = newPosition;

		if ((myParticleVertices[i].myLifeTime -= aDeltaTime) < 0.0f) {
			indicesOfParticlesToRemove.emplace_back(i);
		}
	}

	std::sort(indicesOfParticlesToRemove.begin(), indicesOfParticlesToRemove.end(), [](UINT a, UINT b) { return a > b; });
	for (UINT i = 0; i < indicesOfParticlesToRemove.size(); ++i) {
		std::swap(myParticleVertices[indicesOfParticlesToRemove[i]], myParticleVertices.back());
		myParticlePool.push(myParticleVertices.back());
		myParticleVertices.pop_back();
	}
}