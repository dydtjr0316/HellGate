#include "pch.h"
#include "Device.h"

#include "ConstantBuffer.h"
#include "Texture.h"

#include "ResMgr.h"
#include "RenderMgr.h"
#include "MRT.h"

CDevice::CDevice()
	: m_pDevice(nullptr)
	, m_pFence(nullptr)
	, m_pFactory(nullptr)
	, m_iCurTargetIdx(0)
	, m_hFenceEvent(nullptr)
	, m_iCurDummyIdx(0)
	, m_iFenceValue(0)
{
}

CDevice::~CDevice()
{
	FlushCommandQueue();
	CloseHandle(m_hFenceEvent);

	for (size_t i = 0; i < m_vecCB.size(); ++i)
	{
		SAFE_DELETE(m_vecCB[i]);
	}
}

int CDevice::initDirect3D(HWND _hWnd, const tResolution& _res, bool _bWindow)
{
	m_hWnd = _hWnd;
	m_tResolution = _res;
	m_bWindowed = _bWindow;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
	debugController->EnableDebugLayer();
#endif
	CreateDXGIFactory(IID_PPV_ARGS(&m_pFactory));

	// ����̽� ����
	D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_pDevice));

	// �潺 ����
	m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));
	m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFenceCompute));
	m_iFenceValue = 1;

	// Create an event handle to use for frame synchronization.
	m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_hFenceEvent == nullptr)
	{
		assert(nullptr);
	}
	
	CreateCommandObjects();
	CreateSwapChain();
	CreateViewPort();
	CreateRootSignature();


	return S_OK;
}

void CDevice::render_start(float(&_arrFloat)[4])
{
	m_iCurDummyIdx = 0;

	// �׸��� �غ�
	m_pCmdListAlloc->Reset();
	m_pCommandList->Reset(m_pCmdListAlloc.Get(), nullptr);

	// �ʿ��� ���� ����	
	// RootSignature ����	
	CMDLIST->SetGraphicsRootSignature(CDevice::GetInst()->GetRootSignature(ROOT_SIG_TYPE::RENDER).Get());

	m_pCommandList->RSSetViewports(1, &m_tScreenViewport);
	m_pCommandList->RSSetScissorRects(1, &m_tScissorRect);

	CMRT* pSwapChainMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);

	// Indicate that the back buffer will be used as a render target.
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = pSwapChainMRT->GetRTTex(m_iCurTargetIdx)->GetTex2D().Get();	// miCurTargetIdx�� ���� ����� ��������
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		// ��¿���
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �ٽ� ����۷� ����
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	m_pCommandList->ResourceBarrier(1, &barrier);

	// ù ��° ���� Descriptor Heap �ʱ�ȭ
	ClearDummyDescriptorHeap(0);
}

void CDevice::render_present()
{
	CMRT* pSwapChainMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);

	// Indicate that the back buffer will now be used to present.
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE; ;
	barrier.Transition.pResource = pSwapChainMRT->GetRTTex(m_iCurTargetIdx)->GetTex2D().Get();// m_arrRenderTargets[m_iCurTargetIdx]->GetTex2D().Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// ����ۿ���
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// �ٽ� ������� ����

	m_pCommandList->ResourceBarrier(1, &barrier);

	// Done recording commands.
	m_pCommandList->Close();

	// Ŀ�ǵ� ����Ʈ ����	(Add the command list to the queue for execution)
	ID3D12CommandList* cmdsLists[] = { m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Present the frame.
	m_pSwapChain->Present(0, 0);
	FlushCommandQueue();

	// ������� ������ �ʱ�ȭ
	for (size_t i = 0; i < m_vecCB.size(); ++i)
	{
		m_vecCB[i]->Clear();
	}

	m_iCurTargetIdx = (m_iCurTargetIdx + 1) % SwapChainBufferCount;
}

void CDevice::FlushCommandQueue()
{
	// Signal and increment the fence value.
	const UINT64 fence = m_iFenceValue;
	m_pCommandQueue->Signal(m_pFence.Get(), fence);
	m_iFenceValue++;

	// Wait until the previous frame is finished.
	if (m_pFence->GetCompletedValue() < fence)
	{
		// Fire event when GPU hits current fence.  
		m_pFence->SetEventOnCompletion(fence, m_hFenceEvent);

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(m_hFenceEvent, INFINITE);

	}
}

