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
            cout << "senser에 들어온 다른 player" << _pOther->GetObj()->GetID() << endl;
            return;
        }
        GetObj()->GetParent()->GetScript<CMonsterScript>()->SetIsNearPlayer(true);
        GetObj()->GetParent()->GetScript<CMonsterScript>()->SetPlayer(_pOther->GetObj());
        cout << "Sensor Set Player : " << _pOther->GetObj()->GetID() << endl;
        return;
    }
    else if (GetObj()->GetParent()->GetScript<CMonsterScript>()->GetMonsterState() == MONSTER_STATE::MOVE) {
        cout << "senser 닿음" << endl;
        GetObj()->GetParent()->GetScript<CMonsterScript>()->SetIsFindPlayer(true);
        GetObj()->GetParent()->GetScript<CMonsterScript>()->SetPlayer(_pOther->GetObj());
    }
}
