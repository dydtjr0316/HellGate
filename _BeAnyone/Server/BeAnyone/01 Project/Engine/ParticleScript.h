#pragma once

#include "Script.h"

enum class PARTICLE_TYPE
{
	DEFAULT,
	ATTACK,
};


class CParticleScript :
	public CScript
{
private:
	PARTICLE_TYPE	m_bType;
	CGameObject*	m_pPlayer;

	float			m_fTime;
	bool			m_bDead;

public:
	void SetLifeTime(float _f) { m_fTime = _f; }

public:

	void SetBulletType(PARTICLE_TYPE _type) { m_bType = _type; }

public:
	virtual void update();
	virtual void finalupdate();


	virtual void OnCollisionEnter(CCollider* _pOther);

	void SetPlayer(CGameObject* _player) { m_pPlayer = _player; }
	CGameObject* GetPlayer() { return m_pPlayer; }

public:
	CLONE(CParticleScript);

public:
	CParticleScript();
	virtual ~CParticleScript();
};

