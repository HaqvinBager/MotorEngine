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
	NONE				= 1 << 0,
	PLAYER				= 1 << 1,
	ENEMY				= 1 << 2,
	BOSS				= 1 << 3,
	PLAYERABILITY1		= 1 << 4,
	PLAYERABILITY2		= 1 << 5,
	PLAYERABILITY3		= 1 << 6,
	PLAYERMELEE			= 1 << 7,
	PLAYERHEAVYMELEE	= 1 << 8,
	ENEMYABILITY		= 1 << 9,
	BOSSABILITY1		= 1 << 10,
	BOSSABILITY2		= 1 << 11,
	BOSSABILITY3		= 1 << 12,
	WALL				= 1 << 13,
	FLOOR				= 1 << 14,
	EVENT				= 1 << 15,
	ALL					= 1 << 16
};

class CCollisionManager
{
public:
	CCollisionManager();
	~CCollisionManager();

	void RegisterCollider(CCollider* aCollider);
	void Update();

private:
	std::vector<CCollider*> myColliders;

public:
	static CCollisionManager* GetInstance();

private:
	static CCollisionManager* ourInstance;

};