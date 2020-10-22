#pragma once

#include "../DirectXTK/Inc/SimpleMath.h"
//namespace DirectX{
//	namespace SimpleMath{
//		struct Vector3;
//		struct Matrix;
//	}
//}


namespace ModelSystem
{
	class Model;
	class ModelInstance
	{
	public:
		ModelInstance();
		~ModelInstance();

		void Init(Model* aModel);

		void SetTransform(const DirectX::SimpleMath::Vector3& aPosition, const DirectX::SimpleMath::Vector3& aRotation);
		void SetPosition(const DirectX::SimpleMath::Vector3& aPosition);
		void SetRotation(const DirectX::SimpleMath::Vector3& aRotation);
		void SetScale(const DirectX::SimpleMath::Vector3& aScale);
		void Move(const DirectX::SimpleMath::Vector3& aMovement);
		void Rotate(const DirectX::SimpleMath::Vector3& aRotation);

	public:
		Model* GetModel(){ return myModel; }
		DirectX::SimpleMath::Matrix GetTransform(){ return myTransform; }

	private:
		Model*						myModel;
		DirectX::SimpleMath::Matrix myTransform;
	};
}