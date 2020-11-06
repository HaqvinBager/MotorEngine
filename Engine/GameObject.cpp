#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "TransformComponent.h"
#include <iostream>

CGameObject::CGameObject()
{
	myTransform = new CTransformComponent(*this);
	myComponents.emplace_back(myTransform);
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
}

void CGameObject::Start()
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		if (myComponents[i]->Enabled())
		{
			myComponents[i]->Start();
		}
	}
}

void CGameObject::Update()
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		if (myComponents[i]->Enabled())
		{
			myComponents[i]->Update();
		}
	}
}

void CGameObject::Collided(CGameObject& aCollidedGameObject)
{
	//std::cout << "[" << &(aCollidedGameObject)  << "] Collided W Me: [" << this << "]"<< std::endl;

	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		if (myComponents[i]->Enabled())
		{
			myComponents[i]->Collided(&aCollidedGameObject);
		}
	}
}
