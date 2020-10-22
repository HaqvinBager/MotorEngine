#pragma once
#include <d3d11.h>
#include <string>
#include "..\DirectXTK\Inc\SimpleMath.h"

namespace SM = DirectX::SimpleMath;

class CEnvironmentLight {
public:
	CEnvironmentLight();
	~CEnvironmentLight();
	bool Init(std::string aFilePath, ID3D11Device* aDevice);
	void SetDirection(SM::Vector3 aDirection);
	void SetColor(SM::Vector3 aColor);
	ID3D11ShaderResourceView* const* GetCubemap();
	unsigned int GetMipCount() const;
	SM::Vector4 GetDirection() const;
	SM::Vector4 GetColor() const;

private:
	ID3D11ShaderResourceView* myCubemap;
	SM::Vector4 myDirection;
	SM::Vector4 myColor;
	unsigned int myMipCount;
};

