#pragma once
#include <assert.h>
#include <memory.h>
#include <vector>
#include "Vector.h"

// SIMPLE Wrappers
namespace FBXLoader
{
	struct Vector4f
	{
		Vector4f()
		{
			myX = 0;
			myY = 0;
			myZ = 0;
			myW = 0;
		}
		Vector4f(const float& aX,const float& aY, const float& aZ,const float& aW)
		{
			myX = aX;
			myY = aY;
			myZ = aZ;
			myW = aW;
		}
		float myX;
		float myY;
		float myZ;
		float myW;
	};

	struct Vector2f
	{
		Vector2f()
		{
			myX = 0;
			myY = 0;
		}
		Vector2f(const float& aX, const float& aY)
		{
			myX = aX;
			myY = aY;
		}
		float myX;
		float myY;
	};

	struct Matrix44f
	{
		Matrix44f()
		{
			myMatrix[0][0] = 1;
			myMatrix[1][0] = 0;
			myMatrix[2][0] = 0;
			myMatrix[3][0] = 0;
			myMatrix[0][1] = 0;
			myMatrix[1][1] = 1;
			myMatrix[2][1] = 0;
			myMatrix[3][1] = 0;
			myMatrix[0][2] = 0;
			myMatrix[1][2] = 0;
			myMatrix[2][2] = 1;
			myMatrix[3][2] = 0;
			myMatrix[0][3] = 0;
			myMatrix[1][3] = 0;
			myMatrix[2][3] = 0;
			myMatrix[3][3] = 1;
		}
		float myMatrix[4][4];

