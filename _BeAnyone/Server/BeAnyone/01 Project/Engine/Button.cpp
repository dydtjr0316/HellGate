#include "pch.h"
#include "StaticUI.h"
#include "Button.h"
#include "ResMgr.h"
#include "RenderMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"

#include <random>

default_random_engine dreMoney;
uniform_int_distribution<int> uidMoney(15, 50);

void CButton::init()
{
	int num{};
	if (m_eUiType == UI_TYPE::PRIVATE_ITEM_UI) {
		num = (UINT)ITEM_NUM::HUNDREDS;
		CreateItemNum(num);
		//CreateExplainBox();
	}
	else if (m_eUiType == UI_TYPE::PUBLIC_SHOP_UI) {
		num = (UINT)ITEM_NUM::END;
		SetItemPrice();
		if (m_eItemId != ITEM_ID::EMPTY)
			CreateItemNum(num);
	}

}

void CButton::update()
{

	GetObj()->SetUiRenderCheck(m_bActive);
	for (int i = 0; i < m_vItemNum.size(); ++i)
		m_vItemNum[i]->SetUiRenderCheck(m_bActive);
	
	if (m_bChangeCount && (m_vItemNum.size() != 0))
		ChangeNumTexture();

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{

		for (int i = 0; i < 16; ++i)
		{
			GetObj()->StaticUI();
		}
	}

	if (!m_bActive)
		return;
	else {
		ChangeButtonTexture();

		for (int i = 0; i < m_vItemNum.size(); ++i) {
			Vector3 vButtonPos = GetObj()->Transform()->GetLocalPos();
			Vector3 vButtonScale = GetObj()->Transform()->GetLocalScale();
			Vector3 vNumPos = m_vItemNum[i]->Transform()->GetLocalPos();
			Vector3 vNumScale = m_vItemNum[i]->Transform()->GetLocalScale();

			vNumPos = vButtonPos;
			vNumPos += Vector3((vButtonScale.x / 2 - vNumScale.x / 2) - (i * vNumScale.x), -(vButtonScale.y / 2 - vNumScale.y / 2), 0.f);

			m_vItemNum[i]->Transform()->SetLocalPos(vNumPos);
		}
	}
}

void CButton::finalupdate()
{

}

void CButton::OnCollisionEnter(CCollider* _pOther)
{
	if (m_bState == BT_STATE::SELL)
		return;

	vector<CButton*> vecButton = m_pComParent->GetObj()->StaticUI()->GetButton();
	for (int i = 0; i < vecButton.size(); ++i)
	{
		if (vecButton[i]->GetActive() == BT_ACTIVE::ACTIVE)
			return;
	}
	m_bCheckActive = BT_ACTIVE::ACTIVE;

	m_vecOldPos = Transform()->GetLocalPos();

}

void CButton::OnCollision(CCollider* _pOther)
{
	if (m_bCheckActive == BT_ACTIVE::PASSIVE)
		return;


	m_bState = BT_STATE::CLICKED;

	Vector3 localPos = _pOther->Transform()->GetLocalPos();
	GetObj()->Transform()->SetLocalPos(localPos);

	/*if (_pOther->GetObj()->StaticUI()->GetType() == UI_TYPE::PUBLIC_SHOP_UI)
		m_bState = BT_STATE::SELL;

	else*/
		m_bState = BT_STATE::CLICKED;

}

void CButton::OnCollisionExit(CCollider* _pOther)
{
	switch (m_bState)
	{
	case BT_STATE::NONE:
		break;

	case BT_STATE::CLICKED:
		Transform()->SetLocalPos(m_vecOldPos);
		break;

	case BT_STATE::SELL:
		Transform()->SetLocalPos(m_vecOldPos);


		//m_eItemId == ITEM_ID::EMPTY;
		break;
	}

	m_bState = BT_STATE::NONE;
	m_bCheckActive = BT_ACTIVE::PASSIVE;
}

void CButton::ChangeButtonTexture()
{
	switch (m_eItemId)
	{
	case ITEM_ID::EMPTY:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"EMPTY");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"QuestBase").GetPointer());
		m_pExplainBox->SetUiRenderCheck(false);
		break;

	case ITEM_ID::BASIC_SWORD:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainSword").GetPointer());
		break;

	case ITEM_ID::AX:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"AX");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainAx").GetPointer());
		break;

	case ITEM_ID::BASIC_ARROW:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOW_IMG");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainSword").GetPointer());
		break;

	case ITEM_ID::BRANCH:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BRANCH");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainBranch").GetPointer());
		break;

	case ITEM_ID::APPLE:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"APPLE");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainApple").GetPointer());
		break;

	case ITEM_ID::BOTTLE_STAMINA:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOTTLE_STAMINA");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainBStamina").GetPointer());
		break;

	case ITEM_ID::BOTTLE_DASH:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOTTLE_DASH");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainBDash").GetPointer());
		break;

	case ITEM_ID::STEAK:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"STEAK");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainMeat").GetPointer());
		break;

	case ITEM_ID::MONEYBAG:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"MONEYBAG");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainMoneyBag").GetPointer());
		break;

	case ITEM_ID::CARROT:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"CARROT");
		m_pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ExplainCarrot").GetPointer());
		break;

	case ITEM_ID::END:
		return;
	}
	auto temp = m_pComParent->GetObj()->StaticUI()->GetButton();

	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, m_pItemImage.GetPointer());

}

