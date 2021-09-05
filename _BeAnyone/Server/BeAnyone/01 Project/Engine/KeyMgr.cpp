#include "pch.h"
#include "KeyMgr.h"
#include "Camera.h"
#include "Device.h"
#include "RenderMgr.h"
#include "MRT.h"

// KEY_TYPE �� �����ϴ� ����Ű
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
	// ���� �����찡 Active(Ȱ��ȭ, ��Ŀ��) ���°� �ƴ϶��, 
	// ��� Ű ���¸� �ʱ���·� �����Ѵ�.
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

	// �����찡 Ȱ�����¶�� Ű�� ���������� update �Ѵ�.
	for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
	{
		// ���� Ű�� �����ִ�.
		if (GetAsyncKeyState(arrVK[i]) & 0x8000)
		{
			if (m_vecKey[i].bPrevPress) // ���� �����ӿ� �����־���.
			{
				m_vecKey[i].eState = KEY_STATE::STATE_HOLD;
			}
			else
			{
				m_vecKey[i].eState = KEY_STATE::STATE_TAB;
			}
			m_vecKey[i].bPrevPress = true;
		}
		else // Ű�� ������ ���� ����
		{
			if (m_vecKey[i].bPrevPress) // ���� �����ӿ� �����־���.
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

	// ���콺 ��ǥ	
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
	//pd3ddevice->GetTransform(D3DTS_VIEW, &matView);  //View matrix ������ ���´�.
	//D3DXMatrixInverse(&m, NULL, &matView);    //View matrix �� ������� ���Ѵ�.
	//vPickRayOrig = D3DXVECTOR3(m._41, m._42, m._43); //������ �Ǵ� ���� 
	//vPickRayDir.x = px * m._11 + py * m._21 + 1.f * m._31; //������ �Ǵ� ����
	//vPickRayDir.y = px * m._12 + py * m._22 + 1.f * m._32;
	//vPickRayDir.z = px * m._13 + py * m._23 + 1.f * m._33;
	//D3DXPLANE pPlane = D3DXPLANE(0.f, 0.f, 1.f, 0.f);  //����� �����Ѵ�.	

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
	//pd3ddevice->GetTransform(D3DTS_VIEW, &matView);  //View matrix ������ ���´�.
	//D3DXMatrixInverse(&m, NULL, &matView);    //View matrix �� ������� ���Ѵ�.
	//vPickRayOrig = D3DXVECTOR3(m._41, m._42, m._43); //������ �Ǵ� ���� 
	//vPickRayDir.x = px * m._11 + py * m._21 + 1.f * m._31; //������ �Ǵ� ����
	//vPickRayDir.y = px * m._12 + py * m._22 + 1.f * m._32;
	//vPickRayDir.z = px * m._13 + py * m._23 + 1.f * m._33;
	//D3DXPLANE pPlane = D3DXPLANE(0.f, 0.f, 1.f, 0.f);  //����� �����Ѵ�.	

	return Vector3(fPoint.x, fPoint.y, 1.f);
}
