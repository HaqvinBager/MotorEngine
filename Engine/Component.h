#pragma once
///M�l: G�ra s� cache  v�nligt som m�jligt
///		Alpha version: Att det funkar. KISS.
///		
/// Vi g�r:
///		En kub som �r spelaren
///			Kunna styras med en komponent
///			Ha en komponent som renderas (En CModelInstance)
/// 
/// Vi har 2h: Start: 08/10 - 15:30
/// 
/// GameObject
///		H�ller Component: Componenets definierar ett GameObject
///		
///		GameObject klass
///			Har en Awake
///			Har en Start
///			Har alltid en transform	komponent
///			Har en update
///				Uppdaterar alla sina komponenter 
///			Har en GetComponent function som �r Template typ
///			Kan l�gga till en komponent
///			Kan ta bort en komponent
///			
///		Component klass
///			Har en Awake
///			Har en Start
///			Har en GetComponent 
///			Har en GetParent
///			Kan sl�s p� och av
///			Har en update
///
///		Alla kompononenter kommer finnas p� en plats. �r m�let
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
