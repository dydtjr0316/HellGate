#pragma once

// simd 사용 안함
#define _XM_NO_INTRINSICS_

#include <d3d12.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;



#include <string>
#include <vector>
#include <list>
#include <map>


using std::vector;
using std::list;
using std::map;
using std::string;
using std::wstring;
using std::make_pair;

#include "d3dx12.h" // DX12 2헤더
#include "define.h" // 매크로, enum	
#include "struct.h" // 구조체 설계
// #include "extern.h" // extern 변수
#include "func.h"   // 전역 함수

#include "SimpleMath.h"
