#include "pch.h"
#include "MonsterScript.h"

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
{
}

CMonsterScript::~CMonsterScript()
{
}


void CMonsterScript::update()
{
	// Transform ���� ��ǥ���� ���
	Vector3 vPos = Transform()->GetLocalPos();

	// ������ ��ǥ�� �ٽ� �����ϱ�.
	Transform()->SetLocalPos(vPos);
}

void CMonsterScript::OnCollisionEnter(CCollider* _pOther)
{
	// �浹�� �߻��ϰ�, ��� ��ü�� �����̸� �����θ� ����
	if (L"Attack Object" == _pOther->GetObj()->GetName())
	{
		DeleteObject(GetObj());	// -->���� �̺�Ʈ ���	
	}
}

void CMonsterScript::OnCollisionExit(CCollider* _pOther)
{
}
