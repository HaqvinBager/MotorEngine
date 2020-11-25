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

	CParticle* LoadParticle(std::string aFilePath);
	CParticle* GetParticle(std::string aFilePath);

	std::vector<CParticle*> GetParticleSet(std::vector<std::string> someFilePaths);

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