void CDevice::FlushCommandQueue_CS()
{
	// Signal and increment the fence value.
	static int iFenceValue = 0;

	const size_t fence = iFenceValue;
	m_pCommandQueueCompute->Signal(m_pFenceCompute.Get(), fence);
	iFenceValue++;

	size_t a = m_pFenceCompute->GetCompletedValue();
	// Wait until the previous frame is finished.
	if (a < fence)
	{
		m_pFenceCompute->SetEventOnCompletion(fence, m_hFenceEvent);
		WaitForSingleObject(m_hFenceEvent, INFINITE);
	}
}


void CDevice::CreateCommandObjects()
{
	// Create Command Queue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));

	// Create Compute Command Queue
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueueCompute));

	// Create Command Allocator
	m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdListAlloc));
	m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdListAllocRes));
	m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&m_pCmdListAllocCompute));

	// Create Command List
	m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCmdListAlloc.Get(),	// Associated command allocator
		nullptr,					// Initial PipelineStateObject
		IID_PPV_ARGS(m_pCommandList.GetAddressOf()));

	m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCmdListAllocRes.Get(),	// Associated command allocator
		nullptr,					// Initial PipelineStateObject
		IID_PPV_ARGS(m_pCommandListRes.GetAddressOf()));

	m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_COMPUTE,
		m_pCmdListAllocCompute.Get(),	// Associated command allocator
		nullptr,					// Initial PipelineStateObject
		IID_PPV_ARGS(m_pCommandListCompute.GetAddressOf()));


	m_pCommandList->Close();

}

void CDevice::CreateSwapChain()
{
	// Release the previous swapChain we will be recreating
	m_pSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferCount = SwapChainBufferCount;

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// ������ �ȼ� ����(�ȼ��� 4����Ʈ)
	tDesc.BufferDesc.Width = (UINT)m_tResolution.fWidth;		// ������ �ػ�(������ �ػ󵵶� ��ġ���ѳ���)
	tDesc.BufferDesc.Height = (UINT)m_tResolution.fHeight;		// ������ �ػ�(������ �ػ󵵶� ��ġ���ѳ���)
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	// ������� �ػ� ����ġ�� ȭ�� �����ϸ��� �Ұ�����
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// �ȼ��� ��ĵ���� ����
	tDesc.BufferDesc.RefreshRate.Numerator = 100;				// ȭ�� ���� ����
	tDesc.BufferDesc.RefreshRate.Denominator = 1;				// ȭ�� ���� ����
	
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// ��� Ÿ�� �뵵�� ���۸� �����.
	tDesc.Flags = 0;//DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	tDesc.OutputWindow = m_hWnd;			// ��� ������
	tDesc.Windowed = m_bWindowed;		// â ��� or ��üȭ�� ���
	tDesc.SampleDesc.Count = 1;									// ��Ƽ ���ø� ��� �� ��
	tDesc.SampleDesc.Quality = 0;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			// ���� �ĸ� ���� ��ü �� ���� ������ ���� ����
	
	HRESULT hr = m_pFactory->CreateSwapChain(m_pCommandQueue.Get(), &tDesc, m_pSwapChain.GetAddressOf());
}

void CDevice::CreateViewPort()
{
	m_tScreenViewport = D3D12_VIEWPORT{ 0.f, 0.f, m_tResolution.fWidth, m_tResolution.fHeight, 0.f, 1.f };
	m_tScissorRect = D3D12_RECT{ 0, 0, (LONG)m_tResolution.fWidth, (LONG)m_tResolution.fHeight };
}

