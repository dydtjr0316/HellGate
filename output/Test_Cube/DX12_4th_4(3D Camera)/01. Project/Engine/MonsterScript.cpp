#include "stdafx.h"
#include "MonsterScript.h"

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_iDir(1)
{
}

CMonsterScript::~CMonsterScript()
{
}


void CMonsterScript::update()
{
	// Transform ���� ��ǥ���� ���
	Vec3 vPos = Transform()->GetLocalPos();

	if (vPos.x > 600.f)
		m_iDir = -1;
	else if(vPos.x < -600.f)
		m_iDir = 1;

	vPos.x += DT * 100.f * m_iDir;

	// ������ ��ǥ�� �ٽ� �����ϱ�.
	Transform()->SetLocalPos(vPos);
}

void CMonsterScript::OnCollisionEnter(CCollider2D * _pOther)
{
	// �浹�� �߻��ϰ�, ��� ��ü�� �����̸� �����θ� ����
	if (L"Bullet Object" == _pOther->GetObj()->GetName())
	{		
		DeleteObject(GetObj());	// -->���� �̺�Ʈ ���	
	}	
}

void CMonsterScript::OnCollisionExit(CCollider2D * _pOther)
{	
}
