#pragma once
#include "Particle.h"
#include <map>

class CDirectXFramework;

class CParticleFactory
{
public:
	CParticleFactory();
	~CParticleFactory();

	bool Init(CDirectXFramework* aFramework);

	CParticle* GetParticle(std::string aFilePath);
	CParticle* LoadParticle(std::string aFilePath);

public:
	static CParticleFactory* GetInstance();
	ID3D11ShaderResourceView* GetShaderResourceView(ID3D11Device* aDevice, std::string aTexturePath);
	void ReadJsonValues(std::string aFilePath, CParticle::SParticleData& someParticleData);

private:
	static CParticleFactory* ourInstance;

private:
	ID3D11Device* myDevice;
	std::map<std::string, CParticle*> myParticles;
};

