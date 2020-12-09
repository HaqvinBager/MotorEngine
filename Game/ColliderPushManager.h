#pragma once
class CColliderPushManager
{
public:
	CColliderPushManager();
	~CColliderPushManager();

	void EnemiesPushOutEnemies();

	void PlayerPushOutEnemies();

	void PlayerPushOutDestructibles();
private:
};

