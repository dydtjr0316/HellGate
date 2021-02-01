#include "pch.h"
#include "Device.h"

#include "ConstantBuffer.h"


CDevice::CDevice()
{
}

CDevice::~CDevice()
{
	FlushCommandQueue();
}

int CDevice::initDirect3D(HWND _hWnd, const tResolution& _res, bool _bWindow)
{
	mhWnd = _hWnd;
	mtResolution = _res;
	mbWindowed = _bWindow;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
	debugController->EnableDebugLayer();
#endif
	CreateDXGIFactory(IID_PPV_ARGS(&mdxgiFactory));

	// 디바이스 생성
	D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&md3dDevice));

	// 펜스 생성
	md3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));

	// Create an event handle to use for frame synchronization.
	m_hFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_hFenceEvent == nullptr)
	{
		assert(nullptr);
	}

	// 힙 크기 할당
	mRtvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDsvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCbvSrvUavDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CreateCommandObjects();
	CreateSwapChain();
	CreateRtvAndDsvDescriptorHeaps();
	CreateView();
	CreateViewPort();
	CreateRootSignature();

	// 루트 서명 해야 함


	return S_OK;
}

void CDevice::CreateCommandObjects()
{
	// Create Command Queue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	md3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue));

	// Create Command Allocator
	md3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf()));

	// Create Command List
	md3dDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		mDirectCmdListAlloc.Get(),	// Associated command allocator
		nullptr,					// Initial PipelineStateObject
		IID_PPV_ARGS(mCommandList.GetAddressOf()));

	mCommandList->Close();

}

void CDevice::CreateSwapChain()
{
	// Release the previous swapChain we will be recreating
	mSwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC tDesc;
	tDesc.BufferDesc.Width = (UINT)mtResolution.fWidth;			// 버퍼의 해상도(윈도우 해상도랑 일치시켜놓음)
	tDesc.BufferDesc.Height = (UINT)mtResolution.fHeight;		// 버퍼의 해상도(윈도우 해상도랑 일치시켜놓음)
	tDesc.BufferDesc.RefreshRate.Numerator = 100;				// 화면 갱신 비율
	tDesc.BufferDesc.RefreshRate.Denominator = 1;				// 화면 갱신 비율
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 버퍼의 픽셀 포멧(픽셀당 4바이트)
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;		// 픽셀의 스캔라인 순서
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;	// 윈도우와 해상도 불일치시 화면 스케일링을 할것인지
	tDesc.SampleDesc.Count = 1;									// 멀티 샘플링 사용 안 함
	tDesc.SampleDesc.Quality = 0;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// 출력 타겟 용도로 버퍼를 만든다.
	tDesc.BufferCount = SwapChainBufferCount;

	tDesc.OutputWindow = mhWnd;			// 출력 윈도우
	tDesc.Windowed = mbWindowed;		// 창 모드 or 전체화면 모드
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;			// 전면 후면 버퍼 교체 시 이전 프레임 정보 버림
	tDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = mdxgiFactory->CreateSwapChain(mCommandQueue.Get(), &tDesc, mSwapChain.GetAddressOf());
}

void CDevice::CreateRtvAndDsvDescriptorHeaps()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = SwapChainBufferCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	md3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf()));

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	md3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf()));
}

