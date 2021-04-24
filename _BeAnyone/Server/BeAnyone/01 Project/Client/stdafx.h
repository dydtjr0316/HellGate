// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

<<<<<<< HEAD
=======
#include<d3d12.h>
#include "SimpleMath.h"
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;

#include "../../../../Server/Server/Server/Protocol.h"
#include "../../../../Server/Server/Server/Enum.h"
#include "../../../../Server/Server/Server/Struct.h"


#include "Extern.h"
>>>>>>> 28ea19eafed0606209d4a387ff302cbc8329a72b
//#include "../Engine/extern.h"

// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
