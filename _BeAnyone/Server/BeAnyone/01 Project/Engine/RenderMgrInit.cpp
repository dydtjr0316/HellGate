#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"
#include "ResMgr.h"

#include "Core.h"
#include "ResMgr.h"
#include "Texture.h"
#include "MRT.h"


void CRenderMgr::init(HWND _hWnd, const tResolution& _res, bool _bWindow)
{
	m_hWnd = _hWnd;
	m_tResolution = _res;
	m_bWindowed = _bWindow;

	// MRT ����
	CreateMRT();
}

void CRenderMgr::CreateMRT()
{
	// RenderTargetViewHeap �� �޸� ������
	m_iRTVHeapSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// =============
	// SwapChain MRT
	// =============
	wchar_t szRTVName[50] = {};
	tRT arrRT[2] = {};

	for (UINT i = 0; i < 2; ++i)
	{
		wsprintf(szRTVName, L"SwapchainTargetTex_%d", i);
		ComPtr<ID3D12Resource> pTarget;
		CDevice::GetInst()->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(&pTarget));
		arrRT[i].pTarget = CResMgr::GetInst()->CreateTextureFromResource(szRTVName, pTarget);
	}

	Ptr<CTexture> pDSTex = CResMgr::GetInst()->CreateTexture(L"DepthStencilTex"
		, (UINT)m_tResolution.fWidth, (UINT)m_tResolution.fHeight
		, DXGI_FORMAT_D32_FLOAT, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
		, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(2, arrRT, pDSTex);

	// ============
	// Deferred MRT
	// ============
	{
		tRT arrRT[8] = {};

		arrRT[0].vClearColor = Vector4(0.f, 0.f, 0.f, 0.f);
		arrRT[0].pTarget = CResMgr::GetInst()->CreateTexture(L"DiffuseTargetTex"
			, (UINT)m_tResolution.fWidth, (UINT)m_tResolution.fHeight
			, DXGI_FORMAT_R8G8B8A8_UNORM, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
			, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, arrRT[0].vClearColor);

		arrRT[1].vClearColor = Vector4(0.f, 0.f, 0.f, 0.f);
		arrRT[1].pTarget = CResMgr::GetInst()->CreateTexture(L"NormalTargetTex"
			, (UINT)m_tResolution.fWidth, (UINT)m_tResolution.fHeight
			, DXGI_FORMAT_R32G32B32A32_FLOAT, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
			, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, arrRT[0].vClearColor);

		arrRT[2].vClearColor = Vector4(0.f, 0.f, 0.f, 0.f);
		arrRT[2].pTarget = CResMgr::GetInst()->CreateTexture(L"PositionTargetTex"
			, (UINT)m_tResolution.fWidth, (UINT)m_tResolution.fHeight
			, DXGI_FORMAT_R32G32B32A32_FLOAT, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
			, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, arrRT[0].vClearColor);


		m_arrMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Create(3, arrRT, pDSTex); // ���� �ؽ��Ĵ� SwapChain ���� ����Ѵ�.
	}

	// ============
	// Light MRT
	// ============
	{
		tRT arrRT[8] = {};

		arrRT[0].vClearColor = Vector4(0.f, 0.f, 0.f, 0.f);
		arrRT[0].pTarget = CResMgr::GetInst()->CreateTexture(L"DiffuseLightTargetTex"
			, (UINT)m_tResolution.fWidth, (UINT)m_tResolution.fHeight
			, DXGI_FORMAT_R8G8B8A8_UNORM, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
			, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, arrRT[0].vClearColor);

		arrRT[1].vClearColor = Vector4(0.f, 0.f, 0.f, 0.f);
		arrRT[1].pTarget = CResMgr::GetInst()->CreateTexture(L"SpecularLightTargetTex"
			, (UINT)m_tResolution.fWidth, (UINT)m_tResolution.fHeight
			, DXGI_FORMAT_R8G8B8A8_UNORM, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
			, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, arrRT[0].vClearColor);

		m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(2, arrRT, pDSTex); // ���� �ؽ��Ĵ� SwapChain ���� ����Ѵ�.
	}

	// ===============
	// SHADOWMAP MRT
	// ===============
	{
		tRT arrRT[8] = {};

		arrRT[0].vClearColor = Vector4(0.f, 0.f, 0.f, 0.f);
		arrRT[0].pTarget = CResMgr::GetInst()->CreateTexture(L"ShadowMapTargetTex"
			, 4096, 4096/*, 1024, 1024*/
			, DXGI_FORMAT_R32_FLOAT, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
			, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, arrRT[0].vClearColor);

		// ������ ���̹��۸� ����
		Ptr<CTexture> pDSTex = CResMgr::GetInst()->CreateTexture(L"ShadowMapDepthTex"
			, 4096, 4096
			, DXGI_FORMAT_D32_FLOAT, CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE
			, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

		m_arrMRT[(UINT)MRT_TYPE::SHADOWMAP] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SHADOWMAP]->Create(1, arrRT, pDSTex); // ������ ���̹��� �� ����
	}
}