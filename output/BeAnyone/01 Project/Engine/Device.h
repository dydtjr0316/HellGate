#pragma once

class CDevice
{
public:
	HWND			mhWnd;
	tResolution		mtResolution;
	bool			mbWindowed;

private:
	ComPtr<IDXGIFactory> mdxgiFactory;
	ComPtr<ID3D12Device> md3dDevice;

	ComPtr<ID3D12Fence> mFence;

	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;

	// SwapChain
	ComPtr<IDXGISwapChain> mSwapChain;

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;

	ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	ComPtr<ID3D12Resource> mDepthStencilBuffer;

	// View
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;


	// fence
	HANDLE	m_hFenceEvent;
	UINT64	mCurrentFence = 0;

public:
	int initDirect3D(HWND _hWnd, const tResolution& _res, bool _bWindow);
	void render_start(float(&_arrFloat)[4]);

	void render_present();
	void FlushCommandQueue();

private:	
	void CreateSwapChain();			// Swap Chain
	void CreateCommandObjects();	// Command
	void CreateRtvAndDsvDescriptorHeaps();	// DescriptorHeaps
	void CreateView();				// view
	

	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;
};

