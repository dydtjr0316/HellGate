#pragma once

class CConstantBuffer;

class CDevice
{
	SINGLE(CDevice);

public:
	HWND			m_hWnd;
	tResolution		m_tResolution;
	bool			m_bWindowed;

private:
	ComPtr<IDXGIFactory>				m_pFactory;
	ComPtr<ID3D12Device>				m_pDevice;
	ComPtr<ID3D12PipelineState>			m_pPipelineState;
	ComPtr<ID3D12Fence>					m_pFence;

	ComPtr<ID3D12CommandQueue>			m_pCommandQueue;
	ComPtr<ID3D12CommandAllocator>		m_pCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList>	m_pCommandList;

	// SwapChain
	static const int					SwapChainBufferCount = 2;
	int									m_iCurTargetIdx = 0;
	
	ComPtr<IDXGISwapChain>				m_pSwapChain;
	
	ComPtr<ID3D12Resource>				m_pSwapChainBuffer[SwapChainBufferCount];
	ComPtr<ID3D12Resource>				m_pDepthStencilBuffer;

	// View
	ComPtr<ID3D12DescriptorHeap>		m_pRtvHeap;
	ComPtr<ID3D12DescriptorHeap>		m_pDsvHeap;
	ComPtr<ID3D12DescriptorHeap>		m_pDummyCbvHeap;	// 2·Î?

	UINT m_iRtvDescriptorSize = 0;
	UINT m_iDsvDescriptorSize = 0;
	UINT m_iCbvSrvUavDescriptorSize = 0;

	D3D12_VIEWPORT						m_tScreenViewport;
	D3D12_RECT							m_tScissorRect;

	// fence
	HANDLE								m_hFenceEvent;
	UINT64								m_iFenceValue = 0;

	vector<CConstantBuffer*>			m_vecCB;

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
	void CreateCommandObjects();	// Command
	void CreateSwapChain();			// Swap Chain
	void CreateRtvAndDsvDescriptorHeaps();	// DescriptorHeaps
	void CreateView();				// View
	void CreateViewPort();			// ViewPort
	void CreateRootSignature();

	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;

public:
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return m_pCommandList; }
	ComPtr<ID3D12Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D12RootSignature> GetRootSignature(ROOT_SIG_TYPE _eType) { return m_arrSig[(UINT)_eType]; }
	CConstantBuffer* GetCB(CONST_REGISTER _eRegister) { return m_vecCB[(UINT)_eRegister]; }
};

