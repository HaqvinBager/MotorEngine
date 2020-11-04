#pragma once
//Ta hand om att skapa Colliders
//Registrera Colliders som blir skapade av AddComponent

//Kunna jämföra colliders om dom träffar varandra
//Ta emot en Ray och returnera vilket objekt eller vart rayen träffar första objektet.

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
		struct Vector2;
	}
}

class CCapsuleColliderComponent;
//class CTriangleColliderComponent;

class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

	void RegisterCollider(CCapsuleColliderComponent* aCollider);
	//void RegisterCollider(CTriangleColliderComponent* aCollider);
	void Update();

private:
	std::vector<CCapsuleColliderComponent*> myCapsuleColliders;
	//std::vector<CTriangleColliderComponent*> myTriangleColliders;

public:
	static CCollisionManager* GetInstance();

private:
	static CCollisionManager* ourInstance;

};