#pragma once
#include "Behaviour.h"
#include "NavmeshLoader.h"

struct SNavMesh;
class CGameObject;

class CNavMeshComponent : public CBehaviour
{
public:
	CNavMeshComponent(CGameObject& aParent);
	~CNavMeshComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	// For enemy pathfinding? Correct men kan vara annat aswell //Alexander
	/*std::vector<DirectX::SimpleMath::Vector3>*/void CalculatePath(DirectX::SimpleMath::Vector3 aDestination);
	void CalculatePath();

private:
	STriangle* ReturnClosestTriangle(const DirectX::SimpleMath::Vector3& aStartPosition);
	void ResolveStuck(STriangle* aStartTriangle, const DirectX::SimpleMath::Vector3& aStartPosition, const DirectX::SimpleMath::Vector3& aFinalPosition);

private:
	SNavMesh* myNavMesh;
};

