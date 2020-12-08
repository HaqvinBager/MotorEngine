#include "stdafx.h"
#include "CollisionManager.h"

#include "IntersectionManager.h"
#include "CapsuleColliderComponent.h"
#include "TriangleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "Collider.h"
#include "TransformComponent.h"
#include <SimpleMath.h>
#include <SpeedExplodeBehavior.h>
#include <DelayedExplosionBehavior.h>
#include <BoomerangBehavior.h>
#include "GameObject.h"
#include <ProjectileBehavior.h>
#include <MeleeAttackBehavior.h>
#include <FireConeBehavior.h>
#include <AbilityBehaviorComponent.h>

using namespace DirectX::SimpleMath;

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::RegisterCollider(CCollider* aCollider)
{
	assert(aCollider != nullptr && "Collider is nullptr");
	myColliders.emplace_back(aCollider);
}

bool CCollisionManager::CheckIfAbility(CCollider* anAbilityCollider, CGameObject* aCollidedWithGameObject)
{
	if (anAbilityCollider->GetCollisionLayer() != ECollisionLayer::PLAYERABILITY) {
		if (anAbilityCollider->GetCollisionLayer() != ECollisionLayer::ENEMYABILITY) {
			if (anAbilityCollider->GetCollisionLayer() != ECollisionLayer::BOSSABILITY) {
				return false;
			}
		}
	}

	CGameObject* abilityGameObject = &anAbilityCollider->GameObject();

	CAbilityBehaviorComponent* abilityBehavior = abilityGameObject->GetComponent<CAbilityBehaviorComponent>();

	IAbilityBehavior* behavior = abilityBehavior->AbilityBehavior();
	if (!abilityBehavior) {
		return false;
	}

	behavior->Collided(aCollidedWithGameObject);
	return true;
}

void CCollisionManager::ClearColliders()
{
    myColliders.clear();
}

void CCollisionManager::Update()
{
	for (unsigned int outer = 0; outer < myColliders.size(); ++outer)
	{
		for (unsigned int inner = outer + 1; inner < myColliders.size(); ++inner)
		{
			CGameObject& outerObj = myColliders[outer]->GameObject();
			CGameObject& innerObj = myColliders[inner]->GameObject();
			bool outerActive = outerObj.Active();
			bool innerActive = innerObj.Active();
			bool outerEnabled = myColliders[outer]->Enabled();
			bool innerEnabled = myColliders[inner]->Enabled();

			uint64_t outerCollisionLayer = static_cast<uint64_t>(myColliders[outer]->GetCollisionLayer());
			uint64_t innerCollisionLayer = static_cast<uint64_t>(myColliders[inner]->GetCollisionFlags());

			if (outerActive && innerActive && outerEnabled && innerEnabled) {
				if (outerCollisionLayer & innerCollisionLayer) {
					if (myColliders[outer]->Collided(myColliders[inner]))
					{
						myColliders[outer]->GameObject().Collided(myColliders[inner]->GameObject());
						myColliders[inner]->GameObject().Collided(myColliders[outer]->GameObject());
						
						if (!CheckIfAbility(myColliders[inner], &myColliders[outer]->GameObject())) {
							CheckIfAbility(myColliders[outer], &myColliders[inner]->GameObject());
						}
					}
				}
			}
		/*	if (myColliders[outer]->GameObject().Active() && myColliders[inner]->GameObject().Active() && myColliders[outer]->Enabled() && myColliders[inner]->Enabled()) {
				if (static_cast<uint64_t>(myColliders[outer]->GetCollisionLayer()) & static_cast<uint64_t>(myColliders[inner]->GetCollisionFlags())) {
					if (myColliders[outer]->Collided(myColliders[inner]))
					{
						myColliders[outer]->GameObject().Collided(myColliders[inner]->GameObject());
						myColliders[inner]->GameObject().Collided(myColliders[outer]->GameObject());

						if (!CheckIfAbility(myColliders[inner], &myColliders[outer]->GameObject())) {
							CheckIfAbility(myColliders[outer], &myColliders[inner]->GameObject());
						}
					}
				}
			}*/
		}
	}
}