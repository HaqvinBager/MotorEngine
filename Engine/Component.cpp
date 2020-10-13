#include "stdafx.h"
#include "Component.h"
#include "GameObject.h"

CComponent::CComponent(CGameObject& aParent)
	: myParent(aParent),
	myEnabled(true)
{}

CComponent::~CComponent()
{
}

CGameObject& CComponent::GetParent() const
{
	return myParent;
}
