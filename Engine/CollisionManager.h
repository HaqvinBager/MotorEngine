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
class CTriangleColliderComponent;
class CCircleColliderComponent;
class CRectangleColliderComponent;
class CCollider;
enum class ECollisionLayer {
	PLAYER,
	ENEMY,
	BOSS,
	PLAYERABILITY,
	ENEMYABILITY,
	BOSSABILITY,
	WALL,
	FLOOR,
	ALL,
	NONE
};

class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

	//void RegisterCollider(CCapsuleColliderComponent* aCollider);
	//void RegisterCollider(CTriangleColliderComponent* aCollider);
	//void RegisterCollider(CCircleColliderComponent* aCollider);
	//void RegisterCollider(CRectangleColliderComponent* aCollider);
	void RegisterCollider(CCollider* aCollider);
	void Update();

private:
	std::vector<CCollider*> myColliders;
	//std::vector<CCapsuleColliderComponent*> myCapsuleColliders;
	//std::vector<CTriangleColliderComponent*> myTriangleColliders;
	//std::vector<CCircleColliderComponent*> myCircleColliders;
	//std::vector<CRectangleColliderComponent*> myRectangleColliders;

public:
	static CCollisionManager* GetInstance();

private:
	static CCollisionManager* ourInstance;

};