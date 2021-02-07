#pragma once
#include "Resource.h"

#include "FBXLoader.h"
#include "Ptr.h"
#include "StructuredBuffer.h"

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
	ComPtr<ID3D12Resource>		m_pVB;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView; // ���ؽ� ���� ���� ����ü(stride, total size, ���� GPU �ּ�)
	
	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	void*						m_pVtxSysMem;

	vector<tIndexInfo>			m_vecIdxInfo;

	// Animation3D ����
	vector<tMTAnimClip>			m_vecAnimClip;
	vector<tMTBone>				m_vecBones;	

	CStructuredBuffer*			m_pBoneFrameData; // ��ü �� ������ ����
	CStructuredBuffer*			m_pBoneOffset;	   // �� ���� offset ���


public:
	void Create(UINT _iVtxSize, UINT _iVtxCount, BYTE* _pVtxSysMem
		, DXGI_FORMAT _iIdxFormat, UINT _iIdxCount, BYTE* _pIdxSysMem);

	static CMesh * CreateFromContainer(CFBXLoader & _loader);

	void render(UINT _iSubset = 0);
	void render_instancing(UINT _iInstancCount, UINT _iSubset = 0);

public:
	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
	const vector<tMTBone>* GetBones() { return &m_vecBones; }	
	const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }	
	CStructuredBuffer*	GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // ��ü �� ������ ����
	CStructuredBuffer*	GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // �� ���� offset ���	
	UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
	bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

public:	
	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath/*��� ���*/);

public:
	CMesh();
	virtual ~CMesh();
};

