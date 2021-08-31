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
        if (GetObj()->GetParent()->GetScript<CMonsterScript>()->GetPlayer()->GetID() != _pOther->GetObj()->GetID()) {
            cout << "senser�� ���� �ٸ� player" << _pOther->GetObj()->GetID() << endl;
            return;
        }
        GetObj()->GetParent()->GetScript<CMonsterScript>()->SetIsNearPlayer(true);
        GetObj()->GetParent()->GetScript<CMonsterScript>()->SetPlayer(_pOther->GetObj());
        cout << "Sensor Set Player : " << _pOther->GetObj()->GetID() << endl;
        return;
    }
    else if (GetObj()->GetParent()->GetScript<CMonsterScript>()->GetMonsterState() == MONSTER_STATE::MOVE) {
        cout << "senser ����" << endl;
        GetObj()->GetParent()->GetScript<CMonsterScript>()->SetIsFindPlayer(true);
        GetObj()->GetParent()->GetScript<CMonsterScript>()->SetPlayer(_pOther->GetObj());
    }
}
