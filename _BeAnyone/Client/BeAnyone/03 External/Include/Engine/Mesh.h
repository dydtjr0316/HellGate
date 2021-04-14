#pragma once
#include "Resource.h"

#include "FBXLoader.h"
#include "Ptr.h"
#include "Texture.h"

struct tIndexInfo
{
	ComPtr<ID3D12Resource>		pIB;
	D3D12_INDEX_BUFFER_VIEW		tIdxView;
	DXGI_FORMAT					eIdxFormat;
	UINT						iIdxCount;
	void*						pIdxSysMem;
};

class CMesh :
	public CResource
{
private:
	ComPtr<ID3D12Resource>		m_pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView;		// 버텍스 버퍼 정보 구조체(stride, total size, 가상 GPU 주소)
	
	
	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	void*						m_pVtxSysMem; 

	vector<tIndexInfo>			m_vecIdxInfo;

	// Animation3D 정보
	//vector<tMTAnimClip>			m_vecAnimClip;
	//vector<tMTBone>				m_vecBones;
	//Ptr<CTexture>				m_pBoneTex;

public:
	void Create(UINT _iVtxSize, UINT _iVtxCount, BYTE* _pVtxSysMem
		, DXGI_FORMAT _iIdxFormat, UINT _iIdxCount, BYTE* _pIdxSysMem);

	static CMesh* CreateFromContainer(CFBXLoader& _loader);

	void render(UINT _iSubset = 0);

public:
	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
	//const vector<tMTBone>* GetBones() { return &m_vecBones; }
	//void SetBoneTex(Ptr<CTexture> _pTex) { m_pBoneTex = _pTex; }
	//const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
	//Ptr<CTexture> GetBoneTex() { return m_pBoneTex; }
	//bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

public:
	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath/*상대 경로*/);


public:
	CMesh();
	virtual ~CMesh();
};

