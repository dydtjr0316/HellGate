#include "pch.h"
#include "BulletScript.h"
#include "ParticleSystem.h"
#include "ParticleScript.h"
#include "PlayerScript.h"


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
	//cout << "총알 시간: " << m_fTime << endl;

	if (m_fTime > 2.0f)
	{
		m_bDead = true;
		//cout << "-=========총알 객체 삭제=========" << endl;
		DeleteObject(GetObj());
	}
}

void CBulletScript::OnCollisionEnter(CCollider* _pOther)
{
	Vector3 vecYPos = Transform()->GetLocalPos();
	vecYPos.y += 200.f;
	vecYPos.z += 30.f;

	switch ((UINT)m_bType)
	{
	case (UINT)BULLET_TYPE::DEFAULT:
		m_fTime = 3.0f;
		g_netMgr.Send_Attack_Effect(_pOther->GetObj()->GetID(), vecYPos, true);

		break;
	case (UINT)BULLET_TYPE::PICKUP:
		//	여기다가 픽업 불릿 분기문 설정
		break;
	case (UINT)BULLET_TYPE::MONSTER_ATTACK:	
		if (L"PlayerMale" == _pOther->GetObj()->GetName())
		{
			CGameObject* pPlayer = _pOther->GetObj();
			//	--------------------여따하면 댐---------------------------------------
			CPlayerScript* playerScript = pPlayer->GetScript<CPlayerScript>();
			playerScript->ReduceHpBar();
	/*		cout << "++++++++++++++++++++++++++" << endl;
			cout << "++++++++++아아아++++++++++++++++" << endl;
			cout << "+++++++++++여러번+++++++++++" << endl;
			cout << "+++++++++++++불림++++++++++" << endl;
			cout << "++++++++++++++++++++++++++" << endl;*/

			//---------------------------------------------------------------------------
			
			
			m_fTime = 3.0f;
		}
		//	몬스터의 공격
		break;

	default:
		break;
	}
}