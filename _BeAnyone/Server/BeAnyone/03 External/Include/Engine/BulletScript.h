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
	CGameObject* m_pPlayer;

	float		m_fTime;
	bool		m_bDead;

public:
	virtual void update();
	virtual void OnCollisionEnter(CCollider* _pOther);

	void SetPlayer(CGameObject* _player) { m_pPlayer = _player; }
	CGameObject* GetPlayer() { return m_pPlayer; }

public:
	CLONE(CBulletScript);

public:
	CBulletScript();
	virtual ~CBulletScript();
};