void CDevice::CreateRootSignature()
{
	m_iCbvSrvUavDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ���� �� Descriptor Table�� �ۼ��Ѵ�
	// 0�� ����
	vector<D3D12_DESCRIPTOR_RANGE> vecRange;

	D3D12_DESCRIPTOR_RANGE range = {};
	D3D12_ROOT_PARAMETER slotParam = {};

	vecRange.clear();

	// ��� ��������
	range.BaseShaderRegister = 0;		// b0����
	range.NumDescriptors = 5;			// b4���� 5�� ��� �������� ��� ����
	range.OffsetInDescriptorsFromTableStart = -1;
	range.RegisterSpace = 0;
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // b
	vecRange.push_back(range);

	// �ؽ�ó ��������
	range = {};
	range.BaseShaderRegister = 0;		// t0����
	range.NumDescriptors = 13;			// t12���� 13�� �ؽ�ó �������� ��� ����
	range.OffsetInDescriptorsFromTableStart = 5;
	range.RegisterSpace = 0;
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // t
	vecRange.push_back(range);

	slotParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	slotParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	slotParam.DescriptorTable.NumDescriptorRanges = vecRange.size();
	slotParam.DescriptorTable.pDescriptorRanges = &vecRange[0];

	// Sampler Desc �����
	CreateSamplerDesc();

	//��Ʈ ����
	D3D12_ROOT_SIGNATURE_DESC sigDesc = {};
	sigDesc.NumParameters = 1;
	sigDesc.pParameters = &slotParam;	// Descriptor Table 0�� ���� ����
	sigDesc.NumStaticSamplers = 2;		// m_vecSamplerDesc.size(); -> ��� �� �ϴ� �ֵ�����?
	sigDesc.pStaticSamplers = &m_vecSamplerDesc[0]; // ���� Sampler ����
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // �Է� ������ �ܰ�

	ComPtr<ID3DBlob> pSignature;
	ComPtr<ID3DBlob> pError;
	HRESULT hr = D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pError);
	m_pDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_arrSig[(UINT)ROOT_SIG_TYPE::RENDER]));

	// ���̿� DescriptorHeap �����
	D3D12_DESCRIPTOR_HEAP_DESC dummyCbvHeapDesc = {};

	UINT iDescriptorNum = 0;
	for (size_t i = 0; i < vecRange.size(); ++i) {
		iDescriptorNum += vecRange[i].NumDescriptors;
	}

	dummyCbvHeapDesc.NumDescriptors = iDescriptorNum;
	dummyCbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dummyCbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	for (int i = 0; i < 512; ++i) {
		ComPtr<ID3D12DescriptorHeap> pDummyDescriptor;
		m_pDevice->CreateDescriptorHeap(&dummyCbvHeapDesc, IID_PPV_ARGS(&pDummyDescriptor));
		m_vecDummyDescriptor.push_back(pDummyDescriptor);
	}

	// �ʱ�ȭ�� ���� ��ũ���� �� �ۼ�
	dummyCbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	m_pDevice->CreateDescriptorHeap(&dummyCbvHeapDesc, IID_PPV_ARGS(&m_pInitDescriptor));

	// ====================================
	// Compute Shader ���� Signature �����
	// ====================================

	range = {};
	range.BaseShaderRegister = 0;  // u0 ����
	range.NumDescriptors = 4;	   // u3 ���� 4 �� UAV �������� ��뿩�� 
	range.OffsetInDescriptorsFromTableStart = (UINT)TEXTURE_REGISTER::END;
	range.RegisterSpace = 0;
	range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;

	vecRange.push_back(range);

	slotParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	slotParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	slotParam.DescriptorTable.NumDescriptorRanges = (UINT)vecRange.size();
	slotParam.DescriptorTable.pDescriptorRanges = &vecRange[0];

	sigDesc = {};
	sigDesc.NumParameters = 1;
	sigDesc.pParameters = &slotParam;
	sigDesc.NumStaticSamplers = 0;
	sigDesc.pStaticSamplers = nullptr; // ��ǻƮ ���̴������� Sampler ��� �Ұ���
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE; // ��ǻƮ ���̴��� Signature �� �⺻ �÷��� ������ ����

	pSignature = nullptr;
	pError = nullptr;

	hr = D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pError);
	m_pDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize()
		, IID_PPV_ARGS(&m_arrSig[(UINT)ROOT_SIG_TYPE::COMPUTE]));

	// ������ ��ǻƮ ���̴��� ��Ʈ�ñ׳����ĸ� ��ǻƮ ���̴� Ŀ�ǵ� ����Ʈ�� ���
	CMDLIST_CS->SetComputeRootSignature(CDevice::GetInst()->GetRootSignature(ROOT_SIG_TYPE::COMPUTE).Get());

	// ComputeShader ���� ��ũ���� �� �ۼ�	
	dummyCbvHeapDesc.NumDescriptors += range.NumDescriptors; // UAV ��ŭ �߰�
	dummyCbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dummyCbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_pDevice->CreateDescriptorHeap(&dummyCbvHeapDesc, IID_PPV_ARGS(&m_pDummyDescriptorCompute));

}

