#include "stdafx.h"
#include "PointLight.h"

CPointLight::CPointLight() : myIntensity(0.0f), myRange(0.0f) {
}

CPointLight::~CPointLight() {
}

bool CPointLight::Init() {
	myPosition = { 0.0f, 0.0f, 0.0f };
	myColor = { 0.0f, 0.0f, 0.0f };

	return true;
}

void CPointLight::SetPosition(SM::Vector3 aPosition) {
	myPosition = aPosition;
}

void CPointLight::SetColor(SM::Vector3 aColor) {
	myColor.x = aColor.x;
	myColor.y = aColor.y;
	myColor.z = aColor.z;
}

void CPointLight::SetRange(float aRange) {
	myRange = aRange;
}

void CPointLight::SetIntensity(float aIntensity) {
	myIntensity = aIntensity;
}

SM::Vector3 CPointLight::GetPosition() const {
	return myPosition;
}

SM::Vector3 CPointLight::GetColor() const {
	return myColor;
}

float CPointLight::GetIntensity() const {
	return myIntensity;
}

float CPointLight::GetRange() const {
	return myRange;
}
