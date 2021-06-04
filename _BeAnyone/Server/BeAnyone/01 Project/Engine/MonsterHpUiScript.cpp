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

	Matrix MonsterMatWVP = m_pMonster->Transform()->GetMatWVP();

	//UiPos = MonsterPos;
	//UiPos.y = MonsterPos.y + 500.f;
	//UiPos.z = 1.f;
	Vector3 a = XMVector3TransformNormal(MonsterPos, MonsterMatWVP);

	UiPos.y = 150.f;
	UiPos.x = a.x;

	//cout << MonsterMatWVP._11 << "\t" << MonsterMatWVP._12 << "\t" << MonsterMatWVP._13 << "\n" <<
	//	MonsterMatWVP._21 << "\t" << MonsterMatWVP._22 << "\t" << MonsterMatWVP._23 << "\n" <<
	//	MonsterMatWVP._31 << "\t" << MonsterMatWVP._32 << "\t" << MonsterMatWVP._33 << "\n" << endl;

	cout << a.x << "\t" << a.y << "\t" << a.z << "\t" << endl;
	
		

	m_pObject->Transform()->SetLocalPos(UiPos);
}