#pragma once

class CGameObject;

class CEnemyFactory
{

	friend class CEngine;

public:
	static CEnemyFactory* GetInstance();

	CGameObject& CreateEnemy(const int aInstanceID, DirectX::SimpleMath::Vector3 aPosition, float aHealth, float aDamage, float aMoveSpeed, float aCooldown) const;
private:
	CEnemyFactory();
	~CEnemyFactory();

private:
	static CEnemyFactory* ourInstance;
};

