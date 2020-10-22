#pragma once

class CEnvironmentLight;
class CPointLight;
class CEngine;

class CLightFactory
{
	friend class CEngine;
public:
	bool Init(CEngine& anEngine);

	CEnvironmentLight* CreateEnvironmentLight(std::string aFilePath);
	CPointLight* CreatePointLight();

	static CLightFactory* GetInstance();
private:
	CLightFactory();
	~CLightFactory();
private:
	static CLightFactory* ourInstance;
	CEngine* myEngine;
};

