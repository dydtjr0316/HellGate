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

	if (_pOther->GetObj()->GetName() == L"PlayerMale")
	{
		// obj �� Player�� ��ӵ� StaticUI ������Ʈ�� ���� ���� ������Ʈ
		CGameObject* obj = _pOther->GetObj()->GetScript<CPlayerScript>()->GetUIObj();
		wstring strName = GetObj()->MeshRender()->GetMesh()->GetName();

		CheckItemMesh(strName, obj);
			
	}

	DeleteObject(GetObj());
}

void CDummyItemScript::OnCollisionExit(CCollider* _pOther)
{
}

void CDummyItemScript::OnCollsion(CCollider* _pOther)
{
}

void CDummyItemScript::CheckItemMesh(wstring _str, CGameObject* _obj)
{
	if (_str == L"Mesh\\Branch.mesh")
		_obj->StaticUI()->SetButton(ITEM_ID::BRANCH);
	else if(_str == L"Mesh\\Apple.mesh")
		_obj->StaticUI()->SetButton(ITEM_ID::APPLE);
	else if (_str == L"Mesh\\Bottle_03.mesh")
		_obj->StaticUI()->SetButton(ITEM_ID::BOTTLE_STAMINA);
	else if (_str == L"Mesh\\Bottle_04.mesh")
		_obj->StaticUI()->SetButton(ITEM_ID::BOTTLE_DASH);
	else if (_str == L"Mesh\\Steak_02.mesh")
		_obj->StaticUI()->SetButton(ITEM_ID::STEAK);
	else if (_str == L"Mesh\\MoneyBag.mesh")
		_obj->StaticUI()->SetButton(ITEM_ID::MONEYBAG);
	else if (_str == L"Mesh\\Carrot.mesh")
		_obj->StaticUI()->SetButton(ITEM_ID::CARROT);
}
