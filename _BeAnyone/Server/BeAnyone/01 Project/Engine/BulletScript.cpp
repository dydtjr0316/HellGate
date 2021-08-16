#include "pch.h"
#include "BulletScript.h"
#include "ParticleSystem.h"
#include "ParticleScript.h"


CBulletScript::CBulletScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
	, m_bType(BULLET_TYPE::DEFAULT)
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
	//cout << "총알 위치\t" << "x : " << Transform()->GetLocalPos().x << "\t" << Transform()->GetLocalPos().y << "\t" << Transform()->GetLocalPos().z << endl;

	m_fTime += DT;

	if (m_fTime > 1.0f)
	{
		m_bDead = true;
		DeleteObject(GetObj());
	}
}

void CBulletScript::OnCollisionEnter(CCollider* _pOther)
{
	Vector3 vecYPos = Transform()->GetLocalPos();
	vecYPos.y += 200.f;
	vecYPos.z += 30.f;
	g_netMgr.Send_Attack_Effect(_pOther->GetObj()->GetID(), vecYPos, true);

	switch ((UINT)m_bType)
	{
	case (UINT)BULLET_TYPE::DEFAULT:
		if (L"FireMonster" == _pOther->GetObj()->GetName())
		{
			/// 여기까지 파티클 생성하는거
			/// ////////////////////////////////////////////////////////////////////////////////

			//	총알 객체 삭제 (자신)
			m_fTime = 2.0f;
		}
		break;
	case (UINT)BULLET_TYPE::PICKUP:
		//	여기다가 픽업 불릿 분기문 설정
		break;
	case (UINT)BULLET_TYPE::MONSTER_ATTACK:	
		if (L"PlayerMale" == _pOther->GetObj()->GetName())
			cout << "플레이어 쳐맞음!" << endl;
		//	몬스터의 공격
		break;

	default:
		break;
	}
}