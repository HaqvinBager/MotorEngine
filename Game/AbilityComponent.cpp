#include "stdafx.h"
#include "AbilityComponent.h"
#include "GameObject.h"
#include "VFXComponent.h"
#include "VFXFactory.h"
#include "ParticleFactory.h"
#include "ParticleEmitterComponent.h"
#include "AbilityBehaviorComponent.h"
#include "TriangleColliderComponent.h"
#include "RectangleColliderComponent.h"
#include "CircleColliderComponent.h"
#include "ProjectileBehavior.h"
#include "AuraBehavior.h"
#include "BoomerangBehavior.h"
#include "MeleeAttackBehavior.h"
#include "FireConeBehavior.h"
#include "SpeedExplodeBehavior.h"
#include "DelayedExplosionBehavior.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "InputMapper.h"
#include "MainSingleton.h"
#include "Engine.h"
#include "EngineException.h"
#include "StatsComponent.h"
#include "AnimationComponent.h"
#include <fstream>
#include "rapidjson\document.h"
#include "rapidjson\istreamwrapper.h"

using namespace rapidjson;

CAbilityComponent::CAbilityComponent(CGameObject& aParent, std::vector<std::pair<EAbilityType, unsigned int>> someAbilities)
	: CBehaviour(aParent), myAbilityPoolDescriptions(someAbilities), myCurrentCooldowns(new float[static_cast<int>(EAbilityType::Count)]), myMaxCooldowns(new float[static_cast<int>(EAbilityType::Count)]), myMeleeAttackRange(0.0)
{
	myCurrentCooldowns[0] = 0.0f;
	myCurrentCooldowns[1] = 0.0f;
	myCurrentCooldowns[2] = 0.0f;
	myCurrentCooldowns[3] = 0.0f;
	myCurrentCooldowns[4] = 0.0f;

	std::ifstream inputStream("Json/AbilityPaths.json");
	ENGINE_BOOL_POPUP(inputStream.good(), "Ability json paths could not be found! Looking for Json/AbilityPaths.json");
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);

	myFilePaths.emplace(EAbilityType::PlayerAbility1, document["Player Ability 1"].GetString());
	myFilePaths.emplace(EAbilityType::PlayerAbility2, document["Player Ability 2"].GetString());
	myFilePaths.emplace(EAbilityType::PlayerAbility3, document["Player Ability 3"].GetString());
	myFilePaths.emplace(EAbilityType::PlayerMelee, document["Player Ability 4"].GetString());
	myFilePaths.emplace(EAbilityType::PlayerHeavyMelee, document["Player Ability 5"].GetString());

	myFilePaths.emplace(EAbilityType::EnemyAbility, document["Enemy Ability"].GetString());
	myFilePaths.emplace(EAbilityType::BossAbility1, document["Boss Ability 1"].GetString());
	myFilePaths.emplace(EAbilityType::BossAbility2, document["Boss Ability 2"].GetString());
	myFilePaths.emplace(EAbilityType::BossAbility3, document["Boss Ability 3"].GetString());

	myFilePaths.emplace(EAbilityType::AbilityTest, document["Ability Test"].GetString());
}

CAbilityComponent::~CAbilityComponent()
{
	delete[] myCurrentCooldowns;
	myCurrentCooldowns = nullptr;

	delete[] myMaxCooldowns;
	myMaxCooldowns = nullptr;

	if (myEnabled) {
		CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::Ability1, this);
		CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::Ability2, this);
		CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::Ability3, this);
		CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::AttackClick, this);
	}
}

void CAbilityComponent::Awake()
{
	// Setting up pools
	for (unsigned int i = 0; i < myAbilityPoolDescriptions.size(); ++i) {
		std::vector<CGameObject*> gameObjectsToPool;
		for (unsigned int j = 0; j < myAbilityPoolDescriptions[i].second; ++j) {
			gameObjectsToPool.emplace_back(LoadAbilityFromFile(myAbilityPoolDescriptions[i].first));
			CEngine::GetInstance()->GetActiveScene().AddInstance(gameObjectsToPool.back());
		}
		myAbilityPools.emplace(myAbilityPoolDescriptions[i].first, gameObjectsToPool);
	}
}

void CAbilityComponent::Start()
{
}

void CAbilityComponent::Update()
{
	SendEvent();

	for (unsigned int i = 0; i < myActiveAbilities.size(); ++i) {
		if (!myActiveAbilities[i]->Active()) {
			std::swap(myActiveAbilities[i], myActiveAbilities.back());
			myAbilityPools.at(myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->GetAbilityType()).emplace_back(std::move(myActiveAbilities.back()));
			myActiveAbilities.pop_back();
		}
	}
}

