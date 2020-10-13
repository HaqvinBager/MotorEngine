#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"

CGameObject::CGameObject()
{
}

CGameObject::~CGameObject()
{
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
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
