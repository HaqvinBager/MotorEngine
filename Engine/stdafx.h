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
#include <unordered_map>
#include <string>
#include <map>
#include <array>

#include "EngineDefines.h"

#include "Input.h"
#include "Timer.h"
#include "DirectXFramework.h"
#include "EngineException.h"
#include "Behaviour.h"
#include "Collider.h"
#include "LightFactory.h"
#include "ModelFactory.h"
#include "LineFactory.h"
#include "CameraFactory.h"
#include "FullscreenTextureFactory.h"
#include "EnemyFactory.h"
#include "ParticleFactory.h"
#include "SpriteFactory.h"
#include "TextFactory.h"
#include "StateStack.h"
#include "State.h"

using namespace DirectX::SimpleMath;

#define SAFE_DELETE(aPointer) delete aPointer; aPointer = nullptr;

//namesapce SM = DirectX::SimpleMath;
