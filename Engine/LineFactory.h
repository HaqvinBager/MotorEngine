#pragma once

#include "SimpleMath.h"

#define CLINE_DEFAULT_COLOR {0.44f, 0.77f, 1.0f, 1.0f}

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
	CLine* CreateAxisMarker();
	CLine* CreateSquareXZ(const float aWidth, const DirectX::SimpleMath::Vector4& aColor = CLINE_DEFAULT_COLOR);
	CLine* CreateCircleXZ(const float aRadius, const short aResolution = 16,const DirectX::SimpleMath::Vector4& aColor = CLINE_DEFAULT_COLOR);
	CLine* CreateTriangleXZ(const float aLength, const float aWidth, const DirectX::SimpleMath::Vector4& aColor = CLINE_DEFAULT_COLOR);
	CLine* CreatePolygon(std::vector<DirectX::SimpleMath::Vector3> somePositions);

private:
	CLineFactory();
	~CLineFactory();

	ID3D11Device* myDevice;
	static CLineFactory* ourInstance;
};

