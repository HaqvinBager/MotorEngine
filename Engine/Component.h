#pragma once
///Mål: Göra så cache  vänligt som möjligt
///		Alpha version: Att det funkar. KISS.
///		
/// Vi gör:
///		En kub som är spelaren
///			Kunna styras med en komponent
///			Ha en komponent som renderas (En CModelInstance)
/// 
/// Vi har 2h: Start: 08/10 - 15:30
/// 
/// GameObject
///		Håller Component: Componenets definierar ett GameObject
///		
///		GameObject klass
///			Har en Awake
///			Har en Start
///			Har alltid en transform	komponent
///			Har en update
///				Uppdaterar alla sina komponenter 
///			Har en GetComponent function som är Template typ
///			Kan lägga till en komponent
///			Kan ta bort en komponent
///			
///		Component klass
///			Har en Awake
///			Har en Start
///			Har en GetComponent 
///			Har en GetParent
///			Kan slås på och av
///			Har en update
///
///		Alla kompononenter kommer finnas på en plats. Är målet
/// 

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

	CGameObject& GetParent() const;

protected:
	bool myEnabled;
private:
	CGameObject& myParent;
};

template<class T>
inline T* CComponent::GetComponent()
{
	return myParent.GetComponent<T>();
}
