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
	ComPtr<ID3D12Resource>		m_pVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView;		// ���ؽ� ���� ���� ����ü(stride, total size, ���� GPU �ּ�)
	
	
	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	void*						m_pVtxSysMem; 

	vector<tIndexInfo>			m_vecIdxInfo;

	// Animation3D ����
	vector<tMTAnimClip>			m_vecAnimClip;
	vector<tMTBone>				m_vecBones;

	CStructuredBuffer* m_pBoneFrameData;	// ��ü �� ������ ����
	CStructuredBuffer* m_pBoneOffset;	    // �� ���� offset ���

private:
	//	�� ��ü mesh ��ǥ �ּ� �ִ밪 ������ ���� �������.
	Vector4						m_vecMMax[2];

public:
	void Create(UINT _iVtxSize, UINT _iVtxCount, BYTE* _pVtxSysMem
		, DXGI_FORMAT _iIdxFormat, UINT _iIdxCount, BYTE* _pIdxSysMem);

	static CMesh* CreateFromContainer(CFBXLoader& _loader);

	void render(UINT _iSubset = 0);

public:
	UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }
	const vector<tMTBone>* GetBones() { return &m_vecBones; }
	const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
	CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // ��ü �� ������ ����
	CStructuredBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // �� ���� offset ���	
	UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
	bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

public:
	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath/*��� ���*/);

public:
	Vector4* GetMinMaxVertex() { return m_vecMMax; }
	void SetMinMaxVertex(Vector4* _v) { m_vecMMax[0] = _v[0]; m_vecMMax[1] = _v[1];	}

public:
	Vector3 GetBoundingBoxExtents() 
	{
		//	mesh �ִ밪�� ������ �� �� ���ٴ� ����. �ּҰ��� ���밪 �Լ� �ְ� / 2
		m_vecMMax[0].x = fabs(m_vecMMax[0].x);
		m_vecMMax[0].y = fabs(m_vecMMax[0].y);
		m_vecMMax[0].z = fabs(m_vecMMax[0].z);
		return (((m_vecMMax[1]) + (m_vecMMax[0])) / 2.f); 
	}

	float GetBoundingSphereRadius()
	{
		float radius{};

		m_vecMMax[0].x = fabs(m_vecMMax[0].x);
		m_vecMMax[0].y = fabs(m_vecMMax[0].y);
		m_vecMMax[0].z = fabs(m_vecMMax[0].z);
		
		//	x,y,z�� ��� ���͵�
		Vector3 vecAver = (m_vecMMax[1] + m_vecMMax[0]) / 2.f;

		//	�� ��պ��͵�� ��ճ��� �浹 ������ ���� ��հ� ���� ���
		float fAver = (vecAver.x + vecAver.y + vecAver.z) / 3;

		return fAver;
	}
	
public:
	CMesh();
	virtual ~CMesh();
};

