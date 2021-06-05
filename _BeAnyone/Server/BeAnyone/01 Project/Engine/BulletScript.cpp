#include "pch.h"
#include "BulletScript.h"


CBulletScript::CBulletScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
	, m_BType(BULLET_TYPE::DEFAULT)
	, m_fTime(0.0f)
	, m_bDead(false)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::update()
{
	Vector3 vPos = Transform()->GetLocalPos();

	Transform()->SetLocalPos(vPos);
	//cout << "ÃÑ¾Ë À§Ä¡\t" << "x : " << Transform()->GetLocalPos().x << "\t" << Transform()->GetLocalPos().y << "\t" << Transform()->GetLocalPos().z << endl;


	m_fTime += DT;

	if (m_fTime > 1.f)
	{
		m_bDead = true;
		DeleteObject(GetObj());
	}
	


}

void CBulletScript::OnCollisionEnter(CCollider* _pOther)
{
	if (L"Monster Object" == _pOther->GetObj()->GetName())
	{
		DeleteObject(GetObj());
	}
}
