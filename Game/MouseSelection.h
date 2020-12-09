#pragma once
class CMouseSelection
{
public:
	CMouseSelection();
	~CMouseSelection();

	DirectX::SimpleMath::Vector3 GetPositionAtNavmesh();

	CGameObject* FindSelectedEnemy();
	CGameObject* FindSelectedDestructible();
private:
	CGameObject* myEnemy;
};

