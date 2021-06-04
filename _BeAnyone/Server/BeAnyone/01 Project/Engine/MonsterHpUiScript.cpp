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

	Vector3 uiFront = m_pObject->Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vector3 PlayerFront = m_pPlayer->Transform()->GetLocalDir(DIR_TYPE::FRONT);
	
	//Vector3 UiRot = XMVector3Dot(uiFront, -MonsterFront);
	//float radian = acos();

	float UiDot = XMVectorGetY(XMVector3Dot(uiFront, -PlayerFront));	// 두 벡터의 내적
	float UiRot = acos(UiDot);
	//cout << UiRot.x << "\t" << UiRot.y << "\t" << UiRot.z  << endl;
	//Matrix MonsterMatWVP = m_pMonster->Transform()->GetMatWVP();

	cout << "--------------------------------------------" << endl;
	cout << "UiFront: " << uiFront.x << "\t" << uiFront.y << "\t" << uiFront.z << endl;
	cout << "PlayerFront: " << PlayerFront.x << "\t" << PlayerFront.y << "\t" << PlayerFront.z << endl;
	cout << "UiRot: "<< UiRot << endl;
	cout << "UiDot: " << UiDot << endl;
	cout << "--------------------------------------------" << endl << endl;


	UiPos = MonsterPos;
	UiPos.y = MonsterPos.y + 50.f;
	UiPos.z = 1.f;
	// 
	//Vector3 a = XMVector3TransformNormal(MonsterPos, MonsterMatWVP);


	
		

	m_pObject->Transform()->SetLocalPos(UiPos);
	m_pObject->Transform()->SetLocalRot(Vector3(0.f, UiRot, 0.f));
}