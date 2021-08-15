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

	if (m_fTime > 3.0f)
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
	case 0:
		if (L"FireMonster" == _pOther->GetObj()->GetName())
		{
			

			/// ������� ��ƼŬ �����ϴ°�
			/// ////////////////////////////////////////////////////////////////////////////////

			//	�Ѿ� ��ü ���� (�ڽ�)
			m_fTime = 4.0f;
		}
		break;
	case 1:
		//	����ٰ� �Ⱦ� �Ҹ� �б⹮ ����
		break;
	}
}