void CButton::ChangeNumTexture()
{
	if(m_iItemCount < 100) {	
		int unit = m_iItemCount % 10;
		int tens = m_iItemCount / 10;
		// 1의 자리
		string str = to_string(unit);
		wstring wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::UNIT]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
		// 10의 자리
		str = to_string(tens);
		wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::TENS]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	}
	else if (m_iItemCount / 100 >= 1) {
		int unit = m_iItemCount % 10;
		int tens = (m_iItemCount / 10) % 10;
		int hundreds = (m_iItemCount / 10) / 10;
		// 1의 자리
		string str = to_string(unit);
		wstring wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::UNIT]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
		// 10의 자리
		str = to_string(tens);
		wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::TENS]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
		// 100의 자리
		str = to_string(hundreds);
		wstr = wstring(str.begin(), str.end());
		m_vItemNum[(UINT)ITEM_NUM::HUNDREDS]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
	}

	// moneybag이라면 돈을 넣어 주기
	if (m_eItemId == ITEM_ID::MONEYBAG)
		m_pComParent->GetObj()->StaticUI()->m_iMoney += uidMoney(dreMoney);

	m_bChangeCount = false;
}

void CButton::SetItemPrice()
{
	switch (m_eItemId)
	{
	case ITEM_ID::EMPTY:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"EMPTY");
		break;

	case ITEM_ID::BASIC_SWORD:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"SWORD");
		m_iItemCount = 900;
		break;

	case ITEM_ID::AX:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"AX");
		m_iItemCount = 550;
		break;

	case ITEM_ID::BOTTLE_STAMINA:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOTTLE_STAMINA");
		m_iItemCount = 25;
		break;

	case ITEM_ID::BOTTLE_DASH:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"BOTTLE_DASH");
		m_iItemCount = 15;
		break;

	case ITEM_ID::STEAK:
		m_pItemImage = CResMgr::GetInst()->FindRes<CTexture>(L"STEAK");
		m_iItemCount = 15;
		break;

	case ITEM_ID::END:
		return;
	}
}

void CButton::CreateExplainBox()
{
	Vector3 pos = GetObj()->Transform()->GetLocalPos();
	Vector3 Scale = Vector3(600.f, 150.f, 1.f);
	CGameObject* pExplainBox = new CGameObject;
	pExplainBox->SetName(L"Explain Box");
	pExplainBox->AddComponent(new CTransform);
	pExplainBox->AddComponent(new CMeshRender);
	pExplainBox->Transform()->SetLocalPos(Vector3(pos.x + (Scale.x / 2.f), pos.y - (Scale.y / 2.f), 1.f));
	pExplainBox->Transform()->SetLocalScale(Scale);
	//MeshRender 설정
	pExplainBox->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pExplainBox->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"WalletMtrl"));
	pExplainBox->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"QuestBase").GetPointer());
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pExplainBox);
	pExplainBox->SetUiRenderCheck(false);
	m_pExplainBox = pExplainBox;

}

void CButton::CreateItemNum(int num)
{
	tResolution res = CRenderMgr::GetInst()->GetResolution();

	for (int i = 0; i < num; ++i) {
		CGameObject* pItemNum = new CGameObject;
		pItemNum->SetName(L"ItemNum");
		pItemNum->FrustumCheck(false);
		pItemNum->AddComponent(new CTransform);
		pItemNum->AddComponent(new CMeshRender);
		pItemNum->Transform()->SetLocalPos(Vector3(-res.fWidth / 2, -res.fHeight / 2, 1.f));
		pItemNum->Transform()->SetLocalScale(Vector3(30.f, 30.f, 1.f));
		//MeshRender 설정
		pItemNum->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pItemNum->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"WalletMtrl"));
		pItemNum->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"0").GetPointer());
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pItemNum);

		m_vItemNum.push_back(pItemNum);
	}
}

CButton::CButton()
	: CScript((UINT)SCRIPT_TYPE::TESTSCRIPT)
	, m_eItemId(ITEM_ID::EMPTY)
	, m_bActive(false)
	, m_bState(BT_STATE::NONE)
	, m_bCheckActive(BT_ACTIVE::PASSIVE)
{
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