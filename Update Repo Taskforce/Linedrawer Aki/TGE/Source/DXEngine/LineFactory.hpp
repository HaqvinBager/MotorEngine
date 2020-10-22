#pragma once

#include "SimpleMath.h"

struct ID3D11Device;
class DirectX11Framework;

class Line;
class LineInstance;
class LineFactory
{
public:
	static void			 Create();
	static LineFactory&  GetInstance();
	void				 Init(DirectX11Framework* aFramework);
	void				 Destroy();

	Line* CreateLine(const DirectX::SimpleMath::Vector3& aFrom, const DirectX::SimpleMath::Vector3& aTo, const DirectX::SimpleMath::Vector4& aColor);
	Line* CreateGrid(const DirectX::SimpleMath::Vector4& aColor);

private:
	LineFactory();
	~LineFactory();

	static LineFactory* ourInstance;

	ID3D11Device* myDevice;
};

