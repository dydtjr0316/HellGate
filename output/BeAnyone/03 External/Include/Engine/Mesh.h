#pragma once
#include "Resource.h"


class CMesh :
	public CResource
{
private:
	ComPtr<ID3D12Resource>		m_pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView;		// 버텍스 버퍼 정보 구조체(stride, total size, 가상 GPU 주소)

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

