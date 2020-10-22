#pragma once
//pch.h of DXEngine
#include <d3d11.h>

#include "SimpleMath.h"
#include <DirectXMath.h>

#include <array>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include <cassert>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define SAFE_DELETE(aPointer) delete aPointer; aPointer = nullptr;