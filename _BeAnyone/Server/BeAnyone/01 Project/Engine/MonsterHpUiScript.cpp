#include "pch.h"
#include "MonsterHpUiScript.h"

#include <iostream>
using namespace std;


CMonsterHpUiScript::CMonsterHpUiScript()
	: CScript(0)
{

}

CMonsterHpUiScript::~CMonsterHpUiScript()
{

}

void CMonsterHpUiScript::update()
{
	Vector3 MonsterPos = m_pMonster->Transform()->GetLocalPos();
	Vector3 UiPos = m_pObject->Transform()->GetLocalPos();

	Vector3 PlayerRot = m_pPlayer->Transform()->GetLocalRot();
	
	UiPos = MonsterPos;
	UiPos.x = MonsterPos.x - 100.f;
	UiPos.y = MonsterPos.y + 300.f;
	UiPos.z = MonsterPos.z;

	m_pObject->Transform()->SetLocalPos(UiPos);
	m_pObject->Transform()->SetLocalRot(PlayerRot + Vector3(0.f, XM_PI, 0.f));
}