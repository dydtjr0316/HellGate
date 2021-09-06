#include "pch.h"
#include "PotalScript.h"
#include "PlayerScript.h"

CPotalScript::CPotalScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
{
}

CPotalScript::~CPotalScript()
{
}

void CPotalScript::update()
{
	

}

void CPotalScript::OnCollisionEnter(CCollider* _pOther)
{
	//cout << "아아ㅏ아아아아ㅏ아아아아ㅏ악!!!!!!!!!" << endl;
	if (_pOther->GetObj()->GetName() == L"PlayerMale") {
		Vector3 vPos = Vector3(68000.f, 0.f, 65000.f);
		
		float a = _pOther->GetObj()->GetScript<CPlayerScript>()->GetTerrain()->GetHeight(68000.f, 65000.f, true) * 2;
		_pOther->GetObj()->Transform()->SetLocalPos(Vector3(vPos.x, a, vPos.z));
	}
}

void CPotalScript::OnCollisionExit(CCollider* _pOther)
{
}

void CPotalScript::OnCollsion(CCollider* _pOther)
{
}

