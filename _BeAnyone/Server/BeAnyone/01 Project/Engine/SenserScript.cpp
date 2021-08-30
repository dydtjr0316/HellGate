#include "pch.h"
#include "SenserScript.h"
#include "MonsterScript.h"


CSenserScript::CSenserScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
{
}

CSenserScript::~CSenserScript()
{
}

   
void CSenserScript::update()
{
}

void CSenserScript::OnCollisionEnter(CCollider* _pOther)
{
	if (GetObj()->GetParent()->GetScript<CMonsterScript>()->GetMonsterState() == MONSTER_STATE::FOLLOW) {
		if (GetObj()->GetParent()->GetScript<CMonsterScript>()->GetPlayer() != _pOther->GetObj())
			return;

		GetObj()->GetParent()->GetScript<CMonsterScript>()->SetIsNearPlayer(true);
		GetObj()->GetParent()->GetScript<CMonsterScript>()->SetPlayer(_pOther->GetObj());
		cout << "����� senser ����" << endl;
		return;
	}

	cout << "senser ����" << endl;
	GetObj()->GetParent()->GetScript<CMonsterScript>()->SetIsFindPlayer(true);
	GetObj()->GetParent()->GetScript<CMonsterScript>()->SetPlayer(_pOther->GetObj());
}
