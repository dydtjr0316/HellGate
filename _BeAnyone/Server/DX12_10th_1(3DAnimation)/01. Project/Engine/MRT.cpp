#include "stdafx.h"
#include "MRT.h"

#include "Device.h"
#include "RenderMgr.h"

CMRT::CMRT()
	: m_arrRT{}
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(UINT _iCount, tRT * _arrRT, Ptr<CTexture> _pDSTex)
{
	// DepthStencilTexture �� ����.
	assert(nullptr != _pDSTex);		

	m_iRTCount = _iCount;
	memcpy(m_arrRT, _arrRT, sizeof(tRT) * _iCount);
	m_pDSTex = _pDSTex;

	// ������� RTV �� �����д�.
	D3D12_DESCRIPTOR_HEAP_DESC tDesc = {};
	tDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	tDesc.NumDescriptors = m_iRTCount;
	tDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	tDesc.NodeMask = 0;
	DEVICE->CreateDescriptorHeap(&tDesc, IID_PPV_ARGS(&m_pRTV));
	D3D12_CPU_DESCRIPTOR_HANDLE hRTVHeap = m_pRTV->GetCPUDescriptorHandleForHeapStart();
	
	UINT iRTVSize = CRenderMgr::GetInst()->GetRTVHeapSize();

	for (UINT i = 0; i < m_iRTCount; ++i)
	{
		UINT iDestRange = 1;
		UINT iSrcRange = 1;
				
		D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();
		hDescHandle.ptr += iRTVSize * i;

		ComPtr<ID3D12DescriptorHeap> pRTVHeap = m_arrRT[i].pTarget->GetRTV();
		D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = pRTVHeap->GetCPUDescriptorHandleForHeapStart();		
		
		DEVICE->CopyDescriptors(1, &hDescHandle, &iDestRange
			, 1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	for (UINT i = 0; i < m_iRTCount; ++i)
	{
		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = m_arrRT[i].pTarget->GetTex2D().Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// Ÿ�ٿ���
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;			// �Ϲ� ���ҽ���
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_TargetToRes[i] = barrier;
		
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;		// ���ҽ�
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	// ���� Ÿ������
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		m_ResToTarget[i] = barrier;
	}
}

void CMRT::OMSet(UINT _iCount, UINT _iOffset)
{
	UINT iRTVSize = CRenderMgr::GetInst()->GetRTVHeapSize();
	D3D12_CPU_DESCRIPTOR_HANDLE hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();
	hRTVHandle.ptr += iRTVSize *_iOffset;

	D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSTex->GetDSV()->GetCPUDescriptorHandleForHeapStart();

	// Ÿ�� ����	
	CMDLIST->OMSetRenderTargets(_iCount, &hRTVHandle, FALSE, &hDSVHandle);
}

void CMRT::OMSet()
{
	// RenderTarget �� DepthStencilView �� ����	
	D3D12_CPU_DESCRIPTOR_HANDLE hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();	
	D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSTex->GetDSV()->GetCPUDescriptorHandleForHeapStart();

	// Ÿ�� ����	
	CMDLIST->OMSetRenderTargets(m_iRTCount, &hRTVHandle, TRUE/*DescHeap �� ���������� �ִ�*/, &hDSVHandle);
}

void CMRT::TargetToResBarrier()
{
	CMDLIST->ResourceBarrier(m_iRTCount, m_TargetToRes);
}

void CMRT::ResToTargetBarrier()
{
	CMDLIST->ResourceBarrier(m_iRTCount, m_ResToTarget);
}

void CMRT::Clear()
{
	ResToTargetBarrier();

	// Ÿ�� Ŭ����	
	UINT iRTVSize = CRenderMgr::GetInst()->GetRTVHeapSize();	
	
	for (UINT i = 0; i < m_iRTCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();
		hRTVHandle.ptr += iRTVSize * i;
		float arrClearColor[4] = {m_arrRT[i].vClearColor.x, m_arrRT[i].vClearColor.y, m_arrRT[i].vClearColor.z, m_arrRT[i].vClearColor.w};
		CMDLIST->ClearRenderTargetView(hRTVHandle, arrClearColor, 0, nullptr);
	}

	if (nullptr != m_pDSTex)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSTex->GetDSV()->GetCPUDescriptorHandleForHeapStart();
		CMDLIST->ClearDepthStencilView(hDSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
	}	

}

void CMRT::Clear(UINT _iRTIdx)
{
	// Ÿ�� Ŭ����	
	UINT iRTVSize = CRenderMgr::GetInst()->GetRTVHeapSize();
	D3D12_CPU_DESCRIPTOR_HANDLE hRTVHandle = m_pRTV->GetCPUDescriptorHandleForHeapStart();

	hRTVHandle.ptr += iRTVSize * _iRTIdx;
	float arrClearColor[4] = { m_arrRT[_iRTIdx].vClearColor.x
		, m_arrRT[_iRTIdx].vClearColor.y
		, m_arrRT[_iRTIdx].vClearColor.z
		, m_arrRT[_iRTIdx].vClearColor.w };

	CMDLIST->ClearRenderTargetView(hRTVHandle, arrClearColor, 0, nullptr);
	
	if (nullptr != m_pDSTex)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE hDSVHandle = m_pDSTex->GetDSV()->GetCPUDescriptorHandleForHeapStart();
		CMDLIST->ClearDepthStencilView(hDSVHandle, D3D12_CLEAR_FLAG_DEPTH, 1.f, 0, 0, nullptr);
	}	
}