void CDevice::CreateView()
{
	miCurTargetIdx = 0;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();

	// Create a RTV for each frame.
	for (UINT i = 0; i < SwapChainBufferCount; ++i)
	{
		// 생성된 SwapChain 에서 버퍼를 가져옴
		mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mSwapChainBuffer[i]));

		// 해당 버퍼로 RenderTarvetView 생성함
		md3dDevice->CreateRenderTargetView(mSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.ptr += mRtvDescriptorSize; // Offset 증가
	}

	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = (UINT)mtResolution.fWidth;;
	depthStencilDesc.Height = (UINT)mtResolution.fHeight;;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;

	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	D3D12_HEAP_PROPERTIES HeapProperties;
	HeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
	HeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	HeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	HeapProperties.CreationNodeMask = 1;
	HeapProperties.VisibleNodeMask = 1;

	md3dDevice->CreateCommittedResource(
		&HeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf()));

	
	// d3dx12.h 헤더가 포함되어야 CD3DX12_~ 를 쓸 수 있음
	/*md3dDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf()));*/

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.Texture2D.MipSlice = 0;

	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, DepthStencilView());

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = mDepthStencilBuffer.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	
	// Transition the resource from its initial state to be used as a depth buffer.
	mCommandList->ResourceBarrier(1, &barrier);

	// d3dx12.h 헤더가 포함되어야 CD3DX12_~ 를 쓸 수 있음
	/*mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));*/

	// Execute the resize commands.
	mCommandList->Close();

	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	FlushCommandQueue();
}

void CDevice::FlushCommandQueue()
{
	// Advance the fence value to mark commands up to this fence point.
	mCurrentFence++;

	mCommandQueue->Signal(mFence.Get(), mCurrentFence);

	if (mFence->GetCompletedValue() < mCurrentFence)
	{
		// init에 만듦
		/*HANDLE eventHandle = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
		if (eventHandle == nullptr)
		{
			assert(nullptr);
		}*/

		// Fire event when GPU hits current fence.  
		mFence->SetEventOnCompletion(mCurrentFence, m_hFenceEvent);

		// Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(m_hFenceEvent, INFINITE);
		CloseHandle(m_hFenceEvent);
	}
}

void CDevice::render_start(float(&_arrFloat)[4])
{
	// 그리기 준비
	mDirectCmdListAlloc->Reset();

	mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr);

	// 필요한 상태 설정	
	mCommandList->RSSetViewports(1, &mScreenViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	// Indicate that the back buffer will be used as a render target.
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = mSwapChainBuffer[miCurTargetIdx].Get();	// miCurTargetIdx로 현재 백버퍼 가져오기
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;		// 출력에서
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // 다시 백버퍼로 지정
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	mCommandList->ResourceBarrier(1, &barrier);

	// const float color[4] = { 1.0, 0.7, 0.1, 0.0 };
	// mCommandList->ClearRenderTargetView(CurrentBackBufferView(), color, 0, nullptr);

	// Clear the back buffer and depth buffer.
	// directxcolors.h 없음
	// mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::Honeydew, 0, nullptr);
	mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to. => 파이프라인에 묶기?
	mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

}

void CDevice::render_present()
{
	// Indicate that the back buffer will now be used to present.
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE; ;
	barrier.Transition.pResource = mSwapChainBuffer[miCurTargetIdx].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;	// 백버퍼에서
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;			// 다시 출력으로 지정

	mCommandList->ResourceBarrier(1, &barrier);

	// Done recording commands.
	mCommandList->Close();

	// 커맨드 리스트 수행	(Add the command list to the queue for execution)
	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Present the frame.
	mSwapChain->Present(0, 0);
	FlushCommandQueue();

	miCurTargetIdx = (miCurTargetIdx + 1) % SwapChainBufferCount;
}

D3D12_CPU_DESCRIPTOR_HANDLE CDevice::DepthStencilView() const
{
	return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}

D3D12_CPU_DESCRIPTOR_HANDLE CDevice::CurrentBackBufferView() const
{
	// miCurTargetIdx가 0이라면 0 * size = 0이니까 아무것도 안 더함 -> 핸들의 첫 시작
	// miCurtargetIdx가 1이라면 1 * size = size니까 size만큼 더함 -> 핸들의 처음 + 사이즈 -> 즉, 두 번째 뷰
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = mRtvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHeapHandle.ptr += miCurTargetIdx * mRtvDescriptorSize;
	return rtvHeapHandle;
}

void CDevice::CreateViewPort()
{
	mScreenViewport = D3D12_VIEWPORT{ 0.f, 0.f, mtResolution.fWidth, mtResolution.fHeight, 0.f, 1.f };
	mScissorRect = D3D12_RECT{ 0, 0, (LONG)mtResolution.fWidth, (LONG)mtResolution.fHeight };
}

