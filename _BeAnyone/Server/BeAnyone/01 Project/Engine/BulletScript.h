#pragma once

#include "Script.h"

enum class BULLET_TYPE
{
	DEFAULT,

};


class CBulletScript :
	public CScript
{
private:
	BULLET_TYPE m_BType;

	float		m_fTime;
	bool		m_bDead;

public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);

public:
	CLONE(CBulletScript);

public:
	CBulletScript();
	virtual ~CBulletScript();
};

