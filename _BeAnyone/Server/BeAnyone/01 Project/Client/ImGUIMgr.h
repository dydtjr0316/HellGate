#pragma once
class CImGUIMgr
{
	SINGLE(CImGUIMgr)
private:
	ID3D12DescriptorHeap* m_pDescHeap;

public:
	void init(HWND _hwnd);
};