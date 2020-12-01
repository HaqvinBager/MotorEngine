#include "stdafx.h"
#include "ParticleInstance.h"
#include <algorithm>

#include "RandomNumberGenerator.h"
#include "DebugTimerUtility.h"

namespace SM = DirectX::SimpleMath;

CParticleInstance::CParticleInstance() : myParticle(nullptr), myEmitterTimer(0.0f), myStartDelay(0.0f)
{
}

CParticleInstance::~CParticleInstance()
{
	myParticle = nullptr;
}

void CParticleInstance::Init(CParticle* aParticle)
{
	myParticle = aParticle;
	myParticleVertices.reserve(aParticle->GetParticleData().myNumberOfParticles);
	for (unsigned int i = 0; i < aParticle->GetParticleData().myNumberOfParticles; ++i) {
		myParticlePool.push(CParticle::SParticleVertex());
	}
}

void CParticleInstance::SetTransform(DirectX::SimpleMath::Vector3 aPosition, DirectX::SimpleMath::Vector3 aRotation)
{
	SetRotation(aRotation);
	SetPosition(aPosition);
}

void CParticleInstance::SetRotation(DirectX::SimpleMath::Vector3 aRotation)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform = SM::Matrix::CreateFromYawPitchRoll(
		DirectX::XMConvertToRadians(aRotation.y),
		DirectX::XMConvertToRadians(aRotation.x),
		DirectX::XMConvertToRadians(aRotation.z)
	);
	myTransform.Translation(translation);
}

void CParticleInstance::SetPosition(DirectX::SimpleMath::Vector3 aPosition)
{
	myTransform.Translation(aPosition);
}

void CParticleInstance::Move(DirectX::SimpleMath::Vector3 aMovement)
{
	myTransform.Translation(myTransform.Translation() + myTransform.Right() * aMovement.x);
	myTransform.Translation(myTransform.Translation() + myTransform.Up() * aMovement.y);
	myTransform.Translation(myTransform.Translation() - myTransform.Forward() * aMovement.z);
}

void CParticleInstance::Rotate(DirectX::SimpleMath::Vector3 aRotation)
{
	SM::Vector3 translation = myTransform.Translation();
	myTransform *= SM::Matrix::CreateFromYawPitchRoll(aRotation.y, aRotation.x, aRotation.z);
	myTransform.Translation(translation);
}

void CParticleInstance::Update(float aDeltaTime, DirectX::SimpleMath::Vector3 aCameraPosition)
{
	CParticle::SParticleData particleData = myParticle->GetParticleData();

	SpawnParticles(aDeltaTime, aCameraPosition, particleData);
	//LogTime<&CParticleInstance::UpdateParticles>(this, aDeltaTime, aCameraPosition, particleData);
	UpdateParticles(aDeltaTime, aCameraPosition, particleData);

	std::sort(myParticleVertices.begin(), myParticleVertices.end(),
		[](const CParticle::SParticleVertex& aFirstParticle, const CParticle::SParticleVertex& aSecondParticle)
		{
			return aFirstParticle.mySquaredDistanceToCamera > aSecondParticle.mySquaredDistanceToCamera;
		}
	);
}

void CParticleInstance::SpawnParticles(float aDeltaTime, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& someParticleData)
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

void CParticleInstance::UpdateParticles(float aDeltaTime, DirectX::SimpleMath::Vector3& aCameraPosition, CParticle::SParticleData& particleData)
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

	std::sort(indicesOfParticlesToRemove.begin(), indicesOfParticlesToRemove.end(), [](UINT a, UINT b){ return a > b; } );
	for (UINT i = 0; i < indicesOfParticlesToRemove.size(); ++i) {
		std::swap(myParticleVertices[indicesOfParticlesToRemove[i]], myParticleVertices.back());
		myParticlePool.push(myParticleVertices.back());
		myParticleVertices.pop_back();
	}

	//for (auto it = myParticleVertices.begin(); it != myParticleVertices.end();)
	//{
	//	size_t i = std::distance(myParticleVertices.begin(), it);

	//	float quotient = myParticleVertices[i].myLifeTime / particleData.myParticleLifetime;

	//	myParticleVertices[i].myColor = SM::Vector4::Lerp
	//	(
	//		particleData.myParticleEndColor,
	//		particleData.myParticleStartColor,
	//		quotient
	//	);

	//	myParticleVertices[i].mySize = SM::Vector2::Lerp
	//	(
	//		{ particleData.myParticleEndSize, particleData.myParticleEndSize },
	//		{ particleData.myParticleStartSize, particleData.myParticleStartSize },
	//		quotient
	//	);

	//	myParticleVertices[i].myMovement = SM::Vector4::Lerp
	//	(
	//		particleData.myParticleEndDirection,
	//		particleData.myParticleStartDirection,
	//		quotient
	//	);

	//	SM::Vector4 direction = SM::Vector4::Lerp
	//	(
	//		myParticleVertices[i].myEndMovement,
	//		myParticleVertices[i].myStartMovement,
	//		quotient
	//	);

	//	myParticleVertices[i].mySquaredDistanceToCamera = SM::Vector3::DistanceSquared
	//	(
	//		{ myParticleVertices[i].myPosition.x,
	//		  myParticleVertices[i].myPosition.y,
	//		  myParticleVertices[i].myPosition.z },
	//		aCameraPosition
	//	);

	//	SM::Vector4 newPosition = direction * particleData.myParticleSpeed * aDeltaTime + myParticleVertices[i].myPosition;
	//	myParticleVertices[i].myPosition = newPosition;

	//	if ((myParticleVertices[i].myLifeTime -= aDeltaTime) < 0.0f) {
	//		myParticlePool.push(myParticleVertices[i]);
	//		it = myParticleVertices.erase(it);
	//	}
	//	else {
	//		it++;
	//	}
	//}
}