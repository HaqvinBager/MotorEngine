#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "TransformComponent.h"
#include "Behaviour.h"
#include <iostream>

//CGameObject::CGameObject()
//{
//	myTransform = new CTransformComponent(*this);
//	myComponents.emplace_back(myTransform);
//	myIsActive = true;
//}

CGameObject::CGameObject(const int aInstanceID) : myInstanceID(aInstanceID)
{

	myTransform = new CTransformComponent(*this);
	myComponents.emplace_back(myTransform);
	myIsActive = true;
}

CGameObject::~CGameObject()
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		delete myComponents[i];
		myComponents[i] = nullptr;
	}
	myComponents.clear();
}

void CGameObject::Awake()
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		myComponents[i]->Awake();
	}

	for (size_t i = 0; i < myComponents.size(); ++i) {
		myComponents[i]->OnEnable();
	}
}

void CGameObject::Start()
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		if (CBehaviour* behaviour = dynamic_cast<CBehaviour*>(myComponents[i]))
		{
			if (behaviour->Enabled()) {
				myComponents[i]->Start();
			}
		}
		else {
			myComponents[i]->Start();
		}
	}
}

void CGameObject::Update()
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		if (CBehaviour* behaviour = dynamic_cast<CBehaviour*>(myComponents[i]))
		{
			if (behaviour->Enabled()) {
				myComponents[i]->Update();
			}
		}
		else {
			myComponents[i]->Update();
		}
	}
}

void CGameObject::Collided(CGameObject& aCollidedGameObject)
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		if (CBehaviour* behaviour = dynamic_cast<CBehaviour*>(myComponents[i]))
		{
			if (behaviour->Enabled()) {
				myComponents[i]->Collided(&aCollidedGameObject);
			}
		} else {
			myComponents[i]->Collided(&aCollidedGameObject);
		}
	}
}

void CGameObject::Active(bool aActive)
{
	myIsActive = aActive;

	if (aActive) {
		for (size_t i = 0; i < myComponents.size(); ++i) {
			myComponents[i]->OnEnable();
		}
	}
	else {
		for (size_t i = 0; i < myComponents.size(); ++i) {
			myComponents[i]->OnDisable();
		}
	}
}
