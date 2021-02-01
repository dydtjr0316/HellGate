#include "pch.h"
#include "Core.h"

#include "Device.h"

CCore::CCore()
	: m_hMainHwnd(nullptr)
{
}

CCore::~CCore()
{
}

int CCore::init(HWND _hWnd, const tResolution& _resolution, bool _bWindow)
{
	m_hMainHwnd = _hWnd;
	ChangeWindowSize(m_hMainHwnd, _resolution);
	ShowWindow(_hWnd, true);

	if (FAILED(CDevice::GetInst()->initDirect3D(_hWnd, _resolution, _bWindow)))
	{
		return E_FAIL;
	}

	// 상수 버퍼 만들기
	CDevice::GetInst()->CreateConstBuffer(L"GLOBAL_MATRIX_1", sizeof(tTransform), 512, CONST_REGISTER::b0);
	CDevice::GetInst()->CreateConstBuffer(L"GLOBAL_MATRIX_2", sizeof(tTransform), 512, CONST_REGISTER::b1);

	// 매니저 초기화

	return S_OK;
}

void CCore::ChangeWindowSize(HWND _hWnd, const tResolution& _resolution)
{
	RECT rt = { 0, 0, (int)_resolution.fWidth, (int)_resolution.fHeight };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.top - rt.bottom, 0);
}

void CCore::prgress()
{

}

void CCore::update()
{

}

void CCore::lateupdate()
{

}

void CCore::finalupdate()
{

}

void CCore::render()
{

}