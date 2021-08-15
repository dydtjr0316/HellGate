#include "pch.h"
#include "SnowScript.h"

void CSnowScript::update()
{
}

void CSnowScript::finalupdate()
{
}

void CSnowScript::OnCollisionEnter(CCollider* _pOther)
{
}

CSnowScript::CSnowScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
	, m_vecVelocity(0.0f, -5.0f, 0.0f)
	, m_vecOrigin(0.0f, 0.0f, 0.0f)
	, m_vecExtents(0.0f, 0.f, 0.f)
{
	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh");
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleMtrl");
	m_pUpdateMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleUpdateMtrl");

}

CSnowScript::~CSnowScript()
{
}
