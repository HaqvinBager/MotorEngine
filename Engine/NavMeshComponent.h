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

	// For enemy pathfinding?
	/*std::vector<DirectX::SimpleMath::Vector3>*/void CalculatePath(DirectX::SimpleMath::Vector3 aDestination);
	void CalculatePath();

private:
	SNavMesh* myNavMesh;
};

