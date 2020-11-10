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

#include <typeinfo>
#include <vector>
#include <memory>

class CTransformComponent;
class CComponent;
class CBehaviour;

class CGameObject
{
public:
	CGameObject();
	~CGameObject();

	// Inits components. Remember to add CComponents.
	void Awake();
	// Runs on first frame of the game.
	void Start();

	void Update();

	void Collided(CGameObject& aCollidedGameObject);

	void SetActive(bool aActive);
	bool SetActive() const { return myIsEnabled; };

	template< class T, typename... Args >
	T* AddComponent(Args&&... aParams);

	// Removes Component of type. Use typeid( CComponent )
	template<class T>
	void RemoveComponent();

	template<class T>
	T* GetComponent() const;

	CTransformComponent* myTransform;

public:
	bool Enabled() const { return myIsEnabled; }
	void Enabled(bool anIsEnabled) { myIsEnabled = anIsEnabled; }

private:
	std::vector<CComponent*> myComponents;
	bool myIsEnabled;
};

template<class T, typename... Args >
T* CGameObject::AddComponent(Args&&... aParams)
{
	myComponents.emplace_back(std::move(new T(std::forward<Args>(aParams)...)));
	//myComponents.back()->myParent = *this;
	return dynamic_cast<T*>(myComponents.back());
}

template<class T>
inline void CGameObject::RemoveComponent()
{
	const std::type_info& type = typeid(T);
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		if (type == typeid(*myComponents[i]))
		{
			std::swap(myComponents[i], myComponents[myComponents.size() - 1]);
			delete myComponents[myComponents.size() - 1];
			myComponents[myComponents.size() - 1] = nullptr;
			myComponents.pop_back();
		}
	}
}

template<class T>
inline T* CGameObject::GetComponent() const
{
	const std::type_info& type = typeid(T);
	for (size_t i = 0; i < myComponents.size(); ++i)
	{
		if (type == typeid(*myComponents[i]))
		{
			return dynamic_cast<T*>(myComponents[i]);
		}
	}
	//throw std::exception("Component is missing.");
	return nullptr;
}
