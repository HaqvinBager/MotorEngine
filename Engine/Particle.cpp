#include "stdafx.h"
#include "Particle.h"

CParticle::CParticle() : myParticleData()
{
}

CParticle::~CParticle()
{
}

void CParticle::Init(CParticle::SParticleData someParticleData)
{
	myParticleData = someParticleData;
}
