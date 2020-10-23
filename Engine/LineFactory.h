#pragma once

#include "SimpleMath.h"

struct ID3D11Device;
class CDirectXFramework;

class CLine;
class CLineInstance;
class CLineFactory
{
	friend class CEngine;
public:
	static CLineFactory* GetInstance();
	bool				 Init(CDirectXFramework* aFramework);

	CLine* CreateLine(const DirectX::SimpleMath::Vector3& aFrom, const DirectX::SimpleMath::Vector3& aTo, const DirectX::SimpleMath::Vector4& aColor);
	CLine* CreateGrid(const DirectX::SimpleMath::Vector4& aColor);

private:
	CLineFactory();
	~CLineFactory();

	ID3D11Device* myDevice;
	static CLineFactory* ourInstance;
};

