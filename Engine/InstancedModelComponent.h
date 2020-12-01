#pragma once
#include "Behaviour.h"

class CGameObject;
class CModel;

class CInstancedModelComponent : public CBehaviour
{
public:
	CInstancedModelComponent(CGameObject& aParent, std::string aModelPath, int aCount, std::vector<DirectX::SimpleMath::Matrix> aInstancedTransforms);
	~CInstancedModelComponent();

	void Awake() override;
	void Start() override;
	void Update() override;

	void OnEnable() override;
	void OnDisable() override;

	CModel* GetModel() const { return myModel; }
	const std::vector<DirectX::SimpleMath::Matrix>& GetInstancedTransforms() const { return myIntancedTransforms; }

private:
	std::vector<DirectX::SimpleMath::Matrix> myIntancedTransforms;
	CModel* myModel;
};

