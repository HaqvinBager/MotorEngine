#pragma once
#include "WindowHandler.h"
#include "WinInclude.h"

#include <comdef.h>
#include <d3d11.h>

#include <codecvt>
#include <cstdarg>
#include <iostream>

#include <SimpleMath.h>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <array>
#include "Input.h"
#include "Timer.h"
#include "DirectXFramework.h"
#include "EngineException.h"
#define SAFE_DELETE(aPointer) delete aPointer; aPointer = nullptr;
