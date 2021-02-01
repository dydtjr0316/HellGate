#include "pch.h"
#include "Core.h"

#include "Device.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "PathManager.h"

CCore::CCore()
	: m_hMainHwnd( nullptr ) {
}

CCore::~CCore() {
}

int CCore::init( HWND _hWnd, const tResolution& _resolution, bool _bWindow ) {
	return 0;
}

void CCore::ChangeWindowSize( HWND _hWnd, const tResolution& _resolution ) {
}

void CCore::progress() {
}

void CCore::update() {
}

void CCore::lateupdate() {
}

void CCore::finalupdate() {
}

void CCore::render() {
}
