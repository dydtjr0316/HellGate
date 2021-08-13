#include "pch.h"
#include "Core.h"

#include "Device.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"

#include "SceneMgr.h"
#include "EventMgr.h"
#include "RenderMgr.h"
#include "ItemMgr.h"

#include "PathMgr.h"
#include "ConstantBuffer.h"

#include "MonsterScript.h"

CCore::CCore()
	: m_hMainHwnd(nullptr)
{
}

CCore::~CCore()
{
	//TestRelease();
}

int CCore::init(HWND _hWnd, const tResolution& _resolution, bool _bWindow)
{
	m_hMainHwnd = _hWnd;
	ChangeWindowSize(m_hMainHwnd, _resolution);
	ShowWindow(_hWnd, true);

	if (FAILED(CDevice::GetInst()->initDirect3D (_hWnd, _resolution, _bWindow)))
	{
		return E_FAIL;
	}

	CRenderMgr::GetInst()->init(_hWnd, _resolution, _bWindow);

	// ��� ���� �����
	CDevice::GetInst()->CreateConstBuffer(L"TRANSFORM_MATRIX", sizeof(tTransform), 512, CONST_REGISTER::b0);
	CDevice::GetInst()->CreateConstBuffer(L"MATERIAL_PARAM", sizeof(tMtrlParam), 512, CONST_REGISTER::b1);
	CDevice::GetInst()->CreateConstBuffer(L"ANIM2D", sizeof(tMtrlParam), 512, CONST_REGISTER::b2);

	// ���� ������� ����(1������ ���� �������Ϳ��� ���ӵǾ���)
	//CDevice::GetInst()->CreateConstBuffer(L"LIGHT2D", sizeof(tLightInfo), 1, CONST_REGISTER::b3, true);
	CDevice::GetInst()->CreateConstBuffer(L"LIGHT3D", sizeof(tLightInfo), 1, CONST_REGISTER::b3, true);

	// �Ŵ��� �ʱ�ȭ
	CPathMgr::init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();

	CResMgr::GetInst()->init();

	CSceneMgr::GetInst()->init();

	CItemMgr::GetInst()->init();
	// TestInit();
	//CSound::GetInst()->Play(Sound_Type::BGM, true, false);
	return S_OK;
}

void CCore::ChangeWindowSize(HWND _hWnd, const tResolution& _resolution)
{
	RECT rt = { 0, 0, (int)_resolution.fWidth, (int)_resolution.fHeight };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void CCore::progress()
{
	
	CKeyMgr::GetInst()->update();
	/*cout << "�ھ��� CKeyMgr::GetInst()->update() ���� �� " << endl;
	for (auto& obj : g_Object)
	{
		if ((obj.first == 1000 || obj.first == 1001) &&
			obj.second->GetScript<CMonsterScript>()->GetPacketMove() != nullptr)
		{
			cout << "ID : " << obj.first << "    Dir : " <<
				(int)obj.second->GetScript<CMonsterScript>()->GetPacketMove()->eDir << endl;
		}
	}*/
	CTimeMgr::GetInst()->update();
	/*cout << "�ھ��� CTimeMgr::GetInst()->update() ���� �� " << endl;
	for (auto& obj : g_Object)
	{
		if ((obj.first == 1000 || obj.first == 1001) &&
			obj.second->GetScript<CMonsterScript>()->GetPacketMove() != nullptr)
		{
			cout << "ID : " << obj.first << "    Dir : " <<
				(int)obj.second->GetScript<CMonsterScript>()->GetPacketMove()->eDir << endl;
		}
	}*/

	CEventMgr::GetInst()->clear();
	{
		CSceneMgr::GetInst()->update();
		CRenderMgr::GetInst()->render();
		/*cout << "�ھ��� CRenderMgr::GetInst()->render() ���� �� " << endl;
		for (auto& obj : g_Object)
		{
			if ((obj.first == 1000 || obj.first == 1001) &&
				obj.second->GetScript<CMonsterScript>()->GetPacketMove() != nullptr)
			{
				cout << "ID : " << obj.first << "    Dir : " <<
					(int)obj.second->GetScript<CMonsterScript>()->GetPacketMove()->eDir << endl;
			}
		}*/
	}
	CEventMgr::GetInst()->update();

	/*cout << "�ھ��� CEventMgr::GetInst()->update() ���� �� " << endl;
	for (auto& obj : g_Object)
	{
		if ((obj.first == 1000 || obj.first == 1001) &&
			obj.second->GetScript<CMonsterScript>()->GetPacketMove() != nullptr)
		{
			cout << "ID : " << obj.first << "    Dir : " <<
				(int)obj.second->GetScript<CMonsterScript>()->GetPacketMove()->eDir << endl;
		}
	}
	cout << "-------------" << endl;*/

	int i = 0;
}

