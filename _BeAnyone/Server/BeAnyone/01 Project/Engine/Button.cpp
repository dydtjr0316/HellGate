#include "pch.h"
#include "Button.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"

void CButton::init()
{

}

void CButton::update()
{
	GetObj()->SetUiRenderCheck(m_bActive);

	switch (m_eItemId)
	{
	case ITEM_ID::EMPTY:
		break;

	case ITEM_ID::BASIC_SWORD:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD_IMG");
		break;

	case ITEM_ID::BASIC_ARROW:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOW_IMG");
		break;

	case ITEM_ID::END:
		return;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{

		for (int i = 0; i < 16; ++i)
		{
			GetObj()->StaticUI();
		}
		//CGameObject* pObject = new CGameObject;
		//pObject->SetName(L"MousePoint");
		//pObject->AddComponent(new CTransform);
		//pObject->AddComponent(new CCollider);
		//pObject->FrustumCheck(false);
		//pObject->Transform()->SetLocalPos(Vector3(0.f, 450.f, 1.f));
		//pObject->Transform()->SetLocalScale(Vector3(300.f, 6000.f, 300.f)); // 2น่วิ
		//pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"RectMtrl"));
		//CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		////g_netMgr.SetObj(pObject);

		////CCollisionMgr::GetInst()->CollisionRect();
		//auto mPos = CKeyMgr::GetInst()->GetMousePos();

	}
	else
	{
		m_vecOldPoint = CKeyMgr::GetInst()->GetPreMousePos();
	}
}

void CButton::finalupdate()
{

}

CButton::CButton()
	: CScript((UINT)SCRIPT_TYPE::TESTSCRIPT)
	, m_eItemId(ITEM_ID::EMPTY)
	, m_bActive(false)
{
	m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD_IMG");
}


CButton::~CButton()
{
}

void CButton::SaveToScene(FILE* _pFile)
{
}

void CButton::LoadFromScene(FILE* _pFile)
{
}

void CButton::OnCollisionEnter(CCollider* _pOther)
{
}

void CButton::OnCollision(CCollider* _pOther)
{
	
	Vector3 localPos = _pOther->Transform()->GetLocalPos();
	GetObj()->Transform()->SetLocalPos(localPos);

}

void CButton::OnCollisionExit(CCollider* _pOther)
{
}