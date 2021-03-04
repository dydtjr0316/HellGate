#pragma once
#include "Resource.h"


class CMesh :
	public CResource
{
private:
	ComPtr<ID3D12Resource>		m_pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView;		// 버텍스 버퍼 정보 구조체(stride, total size, 가상 GPU 주소)
	
	ComPtr<ID3D12Resource>		m_pIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW		m_tIdxView;		// 인덱스 버퍼 정보 구조체

	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	void*						m_pVtxSysMem;

	DXGI_FORMAT					m_eIdxFormat;
	UINT						m_iIdxCount;
	void*						m_pIdxSysMem;

public:
	void Create(UINT _iVtxSize, UINT _iVtxCount, BYTE* _pVtxSysMem
		, DXGI_FORMAT _iIdxFormat, UINT _iIdxCount, BYTE* _pIdxSysMem);
	void render();

public:
	CMesh();
	virtual ~CMesh();
};

