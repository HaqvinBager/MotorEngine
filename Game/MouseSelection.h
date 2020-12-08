#pragma once
class CMouseSelection
{
public:
	CMouseSelection();
	~CMouseSelection();

	DirectX::SimpleMath::Vector3 GetPositionAtNavmesh();

	void FindSelectedEnemy();
private:
};

