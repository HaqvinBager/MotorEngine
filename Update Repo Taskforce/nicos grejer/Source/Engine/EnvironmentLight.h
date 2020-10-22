#pragma once
#include "SimpleMath.h"

struct ID3D11ShaderResourceView;
class CDirectXFramework;

class CEnvironmentLight
{
public:
	CEnvironmentLight();
	~CEnvironmentLight();

	bool Init(CDirectXFramework* aFramework, std::string aFilePath);
public:
	ID3D11ShaderResourceView* GetCubemap() const { return myCubemap; }
	DirectX::SimpleMath::Vector4 GetDirection() const { return myDirection; }
	DirectX::SimpleMath::Vector4 GetColor() const { return myColor; }
	UINT GetMipCount() const { return myMipCount; }
	void SetDirection(DirectX::SimpleMath::Vector3 aDirection) { myDirection.x = aDirection.x; myDirection.y = aDirection.y; myDirection.z = aDirection.z; myDirection.Normalize();  }
	void SetColor(DirectX::SimpleMath::Vector3 aColor) { myColor.x = aColor.x; myColor.y = aColor.y; myColor.z = aColor.z; }
private:
	ID3D11ShaderResourceView* myCubemap;
	DirectX::SimpleMath::Vector4 myDirection;
	DirectX::SimpleMath::Vector4 myColor;
	UINT myMipCount;
};

