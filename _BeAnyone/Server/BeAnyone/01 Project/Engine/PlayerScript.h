#pragma once
#include "Script.h"
#include "Terrain.h"
#include "Mesh.h"

class CPlayerScript
	: public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;
	vector<Ptr<CMesh>>	m_pAniData;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();


private:
	CTerrain* m_pTerrainObj;
	XMFLOAT3 m_xmf3Velocity;

	Vector3 m_vecPlayerDir;

public:
	XMFLOAT3 GetVelocity() { return m_xmf3Velocity; }
	void SetVelocity(XMFLOAT3 _fVelocity) { m_xmf3Velocity = _fVelocity; }
	void SetAnimation(const Ani_TYPE& type);
	Ptr<CMesh> GetAniData(const Ani_TYPE& type) { return m_pAniData[(int)type]; }

	void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }
	CTerrain* GetTerrain() { return m_pTerrainObj; }
	void OnPlayerUpdateCallback();
	bool isInMap(const Vector3& localPos);

	// 애니메이션 전환
	void SetAnimationData(Ptr<CMesh> _meshData) { m_pAniData.push_back(_meshData); }


public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
};

