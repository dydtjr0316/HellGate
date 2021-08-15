#include "pch.h"
#include "ParticleScript.h"
#include "ParticleSystem.h"


CParticleScript::CParticleScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
	, m_bType(PARTICLE_TYPE::DEFAULT)
	, m_fTime(0.0f)
	, m_bDead(false)
{
	//	반드시 스크립트 넣기 전에 컴포넌트로서 ParticleSystem 넣어라
	//m_fTime = ParticleSystem()->GetMaxLifeTime();
}

CParticleScript::~CParticleScript()
{
}

void CParticleScript::update()
{

	if (m_fTime < ParticleSystem()->GetAcctime())
	{
		m_bDead = true;
		//g_netMgr.Send_Attack_Effect(_pOther->GetObj()->GetID(), vecYPos, false);
		DeleteObject(GetObj());
	}
}

void CParticleScript::finalupdate()
{

}

void CParticleScript::OnCollisionEnter(CCollider* _pOther)
{

}