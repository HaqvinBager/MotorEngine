#pragma once

#include "GameObject.h"
class CComponent
{
public:
	CComponent(CGameObject& aParent);
	virtual ~CComponent();

	virtual void Awake() = 0;

	virtual void Start() = 0;

	virtual void Update() = 0;

	virtual void Collided(CGameObject* aCollidedGameObject);

	virtual void OnEnable() {}
	virtual void OnDisable() {}

	template<class T>
	T* GetComponent();

	CGameObject& GameObject() const;

private:
	CGameObject& myParent;
};

template<class T>
inline T* CComponent::GetComponent()
{
	return myParent.GetComponent<T>();
}
