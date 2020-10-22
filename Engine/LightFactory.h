#pragma once

class CEnvironmentLight;
class CEngine;
class CPointLight;

class CLightFactory
{
public:
	friend class CEngine;
	static CLightFactory* GetInstance();
	bool Init(CEngine& anEngine);
	CEnvironmentLight* CreateEnvironmentLight(std::string aCubeMapPath);
	CPointLight* CreatePointLight();
private:
	CEngine* myEngine;


private:
	CLightFactory();
	~CLightFactory();
	static CLightFactory* ourInstance;

};