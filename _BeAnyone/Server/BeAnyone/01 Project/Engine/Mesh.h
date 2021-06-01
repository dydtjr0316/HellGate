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
	D3D12_VERTEX_BUFFER_VIEW	m_tVtxView;		// 버텍스 버퍼 정보 구조체(stride, total size, 가상 GPU 주소)
	
	
	UINT						m_iVtxSize;
	UINT						m_iVtxCount;
	void*						m_pVtxSysMem; 

	vector<tIndexInfo>			m_vecIdxInfo;

	// Animation3D 정보
	vector<tMTAnimClip>			m_vecAnimClip;
	vector<tMTBone>				m_vecBones;

	CStructuredBuffer* m_pBoneFrameData;	// 전체 본 프레임 정보
	CStructuredBuffer* m_pBoneOffset;	    // 각 뼈의 offset 행렬

private:
	//	모델 객체 mesh 좌표 최소 최대값 전달을 위한 멤버변수.
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
	CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // 전체 본 프레임 정보
	CStructuredBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // 각 뼈의 offset 행렬	
	UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
	bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

public:
	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath/*상대 경로*/);

public:
	Vector4* GetMinMaxVertex() { return m_vecMMax; }
	void SetMinMaxVertex(Vector4* _v) { m_vecMMax[0] = _v[0]; m_vecMMax[1] = _v[1];	}

public:
	Vector3 GetBoundingBoxExtents(Vector3 _vec = { 1.f, 1.f, 1.f })
	{
		//	mesh 최대값이 음수일 수 가 없다는 가정. 최소값만 절대값 함수 넣고 / 2
		Vector3 vecmin{}, vecmax{m_vecMMax[1]};
		vecmin.x = fabs(m_vecMMax[0].x);
		vecmin.y = fabs(m_vecMMax[0].y);
		vecmin.z = fabs(m_vecMMax[0].z);

		Vector3 a = ((vecmax + vecmin) / 2.f );
		a.x *= _vec.x; a.y *= _vec.y; a.z *= _vec.z;



		/*m_vecMMax[0].x = fabs(m_vecMMax[0].x);
		m_vecMMax[0].y = fabs(m_vecMMax[0].y);
		m_vecMMax[0].z = fabs(m_vecMMax[0].z);
		auto a = ((m_vecMMax[1] + m_vecMMax[0]) / 2.f) * _vec);
		return (((m_vecMMax[1]) + (m_vecMMax[0])) / 2.f); */
		return a;
	}

	float GetBoundingSphereRadius()
	{
		m_vecMMax[0].x = fabs(m_vecMMax[0].x);
		m_vecMMax[0].y = fabs(m_vecMMax[0].y);
		m_vecMMax[0].z = fabs(m_vecMMax[0].z);
		
		//	x,y,z축 평균 벡터들
		Vector3 vecAver = (m_vecMMax[1] + m_vecMMax[0]) / 2.f;

		//	각 평균벡터들로 평균내서 충돌 반지름 길이 평균값 대충 계산
		float fAver = (vecAver.x + vecAver.y + vecAver.z) / 3;

		return fAver;
	}
	
public:
	CMesh();
	virtual ~CMesh();
};

