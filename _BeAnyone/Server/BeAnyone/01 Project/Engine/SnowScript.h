#pragma once
#include "Script.h"

class CSnowScript :
	public CScript
{
private:
	Vector3				m_vecVelocity;	
	Vector3				m_vecOrigin;
	Vector3				m_vecExtents;

	Ptr<CMesh>			m_pMesh;			// 사용 메쉬(Point Mesh)
	Ptr<CMaterial>		m_pMtrl;			// 파티클 렌더링용
	Ptr<CMaterial>		m_pUpdateMtrl;		// 컴퓨트 쉐이더


public:
	virtual void update();
	virtual void finalupdate();


	virtual void OnCollisionEnter(CCollider* _pOther);

public:
	CLONE(CSnowScript);

public:
	CSnowScript();
	virtual ~CSnowScript();
};