void CDevice::CreateRootSignature()
{
	D3D12_DESCRIPTOR_RANGE range[2] = {};
	D3D12_ROOT_PARAMETER sigParam[2] = {};

	range[0].BaseShaderRegister = 0;
	range[0].NumDescriptors = 2;
	range[0].OffsetInDescriptorsFromTableStart = -1;
	range[0].RegisterSpace = 0;
	range[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV; // b

	sigParam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	sigParam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	sigParam[0].DescriptorTable.NumDescriptorRanges = 1;
	sigParam[0].DescriptorTable.pDescriptorRanges = range;

	//루트 서명
	D3D12_ROOT_SIGNATURE_DESC sigDesc = {};
	sigDesc.NumParameters = 1;
	sigDesc.pParameters = sigParam;	// Descriptor Table 0번 슬롯 설명
	sigDesc.NumStaticSamplers = 0;
	sigDesc.pStaticSamplers = nullptr; // 사용될 Sampler 정보
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> pSignature;
	ComPtr<ID3DBlob> pError;
	HRESULT hr = D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pSignature, &pError);
	md3dDevice->CreateRootSignature(0, pSignature->GetBufferPointer(), pSignature->GetBufferSize(), IID_PPV_ARGS(&m_arrSig[(UINT)ROOT_SIG_TYPE::INPUT_ASSEM]));

	// 더미용 DescriptorHeap 만들기
	D3D12_DESCRIPTOR_HEAP_DESC dummyCbvHeapDesc = {};
	dummyCbvHeapDesc.NumDescriptors = 2;
	dummyCbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	dummyCbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	md3dDevice->CreateDescriptorHeap(&dummyCbvHeapDesc, IID_PPV_ARGS(&m_pDummyCbvHeap));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	D3D12_CPU_DESCRIPTOR_HANDLE handle = m_pDummyCbvHeap->GetCPUDescriptorHandleForHeapStart();
	md3dDevice->CreateConstantBufferView(&cbvDesc, handle);
}

void CDevice::SetConstBufferToRegister(CConstantBuffer* _pCB, UINT _iOffset)
{
	UINT iDestRange = 1;
	UINT iSrcRange = 1;

	// 0번 슬롯이 상수 버퍼 데이터

	D3D12_CPU_DESCRIPTOR_HANDLE hDummyHandle = m_pDummyCbvHeap->GetCPUDescriptorHandleForHeapStart();
	hDummyHandle.ptr += _pCB->GetRegisterNum() * mCbvSrvUavDescriptorSize;

	// 상수 버퍼 512개 중 어떤 걸 넣을 거냐
	D3D12_CPU_DESCRIPTOR_HANDLE hSrcHandle = _pCB->GetCBV()->GetCPUDescriptorHandleForHeapStart();
	hSrcHandle.ptr += _iOffset * mCbvSrvUavDescriptorSize;

	md3dDevice->CopyDescriptors(1, &hDummyHandle, &iDestRange,
		1, &hSrcHandle, &iSrcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 더미 힙을 레지스터에 등록
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = m_pDummyCbvHeap->GetGPUDescriptorHandleForHeapStart();
	mCommandList->SetGraphicsRootDescriptorTable(0, gpuHandle);
}

void CDevice::CreateConstBuffer(const wstring& _strName, size_t _iSize,
	size_t _iMaxCount, CONST_REGISTER _eRegisterNum, bool _bGlobal)
{
	CConstantBuffer* pCB = new CConstantBuffer;

	pCB->Create((UINT)_iSize, (UINT)_iMaxCount, _eRegisterNum);
	
	if (m_vecCB.size() <= (UINT)_eRegisterNum)
	{
		m_vecCB.resize((UINT)_eRegisterNum + 1);
	}

	m_vecCB[(UINT)_eRegisterNum] = pCB;
}