void CAbilityComponent::OnEnable()
{
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability1, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability2, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::Ability3, this);
	CInputMapper::GetInstance()->AddObserver(IInputObserver::EInputEvent::AttackClick, this);
}

void CAbilityComponent::OnDisable()
{
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::Ability1, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::Ability2, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::Ability3, this);
	CInputMapper::GetInstance()->RemoveObserver(IInputObserver::EInputEvent::AttackClick, this);
}

bool CAbilityComponent::UseAbility(EAbilityType anAbilityType, DirectX::SimpleMath::Vector3 aSpawnPosition)
{
	if (myAbilityPools.find(anAbilityType) == myAbilityPools.end()) {
		return false;
	}

	if (myAbilityPools.at(anAbilityType).empty()) {
		return false;
	}

	if (myAbilityPools.at(anAbilityType).back()->GetComponent<CAbilityBehaviorComponent>()->AbilityBehavior()->myResourceCost > GameObject().GetComponent<CStatsComponent>()->GetStats().myResource)
		return false;

	myActiveAbilities.emplace_back(myAbilityPools.at(anAbilityType).back());
	myAbilityPools.at(anAbilityType).pop_back();
	myActiveAbilities.back()->Active(true);
	myActiveAbilities.back()->myTransform->Position(aSpawnPosition);
	myActiveAbilities.back()->GetComponent<CAbilityBehaviorComponent>()->Init(&GameObject());

	return true;
}

void CAbilityComponent::SendEvent() {
	//ebin codez
	if (myCurrentCooldowns[0] > 0) {
		SMessage myMessage;
		myCurrentCooldowns[0] -= CTimer::Dt();
		float messageValue = myCurrentCooldowns[0] / myMaxCooldowns[0];
		myMessage.myMessageType = EMessageType::AbilityOneCooldown;
		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
	}

	if (myCurrentCooldowns[1] > 0) {
		SMessage myMessage;
		myCurrentCooldowns[1] -= CTimer::Dt();

		float messageValue = myCurrentCooldowns[1] / myMaxCooldowns[1];
		myMessage.myMessageType = EMessageType::AbilityTwoCooldown;

		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);

	}
	if (myCurrentCooldowns[2] > 0) {
		SMessage myMessage;
		myCurrentCooldowns[2] -= CTimer::Dt();

		std::cout << "Current cooldown: " << myCurrentCooldowns[2] << std::endl;
		std::cout << "Max cooldown: " << myMaxCooldowns[2] << std::endl;

		float messageValue = myCurrentCooldowns[2] / myMaxCooldowns[2];
		myMessage.myMessageType = EMessageType::AbilityThreeCooldown;

		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);
	}
}

void CAbilityComponent::ReceiveEvent(const EInputEvent aEvent)
{
	float messageValue = 1.0f;

	switch (aEvent)
	{
		SMessage myMessage;
	case EInputEvent::Ability1:
		if (this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel > 0) {
			if (myCurrentCooldowns[0] > 0)
				break;

			if (UseAbility(EAbilityType::PlayerAbility1, GameObject().myTransform->Position()))
			{
				myMessage.myMessageType = EMessageType::AbilityOneCooldown;
				myCurrentCooldowns[0] = myMaxCooldowns[0];
				myMessage.data = &messageValue;
				CMainSingleton::PostMaster().Send(myMessage);
			}
		}
		break;
		//this is for group 3
		//TODO: Comment this out before last build
	case EInputEvent::Ability2:
		if (this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel > 1) {
			if (myCurrentCooldowns[1] > 0)
				break;

			if (UseAbility(EAbilityType::PlayerAbility2, GameObject().myTransform->Position()))
			{
				myMessage.myMessageType = EMessageType::AbilityTwoCooldown;
				myCurrentCooldowns[1] = myMaxCooldowns[1];
				myMessage.data = &messageValue;
				CMainSingleton::PostMaster().Send(myMessage);
			}
		}
		break;
	case EInputEvent::Ability3:
		if (this->GameObject().GetComponent<CStatsComponent>()->GetStats().myLevel > 2) {
			if (myCurrentCooldowns[2] > 0)
				break;

			if (UseAbility(EAbilityType::PlayerAbility3, GameObject().myTransform->Position()))
			{
				myMessage.myMessageType = EMessageType::AbilityThreeCooldown;
				myCurrentCooldowns[2] = myMaxCooldowns[2];
				myMessage.data = &messageValue;
				CMainSingleton::PostMaster().Send(myMessage);
			}
		}
		break;
	//case EInputEvent::MoveClick:
	//	if (myCurrentCooldowns[3] > 0)
	//		break;

	//	if (UseAbility(EAbilityType::PlayerMelee, GameObject().myTransform->Position()))
	//	{
	//	}
	//	/*myMessage.myMessageType = EMessageType::AbilityThreeCooldown;
	//	myCurrentCooldowns[2] = myMaxCooldowns[2];
	//	myMessage.data = &messageValue;
	//	CMainSingleton::PostMaster().Send(myMessage);*/
	//	break;
	case EInputEvent::AttackClick:
		if (myCurrentCooldowns[4] > 0)
			break;

		if (UseAbility(EAbilityType::PlayerHeavyMelee, GameObject().myTransform->Position()))
		{
			this->GameObject().GetComponent<CAnimationComponent>()->PlayAttack02ID();
		}
		/*myMessage.myMessageType = EMessageType::AbilityThreeCooldown;
		myCurrentCooldowns[2] = myMaxCooldowns[2];
		myMessage.data = &messageValue;
		CMainSingleton::PostMaster().Send(myMessage);*/
		break;
	default:
		break;
	}
}

