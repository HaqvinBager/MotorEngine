#pragma once

namespace DirectX {
	namespace SimpleMath {
		struct Vector3;
	}
}
class CLevelLoader;
class CModelInstance;
class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Update();

private:

	void UpdateCamera();

	CLevelLoader* myLevelLoader;
};

