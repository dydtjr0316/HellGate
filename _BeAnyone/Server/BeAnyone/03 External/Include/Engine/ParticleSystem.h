#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"


class CStructuredBuffer;

class CParticleSystem :
	public CComponent
{
private:
	CStructuredBuffer*  m_pParticleBuffer; // 파티클 데이터 저장
	CStructuredBuffer*  m_pSharedBuffer;   // 파티클 공유 메모리
	UINT				m_iMaxParticle;    // 최대 파티클 개수

	Ptr<CMaterial>		m_pUpdateMtrl;		// 컴퓨트 쉐이더
	Ptr<CMaterial>		m_pMtrl;			// 파티클 렌더링용
	Ptr<CMesh>			m_pMesh;			// 사용 메쉬(Point Mesh)

	float				m_fFrequency;		// 파티클 생성 주기
	float				m_fAccTime;		    // 누적시간 체크용도

	float				m_fMinLifeTime;   // 파티클 최소 생존 시간
	float				m_fMaxLifeTime;   // 파티클 최대 생존 시간

	float				m_fMinSpeed;	  // 파티클 시작 속도
	float				m_fMaxSpeed;	  // 파티클 최종 속도

	float				m_fStartScale;   // 파티클 시작 크기
	float				m_fEndScale;     // 파티클 최종 크기

	Vector4				m_vStartColor;	// 파티클 초기 색상
	Vector4				m_vEndColor;    // 파티클 최종 색상

private:
	bool				m_bType;

public:
	virtual void finalupdate();
	virtual void render();

public:
	float GetAcctime() { return m_fAccTime; }
	float GetMaxLifeTime() { return m_fMaxLifeTime; }

public:
	void Init(float frequency, float acctime, float minLife, float maxLife, float minSpeed, float maxSpeed, float StartScale, float EndScale, Vector4 StartColor, Vector4 EndColor);
	void SetStartColor(const Vector4& _vColor) { m_vStartColor = _vColor; }
	void SetEndColor(const Vector4& _vColor) { m_vEndColor = _vColor; }
	void SetFrequency(float _fre) { m_fFrequency = _fre; }
	void SetMinLife(float _min) { m_fMinLifeTime = _min; }
	void SetMaxLife(float _max) { m_fMaxLifeTime = _max; }
	
public:
	void SetType(bool _b) { m_bType = _b; }

	int InfiniteParticle();
	int finiteParticle();

public:
	CLONE(CParticleSystem);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CParticleSystem();
	virtual ~CParticleSystem();
};