void CDevice::CreateSamplerDesc()
{
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_ANISOTROPIC;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 4;	// 0���� �ٲ� ����
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS; // D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE; // D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = 0.f; // D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	m_vecSamplerDesc.push_back(sampler);

	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS; // D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 1;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	m_vecSamplerDesc.push_back(sampler);
}

void CDevice::CreateConstBuffer(const wstring& _strName, size_t _iSize,
	size_t _iMaxCount, CONST_REGISTER _eRegisterNum, bool _bGlobal)
{
	CConstantBuffer* pCB = new CConstantBuffer;
	pCB->SetName(_strName);
	pCB->Create((UINT)_iSize, (UINT)_iMaxCount, _eRegisterNum);
	m_vecCB.push_back(pCB);
	
	if (_bGlobal)
	{
		SetGlobalConstBufferToRegister(pCB, 0);
	}
}

void CDevice::SetConstBufferToRegister(CConstantBuffer* _pCB, UINT _iOffset)
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0�� ������ ��� ���� ������
	D3D12_CPU_DESCRIPTOR_HANDLE hDummyHandle = m_vecDummyDescriptor[m_iCurDummyIdx]->GetCPUDescriptorHandleForHeapStart();
	hDummyHandle.ptr += (UINT)_pCB->GetRegisterNum() * m_iCbvSrvUavDescriptorSize;

	// ��� ���� 512�� �� � �� ���� �ų�
	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = _pCB->GetCBV()->GetCPUDescriptorHandleForHeapStart();
	hSrcHandle.ptr += _iOffset * m_iCbvSrvUavDescriptorSize;

	m_pDevice->CopyDescriptors(1, &hDummyHandle, &iDestRange,
		1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CDevice::SetGlobalConstBufferToRegister(CConstantBuffer* _pCB, UINT _iOffset)
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0�� ������ ������� ������
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_pInitDescriptor->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr += m_iCbvSrvUavDescriptorSize * (UINT)_pCB->GetRegisterNum();

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = _pCB->GetCBV()->GetCPUDescriptorHandleForHeapStart();
	hSrcHandle.ptr += _iOffset * m_iCbvSrvUavDescriptorSize;

	m_pDevice->CopyDescriptors(1, &hDescHandle, &iDestRange
		, 1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}


void CDevice::SetTextureToRegister(CTexture* _pTex, TEXTURE_REGISTER _eRegisterNum)
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0�� ������ ��� ���� ������
	D3D12_CPU_DESCRIPTOR_HANDLE hDummyHandle = m_vecDummyDescriptor[m_iCurDummyIdx]->GetCPUDescriptorHandleForHeapStart();
	hDummyHandle.ptr += (UINT)_eRegisterNum * m_iCbvSrvUavDescriptorSize;

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = _pTex->GetSRV()->GetCPUDescriptorHandleForHeapStart();

	m_pDevice->CopyDescriptors(1, &hDummyHandle, &iDestRange
		, 1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CDevice::SetConstBufferToRegister_CS(CConstantBuffer* _pCB, UINT _iOffset)
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0�� ������ ������� ������
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_pDummyDescriptorCompute->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr += m_iCbvSrvUavDescriptorSize * (UINT)_pCB->GetRegisterNum();

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = _pCB->GetCBV()->GetCPUDescriptorHandleForHeapStart();
	hSrcHandle.ptr += _iOffset * m_iCbvSrvUavDescriptorSize;

	m_pDevice->CopyDescriptors(1, &hDescHandle, &iDestRange
		, 1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CDevice::SetTextureToRegister_CS(CTexture* _pTex, TEXTURE_REGISTER _eRegister)
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0�� ������ ������� ������
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_pDummyDescriptorCompute->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr += m_iCbvSrvUavDescriptorSize * (UINT)_eRegister;

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = _pTex->GetSRV()->GetCPUDescriptorHandleForHeapStart();

	m_pDevice->CopyDescriptors(1, &hDescHandle, &iDestRange
		, 1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CDevice::SetUAVToRegister_CS(CTexture* _pTex, UAV_REGISTER _eRegister)
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0�� ������ ������� ������
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_pDummyDescriptorCompute->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr += m_iCbvSrvUavDescriptorSize * (UINT)_eRegister;

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = _pTex->GetUAV()->GetCPUDescriptorHandleForHeapStart();

	m_pDevice->CopyDescriptors(1, &hDescHandle, &iDestRange
		, 1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ���ҽ� ���� ����
	if (_pTex->GetResState() == D3D12_RESOURCE_STATE_COMMON)
	{
		CMDLIST_CS->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(_pTex->GetTex2D().Get()
			, D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

		_pTex->SetResState(D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}
}

void CDevice::UpdateTable()
{
	ID3D12DescriptorHeap* pDescriptor = m_vecDummyDescriptor[m_iCurDummyIdx].Get();
	m_pCommandList->SetDescriptorHeaps(1, &pDescriptor);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle = pDescriptor->GetGPUDescriptorHandleForHeapStart();
	m_pCommandList->SetGraphicsRootDescriptorTable(0, gpuhandle);

	// ���� ���� Descriptor Heap �� ����Ű�� �ε����� ������Ų��.
	++m_iCurDummyIdx;

	// ���� ���� Descriptor Heap�� �ʱ�ȭ�Ѵ� (���� ��� ���۴� ���´�)
	ClearDummyDescriptorHeap(m_iCurDummyIdx);
}


void CDevice::UpdateTable_CS()
{
	ID3D12DescriptorHeap* pDescriptor = m_pDummyDescriptorCompute.Get();
	m_pCommandListCompute->SetDescriptorHeaps(1, &pDescriptor);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuhandle = pDescriptor->GetGPUDescriptorHandleForHeapStart();
	m_pCommandListCompute->SetComputeRootDescriptorTable(0, gpuhandle);
}


void CDevice::ClearDummyDescriptorHeap(UINT _iDummyIndex)
{
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_vecDummyDescriptor[_iDummyIndex]->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr;

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = m_pInitDescriptor->GetCPUDescriptorHandleForHeapStart();
	hSrcHandle.ptr;

	UINT iDestRange = (UINT)TEXTURE_REGISTER::END;
	UINT iSrcRange = (UINT)TEXTURE_REGISTER::END;

	m_pDevice->CopyDescriptors(1/*��ũ���� ����*/
		, &hDescHandle, &iDestRange
		, 1/*��ũ���� ����*/
		, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CDevice::ClearDymmyDescriptorHeap_CS()
{
	D3D12_CPU_DESCRIPTOR_HANDLE hDescHandle = m_pDummyDescriptorCompute->GetCPUDescriptorHandleForHeapStart();
	hDescHandle.ptr;

	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = m_pInitDescriptor->GetCPUDescriptorHandleForHeapStart();
	hSrcHandle.ptr;

	UINT iDestRange = (UINT)TEXTURE_REGISTER::END;
	UINT iSrcRange = (UINT)TEXTURE_REGISTER::END;

	m_pDevice->CopyDescriptors(1/*��ũ���� ����*/
		, &hDescHandle, &iDestRange
		, 1/*��ũ���� ����*/
		, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void CDevice::ExcuteResourceLoad()
{
	// ���ҽ� �ε� ��� �ݱ�
	m_pCommandListRes->Close();

	// Ŀ�ǵ� ����Ʈ ����
	ID3D12CommandList* ppCommandLists[] = { m_pCommandListRes.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	FlushCommandQueue();

	// �ٽ� Ȱ��ȭ
	m_pCmdListAllocRes->Reset();
	m_pCommandListRes->Reset(m_pCmdListAllocRes.Get(), nullptr);
}


void CDevice::ExcuteComputeShader()
{
	// ��ǻƮ ���̴� ��� �ݱ�
	m_pCommandListCompute->Close();

	// Ŀ�ǵ� ����Ʈ ����	
	ID3D12CommandList* ppCommandLists[] = { m_pCommandListCompute.Get() };
	m_pCommandQueueCompute->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	FlushCommandQueue_CS();

	// �ٽ� Ȱ��ȭ
	m_pCmdListAllocCompute->Reset();
	m_pCommandListCompute->Reset(m_pCmdListAllocCompute.Get(), nullptr);

	// ��Ʈ���� ���
	m_pCommandListCompute->SetComputeRootSignature(CDevice::GetInst()->GetRootSignature(ROOT_SIG_TYPE::COMPUTE).Get());
}
