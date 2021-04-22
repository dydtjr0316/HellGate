#pragma once
#include "Script.h"
#include "Terrain.h"

class CPlayerScript :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE( CPlayerScript );


public:
	CPlayerScript();
	CPlayerScript(CTerrain* _terrain);
	virtual ~CPlayerScript();

private:
	CTerrain* m_pTerrainObj;
	XMFLOAT3 m_xmf3Velocity;

public:
	XMFLOAT3 GetVelocity() { return m_xmf3Velocity; }
	void SetVelocity(XMFLOAT3 _fVelocity) { m_xmf3Velocity = _fVelocity; }

	void OnPlayerUpdateCallback();

	void SetTerrain(CTerrain* _terrain) { m_pTerrainObj = _terrain; }
};