		void InitScaleTransform(const float &ScaleX, const float& ScaleY, const float& ScaleZ)
		{
			myMatrix[0][0] = ScaleX; myMatrix[0][1] = 0.0f;   myMatrix[0][2] = 0.0f;   myMatrix[0][3] = 0.0f;
			myMatrix[1][0] = 0.0f;   myMatrix[1][1] = ScaleY; myMatrix[1][2] = 0.0f;   myMatrix[1][3] = 0.0f;
			myMatrix[2][0] = 0.0f;   myMatrix[2][1] = 0.0f;   myMatrix[2][2] = ScaleZ; myMatrix[2][3] = 0.0f;
			myMatrix[3][0] = 0.0f;   myMatrix[3][1] = 0.0f;   myMatrix[3][2] = 0.0f;   myMatrix[3][3] = 1.0f;
		}
		void InitTranslationTransform(const float& x, const float& y, const float& z)
		{
			myMatrix[0][0] = 1.0f; myMatrix[0][1] = 0.0f; myMatrix[0][2] = 0.0f; myMatrix[0][3] = 0;
			myMatrix[1][0] = 0.0f; myMatrix[1][1] = 1.0f; myMatrix[1][2] = 0.0f; myMatrix[1][3] = 0;
			myMatrix[2][0] = 0.0f; myMatrix[2][1] = 0.0f; myMatrix[2][2] = 1.0f; myMatrix[2][3] = 0;
			myMatrix[3][0] = x;    myMatrix[3][1] = y;	  myMatrix[3][2] = z;	 myMatrix[3][3] = 1.0f;
		}
		Matrix44f& operator*=(const Matrix44f& aMatrix)
		{
			Matrix44f temp;

			temp.myMatrix[0][0] = myMatrix[0][0] * aMatrix.myMatrix[0][0] + myMatrix[0][1] * aMatrix.myMatrix[1][0] + myMatrix[0][2] * aMatrix.myMatrix[2][0] + myMatrix[0][3] * aMatrix.myMatrix[3][0];
			temp.myMatrix[0][1] = myMatrix[0][0] * aMatrix.myMatrix[0][1] + myMatrix[0][1] * aMatrix.myMatrix[1][1] + myMatrix[0][2] * aMatrix.myMatrix[2][1] + myMatrix[0][3] * aMatrix.myMatrix[3][1];
			temp.myMatrix[0][2] = myMatrix[0][0] * aMatrix.myMatrix[0][2] + myMatrix[0][1] * aMatrix.myMatrix[1][2] + myMatrix[0][2] * aMatrix.myMatrix[2][2] + myMatrix[0][3] * aMatrix.myMatrix[3][2];
			temp.myMatrix[0][3] = myMatrix[0][0] * aMatrix.myMatrix[0][3] + myMatrix[0][1] * aMatrix.myMatrix[1][3] + myMatrix[0][2] * aMatrix.myMatrix[2][3] + myMatrix[0][3] * aMatrix.myMatrix[3][3];

			temp.myMatrix[1][0] = myMatrix[1][0] * aMatrix.myMatrix[0][0] + myMatrix[1][1] * aMatrix.myMatrix[1][0] + myMatrix[1][2] * aMatrix.myMatrix[2][0] + myMatrix[1][3] * aMatrix.myMatrix[3][0];
			temp.myMatrix[1][1] = myMatrix[1][0] * aMatrix.myMatrix[0][1] + myMatrix[1][1] * aMatrix.myMatrix[1][1] + myMatrix[1][2] * aMatrix.myMatrix[2][1] + myMatrix[1][3] * aMatrix.myMatrix[3][1];
			temp.myMatrix[1][2] = myMatrix[1][0] * aMatrix.myMatrix[0][2] + myMatrix[1][1] * aMatrix.myMatrix[1][2] + myMatrix[1][2] * aMatrix.myMatrix[2][2] + myMatrix[1][3] * aMatrix.myMatrix[3][2];
			temp.myMatrix[1][3] = myMatrix[1][0] * aMatrix.myMatrix[0][3] + myMatrix[1][1] * aMatrix.myMatrix[1][3] + myMatrix[1][2] * aMatrix.myMatrix[2][3] + myMatrix[1][3] * aMatrix.myMatrix[3][3];

			temp.myMatrix[2][0] = myMatrix[2][0] * aMatrix.myMatrix[0][0] + myMatrix[2][1] * aMatrix.myMatrix[1][0] + myMatrix[2][2] * aMatrix.myMatrix[2][0] + myMatrix[2][3] * aMatrix.myMatrix[3][0];
			temp.myMatrix[2][1] = myMatrix[2][0] * aMatrix.myMatrix[0][1] + myMatrix[2][1] * aMatrix.myMatrix[1][1] + myMatrix[2][2] * aMatrix.myMatrix[2][1] + myMatrix[2][3] * aMatrix.myMatrix[3][1];
			temp.myMatrix[2][2] = myMatrix[2][0] * aMatrix.myMatrix[0][2] + myMatrix[2][1] * aMatrix.myMatrix[1][2] + myMatrix[2][2] * aMatrix.myMatrix[2][2] + myMatrix[2][3] * aMatrix.myMatrix[3][2];
			temp.myMatrix[2][3] = myMatrix[2][0] * aMatrix.myMatrix[0][3] + myMatrix[2][1] * aMatrix.myMatrix[1][3] + myMatrix[2][2] * aMatrix.myMatrix[2][3] + myMatrix[2][3] * aMatrix.myMatrix[3][3];

			temp.myMatrix[3][0] = myMatrix[3][0] * aMatrix.myMatrix[0][0] + myMatrix[3][1] * aMatrix.myMatrix[1][0] + myMatrix[3][2] * aMatrix.myMatrix[2][0] + myMatrix[3][3] * aMatrix.myMatrix[3][0];
			temp.myMatrix[3][1] = myMatrix[3][0] * aMatrix.myMatrix[0][1] + myMatrix[3][1] * aMatrix.myMatrix[1][1] + myMatrix[3][2] * aMatrix.myMatrix[2][1] + myMatrix[3][3] * aMatrix.myMatrix[3][1];
			temp.myMatrix[3][2] = myMatrix[3][0] * aMatrix.myMatrix[0][2] + myMatrix[3][1] * aMatrix.myMatrix[1][2] + myMatrix[3][2] * aMatrix.myMatrix[2][2] + myMatrix[3][3] * aMatrix.myMatrix[3][2];
			temp.myMatrix[3][3] = myMatrix[3][0] * aMatrix.myMatrix[0][3] + myMatrix[3][1] * aMatrix.myMatrix[1][3] + myMatrix[3][2] * aMatrix.myMatrix[2][3] + myMatrix[3][3] * aMatrix.myMatrix[3][3];

			memcpy(myMatrix, temp.myMatrix, 4 * 4 * 4);

			return *this;
		}
		Matrix44f operator*(const Matrix44f& aSecondMatrix) const
		{
			Matrix44f Ret;

			for (unsigned int i = 0; i < 4; i++) {
				for (unsigned int j = 0; j < 4; j++) {
					Ret.myMatrix[i][j] = myMatrix[i][0] * aSecondMatrix.myMatrix[0][j] +
						myMatrix[i][1] * aSecondMatrix.myMatrix[1][j] +
						myMatrix[i][2] * aSecondMatrix.myMatrix[2][j] +
						myMatrix[i][3] * aSecondMatrix.myMatrix[3][j];
				}
			}

			return Ret;
		}
	};
}


typedef unsigned int uint;
typedef unsigned short ushort;

enum EModelBluePrint
{
	EModelBluePrint_Position = 1 << 0,
	EModelBluePrint_Normal = 1 << 1,
	EModelBluePrint_BinormTan = 1 << 2,
	EModelBluePrint_UV = 1 << 3,
	EModelBluePrint_Bones = 1 << 4,
	EModelBluePrint_ScreenSpace = 1 << 5,
};


#define NUM_BONES_PER_VEREX 4
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
struct VertexBoneData
{
	unsigned int IDs[NUM_BONES_PER_VEREX];
	float Weights[NUM_BONES_PER_VEREX];

	VertexBoneData()
	{
		Reset();
	};

	void Reset()
	{
		ZERO_MEM(IDs);
		ZERO_MEM(Weights);
	}

	void AddBoneData(unsigned int& BoneID, float& Weight)
	{
		for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
			if (Weights[i] == 0.0) {
				IDs[i] = BoneID;
				Weights[i] = Weight;
				return;
			}
		}

		// should never get here - more bones than we have space for
		assert(0);
	}
};



class SVertexCollection
{
public:
	void PushVec4(FBXLoader::Vector4f& aPos)
	{
		myData.Push(aPos.myX);
		myData.Push(aPos.myY);
		myData.Push(aPos.myZ);
		myData.Push(aPos.myW);

	}
	void PushVec2(FBXLoader::Vector2f& aPos)
	{
		myData.Push(aPos.myX);
		myData.Push(aPos.myY);
	}

	Vector<float> myData;
};