const float CAbilityComponent::MeleeAttackRange() const
{
	return myMeleeAttackRange;
}

void CAbilityComponent::ResetCooldown(int anIndex)
{
	SMessage message;
	if (anIndex == 1) {
		message.myMessageType = EMessageType::AbilityOneCooldown;
		myCurrentCooldowns[anIndex - 1] = 0.0;
		message.data = &myCurrentCooldowns[anIndex - 1];
		CMainSingleton::PostMaster().Send(message);
	} else if (anIndex == 2) {
		message.myMessageType = EMessageType::AbilityTwoCooldown;
		myCurrentCooldowns[anIndex - 1] = 0.0;
		message.data = &myCurrentCooldowns[anIndex - 1];
		CMainSingleton::PostMaster().Send(message);
	} else if (anIndex == 3) {
		message.myMessageType = EMessageType::AbilityThreeCooldown;
		myCurrentCooldowns[anIndex - 1] = 0.0;
		message.data = &myCurrentCooldowns[anIndex - 1];
		CMainSingleton::PostMaster().Send(message);
	}
}

CGameObject* CAbilityComponent::LoadAbilityFromFile(EAbilityType anAbilityType)
{
	std::ifstream inputStream(myFilePaths[anAbilityType]);
	if (!inputStream.good()) {
		return nullptr;
	}
	IStreamWrapper inputWrapper(inputStream);
	Document document;
	document.ParseStream(inputWrapper);

	CGameObject* abilityObject = new CGameObject();
	std::cout << "LoadAbilityFromFile: " << *&abilityObject << std::endl;
	CProjectileBehavior* projectileBehavior = nullptr;
	CAuraBehavior* auraBehavior = nullptr;
	CBoomerangBehavior* boomerangBehavior = nullptr;
	CMeleeAttackBehavior* meleeAttackBehavior = nullptr;
	CFireConeBehavior* fireConeBehavior = nullptr;
	CSpeedExplodeBehavior* speedExplodeBehavior = nullptr;
	CDelayedExplosionBehavior* delayedExplosionBehavior = nullptr;
	std::string colliderType;

	//COOLDOWNS
	int cdIdx = static_cast<int>(anAbilityType);
	myMaxCooldowns[cdIdx] = document.HasMember("Cooldown") ? document["Cooldown"].GetFloat() : 0.5f;
	//!COOLDOWNS

	//VFX
	abilityObject->myTransform->Position({0.0f, 0.0f, 0.0f});
	std::vector<std::string> paths;
	for (unsigned int i = 0; i < document["VFX"].Size(); ++i) {
		paths.emplace_back(document["VFX"][i]["Path"].GetString());
	}
	abilityObject->AddComponent<CVFXComponent>(*abilityObject);
	abilityObject->GetComponent<CVFXComponent>()->Init(CVFXFactory::GetInstance()->GetVFXBaseSet(paths));
	//!VFX

	//PARTICLESYSTEM
	paths.clear();
	for (unsigned int i = 0; i < document["ParticleSystems"].Size(); ++i) {
		paths.emplace_back(document["ParticleSystems"][i]["Path"].GetString());
	}
	abilityObject->AddComponent<CParticleEmitterComponent>(*abilityObject);
	abilityObject->GetComponent<CParticleEmitterComponent>()->Init(CParticleFactory::GetInstance()->GetParticleSet(paths));
	//!PARTICLESYSTEM

	//BEHAVIOR
	auto behavior = document["Behavior"].GetObjectW();
	if (behavior["Type"].GetString() == std::string("Aura"))
	{
		auraBehavior = new CAuraBehavior(&GameObject(), behavior["Rotational Speed"].GetFloat());
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, auraBehavior, anAbilityType);
	} else if (behavior["Type"].GetString() == std::string("Projectile"))
	{
		projectileBehavior = new CProjectileBehavior(behavior["Speed"].GetFloat(), behavior["Duration"].GetFloat(), document["Damage"].GetFloat());
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, projectileBehavior, anAbilityType);
	} else if (behavior["Type"].GetString() == std::string("Boomerang"))
	{
		boomerangBehavior = new CBoomerangBehavior(behavior["Speed"].GetFloat(), behavior["Duration"].GetFloat(), behavior["ResourceCost"].GetFloat(), behavior["RotationalSpeed"].GetFloat(), document["Damage"].GetFloat());
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, boomerangBehavior, anAbilityType);
	} else if (behavior["Type"].GetString() == std::string("MeleeAttack"))
	{
		meleeAttackBehavior = new CMeleeAttackBehavior(behavior["Duration"].GetFloat(), document["Damage"].GetFloat(), abilityObject);
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, meleeAttackBehavior, anAbilityType);

		myMeleeAttackRange = document["Collider"]["Height"].GetFloat();
	} else if (behavior["Type"].GetString() == std::string("FireCone"))
	{
		fireConeBehavior = new CFireConeBehavior(behavior["Duration"].GetFloat(), behavior["ResourceCost"].GetFloat(), document["Damage"].GetFloat(), abilityObject);
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, fireConeBehavior, anAbilityType);
	} else if (behavior["Type"].GetString() == std::string("SpeedExplode"))
	{
		speedExplodeBehavior = new CSpeedExplodeBehavior(behavior["Duration"].GetFloat(), behavior["ResourceCost"].GetFloat(), behavior["ExplodeAfter"].GetFloat(), behavior["SpeedMultiplier"].GetFloat(), document["Damage"].GetFloat(), abilityObject);
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, speedExplodeBehavior, anAbilityType);
	} else if (behavior["Type"].GetString() == std::string("DelayedExplosion"))
	{
		delayedExplosionBehavior = new CDelayedExplosionBehavior(behavior["Duration"].GetFloat(), behavior["Delay"].GetFloat(), behavior["ResourceCost"].GetFloat(), document["Damage"].GetFloat(), abilityObject);
		abilityObject->AddComponent<CAbilityBehaviorComponent>(*abilityObject, delayedExplosionBehavior, anAbilityType);
	}
	//!BEHAVIOR

	//COLLIDER
	colliderType = std::string(document["Collider"]["Type"].GetString());
	if (colliderType == "Circle") {
		abilityObject->AddComponent<CCircleColliderComponent>
			(
				*abilityObject,
				document["Collider"]["Width"].GetFloat(),
				static_cast<ECollisionLayer>(document["Collider"]["Collision Layer"].GetInt()),
				document["Collider"]["Collides With"].GetInt()
				);
	} else if (colliderType == "Rectangle") {
		abilityObject->AddComponent<CRectangleColliderComponent>
			(
				*abilityObject,
				document["Collider"]["Width"].GetFloat(),
				document["Collider"]["Height"].GetFloat(),
				static_cast<ECollisionLayer>(document["Collider"]["Collision Layer"].GetInt()),
				document["Collider"]["Collides With"].GetInt()
				);
	} else if (colliderType == "Triangle") {
		abilityObject->AddComponent<CTriangleColliderComponent>
			(
				*abilityObject,
				document["Collider"]["Width"].GetFloat(),
				document["Collider"]["Height"].GetFloat(),
				static_cast<ECollisionLayer>(document["Collider"]["Collision Layer"].GetInt()),
				document["Collider"]["Collides With"].GetInt()
				);
	}
	//!COLLIDER

	abilityObject->Active(false);
	//CEngine::GetInstance()->GetActiveScene().AddInstance(abilityObject);

	return abilityObject;
}
