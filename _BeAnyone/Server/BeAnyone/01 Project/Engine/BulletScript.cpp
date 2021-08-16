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
	//cout << "�Ѿ� ��ġ\t" << "x : " << Transform()->GetLocalPos().x << "\t" << Transform()->GetLocalPos().y << "\t" << Transform()->GetLocalPos().z << endl;

	m_fTime += DT;

	if (m_fTime > 2.0f)
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
			//	�Ѿ� ��ü ���� (�ڽ�)
			m_fTime = 3.0f;
		}
		break;
	case (UINT)BULLET_TYPE::PICKUP:
		//	����ٰ� �Ⱦ� �Ҹ� �б⹮ ����
		break;
	case (UINT)BULLET_TYPE::MONSTER_ATTACK:	
		if (L"PlayerMale" == _pOther->GetObj()->GetName())
		{
			CGameObject* pPlayer = _pOther->GetObj();
			//	--------------------�����ϸ� ��---------------------------------------


			//---------------------------------------------------------------------------
			
			
			m_fTime = 3.0f;
		}
		//	������ ����
		break;

	default:
		break;
	}
}