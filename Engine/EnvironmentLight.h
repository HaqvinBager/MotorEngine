#pragma once

struct ID3D11ShaderResourceView;
class CDirectXFramework;
namespace DirectX {
	namespace SimpleMath {
		struct Vector4;
		struct Vector3;
	}
}

class CEnvironmentLight
{
public:
	CEnvironmentLight();
	~CEnvironmentLight();

	bool Init(CDirectXFramework* aFramework, std::string aFilePath);

	void SetDirection(DirectX::SimpleMath::Vector3 aDirection)
	{
		aDirection.Normalize();
		myDirection.x = aDirection.x;
		myDirection.y = aDirection.y;
		myDirection.z = aDirection.z;
		//myDirection.Normalize();
	}
	void SetColor(DirectX::SimpleMath::Vector3 aColor)
	{
		myColor.x = aColor.x; 
		myColor.y = aColor.y; 
		myColor.z = aColor.z; 
	}
	void SetIntensity(float anIntensity)
	{
		myColor.w = anIntensity;
	}

	DirectX::SimpleMath::Vector4 GetDirection() { return myDirection; }
	DirectX::SimpleMath::Vector4 GetColor() { return myColor; }

	ID3D11ShaderResourceView* const* GetCubeMap();
	unsigned int GetMipLevels();
private:
	unsigned int myMipLevels;
	ID3D11ShaderResourceView* cubeShaderResourceView = nullptr;
	DirectX::SimpleMath::Vector4 myDirection; //Points towards the Light
	DirectX::SimpleMath::Vector4 myColor; //Color (XYZ) and Intensity (W)
};