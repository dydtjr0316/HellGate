#pragma once

class CConstantBuffer;

class CDevice
{
	SINGLE(CDevice);

public:
	HWND			mhWnd;
	tResolution		mtResolution;
	bool			mbWindowed;

private:
	ComPtr<IDXGIFactory>				mdxgiFactory;
	ComPtr<ID3D12Device>				md3dDevice;
	ComPtr<ID3D12PipelineState>			mPipelineState;
	ComPtr<ID3D12Fence>					mFence;

	ComPtr<ID3D12CommandQueue>			mCommandQueue;
	ComPtr<ID3D12CommandAllocator>		mDirectCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList>	mCommandList;

	// SwapChain
	static const int					SwapChainBufferCount = 2;
	int									miCurTargetIdx = 0;
	
	ComPtr<IDXGISwapChain>				mSwapChain;
	
	ComPtr<ID3D12Resource>				mSwapChainBuffer[SwapChainBufferCount];
	ComPtr<ID3D12Resource>				mDepthStencilBuffer;

	// View
	ComPtr<ID3D12DescriptorHeap>		mRtvHeap;
	ComPtr<ID3D12DescriptorHeap>		mDsvHeap;
	ComPtr<ID3D12DescriptorHeap>		mDummyCvbHeap;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	D3D12_VIEWPORT						mScreenViewport;
	D3D12_RECT							mScissorRect;

	// fence
	HANDLE								m_hFenceEvent;
	UINT64								mCurrentFence = 0;

	vector<CConstantBuffer*>			mVecCB;

private:
	ComPtr<ID3D12RootSignature>	m_arrSig[(UINT)ROOT_SIG_TYPE::END];

public:
	int initDirect3D(HWND _hWnd, const tResolution& _res, bool _bWindow);
	void render_start(float(&_arrFloat)[4]);

	void render_present();
	void FlushCommandQueue();

	void SetConstBufferToRegister(CConstantBuffer* _pCB, UINT _iOffset);

	void CreateConstBuffer(const wstring& _strName, size_t _iBufferOffset,
		size_t _iMaxCount, CONST_REGISTER _eRegisterNum, bool _bGlobal = false);

private:
	void CreateSwapChain();			// Swap Chain
	void CreateCommandObjects();	// Command
	void CreateRtvAndDsvDescriptorHeaps();	// DescriptorHeaps
	void CreateView();				// View
	void CreateViewPort();			// ViewPort
	void CreateRootSignature();

	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;

public:
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return mCommandList; }
	ComPtr<ID3D12Device> GetDevice() { return md3dDevice; }
};

