#pragma once
#include "Script.h"
#include "Terrain.h"

class CPlayerScript
	: public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

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

public:
	XMFLOAT3 GetVelocity() { return m_xmf3Velocity; }
	void SetVelocity(XMFLOAT3 _fVelocity) { m_xmf3Velocity = _fVelocity; }
	void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }
	CTerrain* GetTerrain() { return m_pTerrainObj; }

	
	void OnPlayerUpdateCallback();

	bool isInMap(const Vector3& localPos);

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
};

