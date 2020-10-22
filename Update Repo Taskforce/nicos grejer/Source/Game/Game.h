#pragma once
#include <vector>

class CModelInstance;
class CVFXInstance;

class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Update(const float dt);

private:
	std::vector<CModelInstance*> myInstances;
	std::vector<CVFXInstance*> myVFX;
};

