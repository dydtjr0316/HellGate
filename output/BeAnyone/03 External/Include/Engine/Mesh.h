#pragma once
#include "Resource.h"


class CMesh :
	public CResource
{
private:
	ComPtr<ID3D12Resource>		m_pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView;		// ���ؽ� ���� ���� ����ü(stride, total size, ���� GPU �ּ�)

	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	
	void*						m_pVtxSysmem;

public:
	void Create(UINT _iBuffuerSize, BYTE* _pVtxSysMem);
	void render();

public:
	CMesh();
	virtual ~CMesh();
};

