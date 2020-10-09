#pragma once
#include <xmmintrin.h>

#define QUICK_TRANSPOSE(row0, row1, row2, row3) {                 \
            __m128 _Tmp3, _Tmp2, _Tmp1, _Tmp0;                          \
                                                                    \
            _Tmp0   = _mm_shuffle_ps((row0), (row1), 0x44);          \
            _Tmp2   = _mm_shuffle_ps((row0), (row1), 0xEE);          \
            _Tmp1   = _mm_shuffle_ps((row2), (row3), 0x44);          \
            _Tmp3   = _mm_shuffle_ps((row2), (row3), 0xEE);          \
                                                                    \
            (row0) = _mm_shuffle_ps(_Tmp0, _Tmp1, 0x88);              \
            (row1) = _mm_shuffle_ps(_Tmp0, _Tmp1, 0xDD);              \
            (row2) = _mm_shuffle_ps(_Tmp2, _Tmp3, 0x88);              \
            (row3) = _mm_shuffle_ps(_Tmp2, _Tmp3, 0xDD);              \
        }

struct StructData
{
	float        _11, _12, _13, _14;
	float        _21, _22, _23, _24;
	float        _31, _32, _33, _34;
	float        _41, _42, _43, _44;
};

struct M128Data
{
	__m128 row0, row1, row2, row3;
};

struct SlimMatrix44
{

	void Transpose()
	{
		QUICK_TRANSPOSE(myM128Data.row0, myM128Data.row1, myM128Data.row2, myM128Data.row3);
	}

	void SetIdentity()
	{
		mySData._11 = 1; mySData._12 = 0; mySData._13 = 0; mySData._14 = 0;
		mySData._21 = 0; mySData._22 = 1; mySData._23 = 0; mySData._24 = 0;
		mySData._31 = 0; mySData._32 = 0; mySData._33 = 1; mySData._34 = 0;
		mySData._41 = 0; mySData._42 = 0; mySData._43 = 0; mySData._44 = 1;
	}

	union
	{
		StructData mySData;
		float myOneDimentionalData[16];
		float myTwoDimentionalData[4][4];

		__m128 myArrayRows[4];
		M128Data myM128Data;

	};
};
