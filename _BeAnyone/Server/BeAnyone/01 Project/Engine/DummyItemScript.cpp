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
	//cout << "���� ������ ����" << endl;
	if (_pOther->GetName() == L"Player Object")
	{
		// obj �� Player�� ��ӵ� StaticUI ������Ʈ�� ���� ���� ������Ʈ
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