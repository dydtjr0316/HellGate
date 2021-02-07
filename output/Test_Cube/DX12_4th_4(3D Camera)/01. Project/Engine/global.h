#pragma once


// simd ��� ����
#define _XM_NO_INTRINSICS_

#include <d3d12.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <algorithm>
#include "SimpleMath.h"

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

#include "d3dx12.h"	// Dx12 ����Ʈ ���
#include "define.h" // ��ũ��, enum
#include "struct.h" // ����ü ����
#include "extern.h" // extern ����
#include "func.h"   // ���� �Լ