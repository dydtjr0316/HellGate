#pragma once

#include "Script.h"

enum class BULLET_TYPE
{
	DEFAULT,
	PICKUP,
};


class CBulletScript :
	public CScript
{
private:
	BULLET_TYPE m_bType;

	float		m_fTime;
	bool		m_bDead;

private:
	// pick up
	CGameObject* m_pItemUIObj;

public:
	void SetBulletType(BULLET_TYPE _type) { m_bType = _type; }

	CGameObject* GetUIObj() { return m_pItemUIObj; }
	void SetUIObj(CGameObject* _obj) { m_pItemUIObj = _obj; }

public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);

public:
	CLONE(CBulletScript);

public:
	CBulletScript();
	virtual ~CBulletScript();
};

