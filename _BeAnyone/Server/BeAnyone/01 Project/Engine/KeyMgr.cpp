#include "pch.h"
#include "KeyMgr.h"
#include "Camera.h"
#include "Device.h"
#include "RenderMgr.h"
#include "MRT.h"

// KEY_TYPE 에 대응하는 가상키
int arrVK[(UINT)KEY_TYPE::KEY_END] =
{
	VK_UP,		// KEY_UP,
	VK_DOWN,	// KEY_DOWN,
	VK_LEFT,	// KEY_LEFT,
	VK_RIGHT,	// KEY_RIGHT,

	VK_SPACE,   // KEY_SPACE,
	VK_RETURN,  // KEY_ENTER,
	VK_LSHIFT,  // KEY_LSHIFT,
	VK_LCONTROL, // KEY_LCTRL,
	VK_MENU,	// KEY_LALT,
	VK_ESCAPE,	// KEY_ESC,

	'Q',		// KEY_Q, ..
	'W',
	'S',
	'A',
	'D',
	'E',
	'R',
	'Z',
	'X',
	'I',
	'P',
	'M',
	'N',

	VK_LBUTTON,
	VK_RBUTTON,

	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,
};

CKeyMgr::CKeyMgr() {
}

CKeyMgr::~CKeyMgr() {
}

void CKeyMgr::init() {
	for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
	{
		m_vecKey.push_back(tKeyState{ KEY_STATE::STATE_NONE, false });
	}
}

void CKeyMgr::update() {
	// 현재 윈도우가 Active(활성화, 포커싱) 상태가 아니라면, 
	// 모든 키 상태를 초기상태로 변경한다.
	HWND hActive = GetActiveWindow();

	if (nullptr == hActive)
	{
		for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
		{
			m_vecKey[i].bPrevPress = false;
			m_vecKey[i].eState = KEY_STATE::STATE_NONE;
		}
		return;
	}

	// 윈도우가 활성상태라면 키를 정상적으로 update 한다.
	for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
	{
		// 현재 키가 눌려있다.
		if (GetAsyncKeyState(arrVK[i]) & 0x8000)
		{
			if (m_vecKey[i].bPrevPress) // 이전 프레임에 눌려있었다.
			{
				m_vecKey[i].eState = KEY_STATE::STATE_HOLD;
			}
			else
			{
				m_vecKey[i].eState = KEY_STATE::STATE_TAB;
			}
			m_vecKey[i].bPrevPress = true;
		}
		else // 키가 눌리지 않은 상태
		{
			if (m_vecKey[i].bPrevPress) // 이전 프레임에 눌려있었다.
			{
				m_vecKey[i].eState = KEY_STATE::STATE_AWAY;
			}
			else
			{
				m_vecKey[i].eState = KEY_STATE::STATE_NONE;
			}
			m_vecKey[i].bPrevPress = false;
		}
	}

	// 마우스 좌표	
	m_ptOldMouse = m_ptMouse;
	GetCursorPos(&m_ptMouse);
	ScreenToClient(CRenderMgr::GetInst()->GetHwnd(), &m_ptMouse);

	m_vDragDir = Vector2((float)(m_ptMouse.x - m_ptOldMouse.x), (float)(m_ptOldMouse.y - m_ptMouse.y));
}

Vector3 CKeyMgr::GetTransformedPoint(Matrix _mat)
{
	POINT mousePoint = GetMousePos();
	POINT fPoint{};

	D3D12_VIEWPORT view = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->GetView();
	Matrix matProj = _mat;

	fPoint.x = (((2.f * mousePoint.x) / view.Width) - 1.f) / matProj._11;
	fPoint.y = (((-2.f * mousePoint.y) / view.Height) + 1.f) / matProj._22;

	//D3DXMATRIX matView, m, matWorld;
	//pd3ddevice->GetTransform(D3DTS_VIEW, &matView);  //View matrix 정보를 얻어온다.
	//D3DXMatrixInverse(&m, NULL, &matView);    //View matrix 의 역행렬을 구한다.
	//vPickRayOrig = D3DXVECTOR3(m._41, m._42, m._43); //시점이 되는 백터 
	//vPickRayDir.x = px * m._11 + py * m._21 + 1.f * m._31; //종점이 되는 벡터
	//vPickRayDir.y = px * m._12 + py * m._22 + 1.f * m._32;
	//vPickRayDir.z = px * m._13 + py * m._23 + 1.f * m._33;
	//D3DXPLANE pPlane = D3DXPLANE(0.f, 0.f, 1.f, 0.f);  //평면을 정의한다.	

	return Vector3(fPoint.x, fPoint.y, 1.f);
}


Vector3 CKeyMgr::GetTransformedPoint(CGameObject* _cam)
{
	POINT mousePoint = GetMousePos();
	POINT fPoint{};

	D3D12_VIEWPORT view = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->GetView();
	Matrix matProj = _cam->Camera()->GetProjMat();
	
	fPoint.x = (((2.f * mousePoint.x) / view.Width) - 1.f) / matProj._11;
	fPoint.y = (((-2.f * mousePoint.y) / view.Height) + 1.f) / matProj._22;

	//D3DXMATRIX matView, m, matWorld;
	//pd3ddevice->GetTransform(D3DTS_VIEW, &matView);  //View matrix 정보를 얻어온다.
	//D3DXMatrixInverse(&m, NULL, &matView);    //View matrix 의 역행렬을 구한다.
	//vPickRayOrig = D3DXVECTOR3(m._41, m._42, m._43); //시점이 되는 백터 
	//vPickRayDir.x = px * m._11 + py * m._21 + 1.f * m._31; //종점이 되는 벡터
	//vPickRayDir.y = px * m._12 + py * m._22 + 1.f * m._32;
	//vPickRayDir.z = px * m._13 + py * m._23 + 1.f * m._33;
	//D3DXPLANE pPlane = D3DXPLANE(0.f, 0.f, 1.f, 0.f);  //평면을 정의한다.	

	return Vector3(fPoint.x, fPoint.y, 1.f);
}
