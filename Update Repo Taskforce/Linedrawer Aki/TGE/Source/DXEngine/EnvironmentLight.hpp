#pragma once

#include "SimpleMath.h"

struct ID3D11ShaderResourceView;
class EnvironmentLight
{
public:
	//EnvironmentLight(ID3D11ShaderResourceView* const* aCubeMap);
	EnvironmentLight(ID3D11ShaderResourceView* aCubeMap);
	~EnvironmentLight();

	//void SetDirection(const DirectX::SimpleMath::Vector4& aDirection);
	//void SetDirectionAndNormalize(const DirectX::SimpleMath::Vector4& aDirection);
	void SetColor(const DirectX::SimpleMath::Vector4& aColor);

public:
	ID3D11ShaderResourceView* const* GetCubeMap()		{ return &myCubeMap; }
	//const DirectX::SimpleMath::Vector4& GetDirection()	{ return myDirection; }
	const DirectX::SimpleMath::Vector4& GetColor()		{ return myColor; }
	
private:
	//DirectX::SimpleMath::Vector4 myDirection;
	DirectX::SimpleMath::Vector4 myColor;
	ID3D11ShaderResourceView* myCubeMap;
	//ID3D11ShaderResourceView* const* myCubeMap;
};

