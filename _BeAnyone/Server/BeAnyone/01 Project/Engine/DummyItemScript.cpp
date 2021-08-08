#include "pch.h"
#include "StaticUI.h"
#include "PlayerScript.h"
#include "DummyItemScript.h"

CDummyItemScript::CDummyItemScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
{

}

CDummyItemScript::~CDummyItemScript()
{

}

void CDummyItemScript::update()
{
	
}

void CDummyItemScript::OnCollisionEnter(CCollider* _pOther)
{
	//cout << "씨발 아이템 닿음" << endl;
	if (_pOther->GetName() == L"Player Object")
	{
		// obj 는 Player에 상속된 StaticUI 컴포넌트를 갖는 게임 오브젝트
		CGameObject* obj = _pOther->GetObj()->GetScript<CPlayerScript>()->GetUIObj();
		wstring strName = GetObj()->MeshRender()->GetMesh()->GetName();

		if (strName == L"Mesh\\Branch.mesh")
			obj->StaticUI()->SetButton(ITEM_ID::BASIC_ARROW);
			
	}

	DeleteObject(GetObj());
}

void CDummyItemScript::OnCollisionExit(CCollider* _pOther)
{
}

void CDummyItemScript::OnCollsion(CCollider* _pOther)
{
}