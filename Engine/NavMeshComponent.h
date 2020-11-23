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

	std::vector<DirectX::SimpleMath::Vector3> CalculatePath(DirectX::SimpleMath::Vector3 aDestination);
	std::vector<DirectX::SimpleMath::Vector3> CalculatePath(DirectX::SimpleMath::Ray aRay, DirectX::SimpleMath::Vector3& anOutPosition);

private:
	SNavMesh* myNavMesh;
};

