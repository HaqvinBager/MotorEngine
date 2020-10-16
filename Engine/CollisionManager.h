#pragma once
//Ta hand om att skapa Colliders
//Registrera Colliders som blir skapade av AddComponent

//Kunna j�mf�ra colliders om dom tr�ffar varandra
//Ta emot en Ray och returnera vilket objekt eller vart rayen tr�ffar f�rsta objektet.

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
		struct Vector2;
	}
}

class CCapsuleColliderComponent;

class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

	void RegisterCollider(CCapsuleColliderComponent* aCollider);
	void Update();

private:
	std::vector<CCapsuleColliderComponent*> myCapsuleColliders;

public:
	static CCollisionManager* GetInstance();

private:
	static CCollisionManager* ourInstance;

};