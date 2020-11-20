#include "Component.h"

class CCameraComponent;
class CGameObject;
class CCameraControllerComponent : public CComponent
{
public:
	enum class ECameraMode
	{
		Player,
		FreeCam,
	};

public:
	CCameraControllerComponent(CGameObject& gameObject, const float aCameraMoveSpeed = 25.0f/*, ECameraMode aCameraMode = ECameraMode::Player, char aToggleFreeCam = 'f'*/);
	~CCameraControllerComponent() override;

	void Awake() override;
	void Start() override;
	void Update() override;

private:
	//const char myToggleFreeCam;
	//ECameraMode myCameraMode;
	float myCameraMoveSpeed;
	CCameraComponent* myCamera;
};
