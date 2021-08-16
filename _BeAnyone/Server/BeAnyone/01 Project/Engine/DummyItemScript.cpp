#include "pch.h"
#include "StaticUI.h"
#include "PlayerScript.h"
#include "BulletScript.h"
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
	/*cout << "---------------------------------------" << endl;
	cout << "아이템 위치\t" << "x : " << Transform()->GetLocalPos().x << "\t" << "y : " << Transform()->GetLocalPos().y << "\t" << "z : " << Transform()->GetLocalPos().z << endl;
	cout << "총알 위치\t" << "x : " << Collider()->GetBoundingSphere().Center.x << "\t"
		<< "y : " << Collider()->GetBoundingSphere().Center.y << "\t" 
		<< "z : " << Collider()->GetBoundingSphere().Center.z << endl;*/
}

void CDummyItemScript::OnCollisionEnter(CCollider* _pOther)
{
	//cout << "씨발 아이템 닿음" << endl;

	if (_pOther->GetObj()->GetName() == L"PickUP Object")
	{
		// obj 는 Player에 상속된 StaticUI 컴포넌트를 갖는 게임 오브젝트
		CGameObject* obj = _pOther->GetObj()->GetScript<CBulletScript>()->GetUIObj(); // 0810 기존에 Player와 충돌체크할때는 가능한 코드, BulletSciprt로 바꾸고 현재 터짐.
		wstring strName = GetObj()->MeshRender()->GetMesh()->GetName();

		CheckItemMesh(strName, obj);
		Vector3 a = GetObj()->Transform()->GetLocalPos();
		cout << "보낼때 item pos: " << a.x << "\t" << a.y << "\t" << a.z << endl;

		// 용석 아이템 없어지는 부분 
		g_netMgr.Send_ItemDelete_Paket(GetObj()->Transform( )->GetLocalPos());

		//DeleteObject(GetObj());
	}

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
