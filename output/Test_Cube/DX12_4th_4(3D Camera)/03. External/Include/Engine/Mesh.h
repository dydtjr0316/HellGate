#pragma once
#include "Resource.h"

class CMesh :
	public CResource
{
private:
	ComPtr<ID3D12Resource>		m_pVB;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView; // ���ؽ� ���� ���� ����ü(stride, total size, ���� GPU �ּ�)

	ComPtr<ID3D12Resource>		m_pIB;
	D3D12_INDEX_BUFFER_VIEW		m_tIdxView; // ���ؽ� ���� ���� ����ü(stride, total size, ���� GPU �ּ�)
	
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

