#include "stdafx.h"
#include "PointLight.h"

CPointLight::CPointLight()
{
}

CPointLight::~CPointLight()
{
}

bool CPointLight::Init()
{
	myPosition = {0.0f, 0.0f, 0.0f, 1.0f};
	myColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	myRange = 0.0f;
	myIntensity = 0.0f;
	return true;
}
