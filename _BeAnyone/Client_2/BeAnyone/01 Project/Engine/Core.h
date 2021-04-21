#pragma once
class CCore
{
	SINGLE(CCore);
public:
	HWND	m_hMainHwnd;	// 윈도우 핸들

public:
	int init(HWND _hWnd, const tResolution& _resolution, bool _bWindow);
	void progress();
	HWND GetWindowHwnd() { return m_hMainHwnd; }

private:
	void ChangeWindowSize(HWND _hWnd, const tResolution& _resolution);
};

