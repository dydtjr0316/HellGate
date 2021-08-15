#include "pch.h"
#include "NpcScript.h"
#include "RenderMgr.h"
#include "Quest.h"
#include "StaticUI.h"
#include "Button.h"
#include "ToolCamScript.h"
#include "PlayerScript.h"

CNpcScript::CNpcScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT) //CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),
	, m_eReqState{ REQUEST_STATE::NOT_RECIEVE }
	, m_vPlayerQuest{}
	, m_pStoreUi{ NULL }
{
	// 0729효림
	// Request Box Create
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Npc\\exclamation_mark@ani.fbx", FBX_TYPE::NPC);
	
	m_pRequestMark = new CGameObject;
	m_pRequestMark = pMeshData->Instantiate();
	m_pRequestMark->SetName(L"Request_Box");
	m_pRequestMark->FrustumCheck(false);
	m_pRequestMark->Transform()->SetLocalScale(Vector3(5.f, 5.f, 5.f));
	m_pRequestMark->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	
	// add GameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Npc")->AddGameObject(m_pRequestMark);

	// 느낌표 추가
	SetReqMarkData(pMeshData->GetMesh(), pMeshData->GetMtrl());
	// 말풍선 추가
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Npc\\chat.fbx", FBX_TYPE::NPC);
	SetReqMarkData(pMeshData->GetMesh(), pMeshData->GetMtrl());
	// request state 설정
	//m_eReqState = REQUEST_STATE::REQUESTING;

	// Conversation Box Create
	Vector3 vScale(1100.f, 230.f, 1.f);

	m_pConversationBox = new CGameObject;
	m_pConversationBox->SetName(L"Conversation_Box");
	m_pConversationBox->FrustumCheck(false);	// 절두체 컬링 x
	m_pConversationBox->AddComponent(new CTransform);
	m_pConversationBox->AddComponent(new CMeshRender);

	tResolution res = CRenderMgr::GetInst()->GetResolution();
	m_pConversationBox->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 90.f
		, -(res.fHeight / 2.f) + (vScale.y / 2.f) + 30.f
		, 1.f));
	m_pConversationBox->Transform()->SetLocalScale(vScale);
	m_pConversationBox->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	m_pConversationBox->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"CoversationMtrl"));
	m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UiBoard").GetPointer());
	m_pConversationBox->SetUiRenderCheck(false);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pConversationBox);

	// Quest bool값 초기화
	for (int i = 0; i < 2; ++i) {
		m_vIsQuest.push_back(REQUEST_STATE::NOT_RECIEVE);
	}

}

CNpcScript::~CNpcScript()
{
}

void CNpcScript::init()
{
	// ui 만들기
	CGameObject* storeUi = new CGameObject;
	storeUi->SetName(L"Store UI Object");
	storeUi->FrustumCheck(false);
	storeUi->AddComponent(new CTransform);
	storeUi->AddComponent(new CMeshRender);
	storeUi->AddComponent(new CStaticUI);
	storeUi->StaticUI()->init(UI_TYPE::PUBLIC_SHOP_UI);
	//storeUi->StaticUI()->CreatePickingObj();
	// 투영행렬 statiUI 컴포넌트에 등록 (ORTHOGRAPHIC 카메라 정보 필요)
	m_pCam = FindCam(L"UiCam", L"Default");
	storeUi->StaticUI()->SetCameraProj(m_pCam->Camera());
	
	// Transform 설정
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	storeUi->Transform()->SetLocalPos(Vector3(-400.f, 80.f, 1.f));
	storeUi->Transform()->SetLocalScale(Vector3(600.f, 800.f, 1.f));
	// MeshRender 설정	
	storeUi->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"UITexMtrl");
	storeUi->MeshRender()->SetMaterial(pMtrl->Clone());
	Ptr<CTexture> itemUI = storeUi->StaticUI()->m_pFrame;
	float fUI = 0.5f;
	storeUi->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, itemUI.GetPointer());
	storeUi->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fUI);
	
	// AddGameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(storeUi);
	m_pStoreUi = storeUi;

	//	Static Ui에 상속된 버튼들 Scene에 Obj로 추가
	Vector3 vScale = Vector3(80.f, 120.f, 1.f);
	Vector3 vObjectPos = storeUi->Transform()->GetLocalPos();
	Vector3 vObjectScale = storeUi->Transform()->GetLocalScale();
	float	fEmptyY = (vObjectScale.y - 100.f - (vScale.y * 4.f)) / 5.f;
	float	fEmptyX = (vObjectScale.x - (vScale.x * 4.f)) / 5.f;
	storeUi->StaticUI()->CreateStoreButton();
	storeUi->StaticUI()->m_StoreButton[(UINT)STORE_BUTTON::EXIT]->Transform()->SetLocalPos(Vector3(vObjectPos.x, vObjectPos.y - (vObjectScale.y / 2.f) + 50.f, 1.f));
	storeUi->StaticUI()->m_StoreButton[(UINT)STORE_BUTTON::EXIT]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"EXIT").GetPointer());
	for (int i = 0; i < storeUi->StaticUI()->m_vecButton.size(); ++i)
	{
		Ptr<CTexture> itemUI = CResMgr::GetInst()->FindRes<CTexture>(L"ItemUiTex");
		CGameObject* pButtonObj = new CGameObject;
		pButtonObj->SetName(L"Button Object");
		pButtonObj->FrustumCheck(false);// 절두체 컬링 사용하지 않음
		pButtonObj->AddComponent(new CTransform);
		pButtonObj->AddComponent(new CMeshRender);
		pButtonObj->AddComponent(new CCollider);
		pButtonObj->Collider()->SetColliderType(COLLIDER_TYPE::RECT);
		//	버튼 Script 설정
		pButtonObj->AddComponent(storeUi->StaticUI()->m_vecButton[i]);
		storeUi->StaticUI()->m_vecButton[i]->SetParent(storeUi->StaticUI());
		storeUi->StaticUI()->m_vecButton[i]->SetUiType(UI_TYPE::PUBLIC_SHOP_UI);
		// Transform 설정
		tResolution res = CRenderMgr::GetInst()->GetResolution();
		Vector3 result = Vector3(vObjectPos.x - (vObjectScale.x / 2.f) + (vScale.x / 2.f + fEmptyX) + ((vScale.x + fEmptyX) * (i % 4))
			, vObjectPos.y + (vObjectScale.y / 2.f) - (vScale.y / 2.f + fEmptyY) - ((vScale.y + fEmptyY) * (i / 4))
			, 1.f);
		pButtonObj->Transform()->SetLocalPos(result);
		pButtonObj->Transform()->SetLocalScale(vScale);
		// MeshRender 설정
		pButtonObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TexMtrl");
		pButtonObj->MeshRender()->SetMaterial(pMtrl->Clone());
		pButtonObj->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, storeUi->StaticUI()->m_vecButton[i]->GetImage().GetPointer());
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pButtonObj);
	}

	CGameObject* vecTemp = FindCam(L"MousePoint", L"PUI");
	storeUi->StaticUI()->m_pMousePoint = vecTemp;

	// 팔 item 설정
	storeUi->StaticUI()->SetButton(ITEM_ID::BASIC_SWORD);
	storeUi->StaticUI()->SetButton(ITEM_ID::AX);
	storeUi->StaticUI()->SetButton(ITEM_ID::BOTTLE_STAMINA);
	storeUi->StaticUI()->SetButton(ITEM_ID::BOTTLE_DASH);
	storeUi->StaticUI()->SetButton(ITEM_ID::STEAK);

	for (int i = 0; i < storeUi->StaticUI()->m_vecButton.size(); ++i)
		storeUi->StaticUI()->m_vecButton[i]->init();

	for (int i = 0; i < storeUi->StaticUI()->m_vecButton.size(); ++i)
		storeUi->StaticUI()->m_vecButton[i]->CreateExplainBox();

}

void CNpcScript::update()
{
	//if (g_myid < 0)return;
	CTransform* NpcTrans = GetObj()->Transform();
	Vector3 NpcPos = NpcTrans->GetLocalPos();
	Vector3 RequestPos{};
	Vector3 PlayerRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();


	RequestPos = Vector3(NpcPos.x, NpcPos.y + 150.f, NpcPos.z);
	m_pRequestMark->Transform()->SetLocalPos(RequestPos);

	if(m_eReqState == REQUEST_STATE::NOT_RECIEVE)
		m_pRequestMark->Transform()->SetLocalRot(PlayerRot + Vector3(0.0f, XM_PI, 0.f)); // - XM_PI / 2 말풍선일 때 x
	else if (m_eReqState == REQUEST_STATE::REQUESTING) {
		m_pRequestMark->Transform()->SetLocalRot(PlayerRot + Vector3(-XM_PI / 2, 0.f, 0.f)); // - XM_PI / 2 말풍선일 때 x
		m_pRequestMark->Transform()->SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));
	}

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {

		POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();

		if (m_bIsTalk == true) { // 필요없을 것 같은데
			++m_iClickNum;

			if ((m_eQuestType != NPC_QUEST::DONE || m_eQuestType != NPC_QUEST::WHY) && m_iClickNum == 3 && (GetObj()->GetName() != L"Npc_3")) {	// 3번 누르면 나가는 걸로 가정
				SetCameraState(CAMERA_STATE::FIXED_CAMERA);
				SetAnimation(NPC_ANI_TYPE::IDLE);
				m_bisAniReset = false;
				m_pConversationBox->SetUiRenderCheck(false);
				m_bIsTalk = false;
				m_bIsCollision = false;
				m_iClickNum = 0;
				m_bQuestBox = true;

				// -------로직 생각해야 함
				// 3번 누르면 req mark 사라지게 해야 함
				//m_pRequestMark->SetUiRenderCheck(false);
				// 느낌표로 변경
				//m_eReqState = REQUEST_STATE::NOT_RECIEVE;
				//SetReqMarkMesh(m_eReqState); 
				// m_pRequestMark->Animator3D()->SetAniUse(true);
				// -----------------
			}
			else if ((m_eQuestType == NPC_QUEST::DONE || m_eQuestType == NPC_QUEST::WHY) && m_iClickNum == 1 && (GetObj()->GetName() != L"Npc_3"))
			{
				SetCameraState(CAMERA_STATE::FIXED_CAMERA);
				SetAnimation(NPC_ANI_TYPE::IDLE);
				m_bisAniReset = false;
				m_pConversationBox->SetUiRenderCheck(false);
				m_bIsTalk = false;
				m_bIsCollision = false;
				m_iClickNum = 0;
				m_bQuestBox = true;
			}
			else if (m_eQuestType == NPC_QUEST::STORE) {
				if (m_iClickNum == 1) {
					SetStaticUiRender(true);
					m_pConversationBox->SetUiRenderCheck(false);
					m_pPlayerUi->StaticUI()->SetWalletMoney();
				}
				
				Vector3 pos = m_pStoreUi->StaticUI()->m_StoreButton[(UINT)STORE_BUTTON::EXIT]->Transform()->GetLocalPos();
				Vector3 scale = m_pStoreUi->StaticUI()->m_StoreButton[(UINT)STORE_BUTTON::EXIT]->Transform()->GetLocalScale();
				if (ComputeMousePos(pos, scale)) {
					SetCameraState(CAMERA_STATE::FIXED_CAMERA);
					SetAnimation(NPC_ANI_TYPE::IDLE);
					m_bisAniReset = false;
					m_pConversationBox->SetUiRenderCheck(false);
					m_bIsTalk = false;
					m_bIsCollision = false;
					m_iClickNum = 0;
					m_bQuestBox = true;
					SetStaticUiRender(false);
				}
				
			}
		}

		if (m_bIsCollision == true && m_bIsTalk == false) { // && 위치 값 같게 하기 (여기선 굳이 안 해도 ㄱㅊ을 것 같기도)
			// 말풍선으로 변경
			m_pRequestMark->Animator3D()->SetAniUse(false);
			m_eReqState = REQUEST_STATE::REQUESTING;
			SetReqMarkMesh(m_eReqState);
			m_bIsTalk = true;
			m_pConversationBox->SetUiRenderCheck(true);

			// 카메라 회전
			SetCameraState(CAMERA_STATE::NPC_CAMERA);
			m_bGetParentCamera = false;
			AnimClipReset();
			SetAnimation(NPC_ANI_TYPE::TALK);
			

			// 첫 대화가 나와야 함
			// 클릭할 때마다 다른 대화
			// 마지막 대화 끝나면 미션 true로 
		}

		// 대화 상자 텍스트 바꾸기
		if (m_bIsCollision == true)
			ChangeBoxTexture();

		/*if ((m_vIsQuest[0] == REQUEST_STATE::COMPLETE || m_vIsQuest[1] == REQUEST_STATE::COMPLETE)
			&& m_iClickNum == 1)
			m_pPlayer->StaticUI()->m_iMoney += 300;*/
	}

	if (KEY_TAB(KEY_TYPE::KEY_RBTN) && GetObj()->GetName() == L"Npc_3" && (m_bIsTalk == true)) {
		SellAndBuy();
	}
}

void CNpcScript::SetStaticUiRender(bool _bool)
{
	m_pStoreUi->StaticUI()->m_bActive = _bool;
	m_pPlayerUi = m_pPlayer->GetScript<CPlayerScript>()->GetUIObj();
	m_pPlayerUi->StaticUI()->m_bActive = _bool;

	for (int i = 0; i < 16; ++i) {
		m_pStoreUi->StaticUI()->m_vecButton[i]->SetActive(_bool);
		m_pPlayerUi->StaticUI()->m_vecButton[i]->SetActive(_bool);
	}

	m_pStoreUi->StaticUI()->m_StoreButton[(UINT)STORE_BUTTON::EXIT]->SetUiRenderCheck(_bool);
}

void CNpcScript::SellAndBuy()
{
	int playerMoney = m_pPlayerUi->StaticUI()->m_iMoney;

	CStaticUI* storeUi = m_pStoreUi->StaticUI();
	CStaticUI* playerUi = m_pPlayerUi->StaticUI();

	// store
	if (ComputeMousePos((Vector3)m_pStoreUi->Transform()->GetLocalPos(), (Vector3)m_pStoreUi->Transform()->GetLocalScale())) {
		for (int i = 0; i < storeUi->m_vecButton.size(); ++i) {
			if (storeUi->m_vecButton[i]->GetItemID() != ITEM_ID::EMPTY) {
				Vector3 ButtonPos = storeUi->m_vecButton[i]->GetObj()->Transform()->GetLocalPos();
				Vector3 ButtonScale = storeUi->m_vecButton[i]->GetObj()->Transform()->GetLocalScale();

				if (ComputeMousePos(ButtonPos, ButtonScale)) {
					if (playerMoney - storeUi->m_vecButton[i]->GetItemCount() >= 0) {
						playerUi->SetButton(storeUi->m_vecButton[i]->GetItemID());
						playerUi->m_iMoney -= storeUi->m_vecButton[i]->GetItemCount();
						playerUi->SetWalletMoney();

						if (m_pPlayer->Quest()->GetDoQuest(QUEST_TYPE::BUY_POTION) == false ||
							m_pPlayer->Quest()->GetDoQuest(QUEST_TYPE::BUY_WEAPON) == false) {
							// stamina potion
							switch (storeUi->m_vecButton[i]->GetItemID()) {
							case ITEM_ID::BOTTLE_STAMINA:
								m_pPlayer->Quest()->AddQuestcount(QUEST_TYPE::BUY_POTION);
								break;
							case ITEM_ID::AX:	// 다른 무기 추가해도 ㄱㅊ을 듯
								m_pPlayer->Quest()->AddQuestcount(QUEST_TYPE::BUY_WEAPON);
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
	}
	// player
	else {
		for (int i = 0; i < playerUi->m_vecButton.size(); ++i) {
			if (playerUi->m_vecButton[i]->GetItemID() != ITEM_ID::EMPTY) {
				Vector3 ButtonPos = playerUi->m_vecButton[i]->GetObj()->Transform()->GetLocalPos();
				Vector3 ButtonScale = playerUi->m_vecButton[i]->GetObj()->Transform()->GetLocalScale();

				if (ComputeMousePos(ButtonPos, ButtonScale)) {
					if (playerUi->m_vecButton[i]->GetItemCount() > 1) {
						playerUi->m_vecButton[i]->SubItemCount();
						playerUi->SetWalletMoney();
						playerUi->m_iMoney += 10;		// 팔고 돈 올리기
					}
					else if (playerUi->m_vecButton[i]->GetItemCount() == 1) {
						playerUi->m_vecButton[i]->SetItemID(ITEM_ID::EMPTY);
						playerUi->m_vecButton[i]->SubItemCount();
						playerUi->SetWalletMoney();
						playerUi->m_iMoney += 10;
					}
				}
			}
		}
	}
}

bool CNpcScript::ComputeMousePos(Vector3& _pos, Vector3& _scale)
{
	POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	pMousePos.x -= res.fWidth / 2.f;
	pMousePos.y -= res.fHeight / 2.f;
	pMousePos.y = -pMousePos.y;

	if (pMousePos.x >= _pos.x - (_scale.x / 2) && pMousePos.x <= _pos.x + (_scale.x / 2)
		&& pMousePos.y <= _pos.y + (_scale.y / 2) && pMousePos.y >= _pos.y - (_scale.y / 2))
		return true;
	return false;
}


void CNpcScript::CheckPlayer()
{
	m_vPlayerQuest = m_pPlayer->Quest()->GetQuestCheck();

	if (m_vPlayerQuest[(UINT)QUEST_TYPE::KILL_MONSTER] >= 3 && GetObj()->GetName() == L"Npc_1") {
		m_vIsQuest[0] = REQUEST_STATE::REQUEST_RESOLUTION;
		m_vPlayerQuest[(UINT)QUEST_TYPE::KILL_MONSTER] = 0;
		m_pPlayer->Quest()->ResetQuestCheck(QUEST_TYPE::KILL_MONSTER);
		m_pPlayer->Quest()->SetDoQuest(QUEST_TYPE::KILL_MONSTER, true);
	}
	else if (m_vIsQuest[1] == REQUEST_STATE::REQUESTING && m_vPlayerQuest[(UINT)QUEST_TYPE::GET_ITEM] >= 3 && GetObj()->GetName() == L"Npc_1") {
		m_vIsQuest[1] = REQUEST_STATE::REQUEST_RESOLUTION;
		m_pPlayer->Quest()->SetDoQuest(QUEST_TYPE::GET_ITEM, true);
	}
	else if (m_vIsQuest[0] == REQUEST_STATE::REQUESTING && m_vPlayerQuest[(UINT)QUEST_TYPE::BUY_POTION] >= 3 && GetObj()->GetName() == L"Npc_2") {
		m_vIsQuest[0] = REQUEST_STATE::REQUEST_RESOLUTION;
		m_pPlayer->Quest()->SetDoQuest(QUEST_TYPE::BUY_POTION, true);
	}
	else if (m_vIsQuest[1] == REQUEST_STATE::REQUESTING && m_vPlayerQuest[(UINT)QUEST_TYPE::BUY_WEAPON] >= 1 && GetObj()->GetName() == L"Npc_2") {
		m_vIsQuest[1] = REQUEST_STATE::REQUEST_RESOLUTION;
		m_pPlayer->Quest()->SetDoQuest(QUEST_TYPE::BUY_WEAPON, true);
	}
	

}


void CNpcScript::SetQuestBox(wstring wstr, QUESTBOX_TYPE _eType)
{
	if (m_bQuestBox) {
		for (int i = 0; i < 2; ++i) {
			if (m_pPlayer->Quest()->m_vExistQuestBox[i] == QUESTBOX_TYPE::EMPTY || m_pPlayer->Quest()->m_vExistQuestBox[i] == _eType) {
				m_pPlayer->Quest()->m_pQuestBox[i]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(wstr).GetPointer());
				m_pPlayer->Quest()->m_vExistQuestBox[i] = _eType;
				m_bQuestBox = false;
				return;
			}
		}
	}
}



void CNpcScript::DecideQuestType()
{
	if (m_vIsQuest[0] == REQUEST_STATE::NOT_RECIEVE && m_vIsQuest[1] == REQUEST_STATE::NOT_RECIEVE) {
		if (GetObj()->GetName() == L"Npc_1") {
			m_eQuestType = NPC_QUEST::KILL_MONSTER; SetQuestBox(L"MonsterKill", QUESTBOX_TYPE::NPC_1);
		}
		else if (GetObj()->GetName() == L"Npc_2") {
			m_eQuestType = NPC_QUEST::BUY_POTION; SetQuestBox(L"BuyPotion", QUESTBOX_TYPE::NPC_2);
		}
	}
	else if(m_vIsQuest[0] == REQUEST_STATE::REQUESTING && m_vIsQuest[1] == REQUEST_STATE::NOT_RECIEVE
		|| m_vIsQuest[0] == REQUEST_STATE::COMPLETE && m_vIsQuest[1] == REQUEST_STATE::REQUESTING){
		m_eQuestType = NPC_QUEST::WHY;
	}
	else if (m_vIsQuest[0] == REQUEST_STATE::REQUEST_RESOLUTION && m_vIsQuest[1] == REQUEST_STATE::NOT_RECIEVE
		|| m_vIsQuest[0] == REQUEST_STATE::COMPLETE && m_vIsQuest[1] == REQUEST_STATE::REQUEST_RESOLUTION) {
		m_eQuestType = NPC_QUEST::DONE;
		if (GetObj()->GetName() == L"Npc_1") 
			SetQuestBox(L"QuestBase", QUESTBOX_TYPE::NPC_1);
		else if (GetObj()->GetName() == L"Npc_2") 
			SetQuestBox(L"QuestBase", QUESTBOX_TYPE::NPC_2);
	}
	else if (m_vIsQuest[0] == REQUEST_STATE::COMPLETE && m_vIsQuest[1] == REQUEST_STATE::NOT_RECIEVE) {
		if (GetObj()->GetName() == L"Npc_1") {
			m_eQuestType = NPC_QUEST::GET_ITEM; SetQuestBox(L"GetItem", QUESTBOX_TYPE::NPC_1);
		}
		else if (GetObj()->GetName() == L"Npc_2") {
			m_eQuestType = NPC_QUEST::BUY_WEAPON; SetQuestBox(L"BuyWeapone", QUESTBOX_TYPE::NPC_2);
		}
	}
}

void CNpcScript::ChangeBoxTexture()
{
	CheckPlayer();
	DecideQuestType();

	//NPC_1
	if (GetObj()->GetName() == L"Npc_1") {

		if (m_eQuestType == NPC_QUEST::KILL_MONSTER) {
			switch (m_iClickNum) {
			case 0:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_quest1(1)").GetPointer());
				break;
			case 1:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_quest1(2)").GetPointer());
				break;
			case 2:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_quest1(3)").GetPointer());
				m_vIsQuest[0] = REQUEST_STATE::REQUESTING;
				break;
			}

			
		}
		else if (m_eQuestType == NPC_QUEST::GET_ITEM) {
			switch (m_iClickNum) {
			case 0:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_quest2(1)").GetPointer());
				break;
			case 1:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_quest2(2)").GetPointer());
				break;
			case 2:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_quest2(3)").GetPointer());
				m_vIsQuest[1] = REQUEST_STATE::REQUESTING;
				break;
			}
		}
		else if (m_eQuestType == NPC_QUEST::WHY) {
			m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_why").GetPointer());
		}
		else if (m_eQuestType == NPC_QUEST::DONE) {
			m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_done").GetPointer());
			if(m_vIsQuest[0] == REQUEST_STATE::REQUEST_RESOLUTION)
				m_vIsQuest[0] = REQUEST_STATE::COMPLETE;
			else if(m_vIsQuest[0] == REQUEST_STATE::COMPLETE)
				m_vIsQuest[1] = REQUEST_STATE::COMPLETE;
		}
	}
	// NPC_2
	else if (GetObj()->GetName() == L"Npc_2") {
		if (m_eQuestType == NPC_QUEST::BUY_POTION) {
			switch (m_iClickNum) {
			case 0:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc2_quest1(1)").GetPointer());
				break;
			case 1:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc2_quest1(2)").GetPointer());
				break;
			case 2:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc2_quest1(3)").GetPointer());
				m_vIsQuest[0] = REQUEST_STATE::REQUESTING;
				break;
			}
		}
		else if (m_eQuestType == NPC_QUEST::BUY_WEAPON) {
			switch (m_iClickNum) {
			case 0:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc2_quest2(1)").GetPointer());
				break;
			case 1:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc2_quest2(2)").GetPointer());
				break;
			case 2:
				m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc2_quest2(3)").GetPointer());
				m_vIsQuest[1] = REQUEST_STATE::REQUESTING;
				break;
			}
		}
		else if (m_eQuestType == NPC_QUEST::WHY) {
			m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc2_why").GetPointer());
		}
		else if (m_eQuestType == NPC_QUEST::DONE) {
			m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc2_done").GetPointer());
			if (m_vIsQuest[0] == REQUEST_STATE::REQUEST_RESOLUTION)
				m_vIsQuest[0] = REQUEST_STATE::COMPLETE;
			else if (m_vIsQuest[0] == REQUEST_STATE::COMPLETE)
				m_vIsQuest[1] = REQUEST_STATE::COMPLETE;
		}
	}

	//NPC_3
	else if (GetObj()->GetName() == L"Npc_3") {
		m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc3_start").GetPointer());
		m_eQuestType = NPC_QUEST::STORE;
	}
}

void CNpcScript::Move()
{
	CGameObject* npc = GetObj();
	CTransform* npcTrans = npc->Transform();
	Vector3 npcPos = npcTrans->GetLocalPos();
	Vector3 worldDir;
	CNpcScript* npcScript = npc->GetScript<CNpcScript>();
}

void CNpcScript::SetReqMarkMesh(const REQUEST_STATE& _eType)
{
	m_pRequestMark->MeshRender()->SetMesh(m_ReqMarkMeshData[(UINT)_eType]);
	m_pRequestMark->MeshRender()->SetMaterial(m_ReqMarkMtrlData[(UINT)_eType]);
}


void CNpcScript::SetAnimation(const NPC_ANI_TYPE& type)
{
	GetObj()->Animator3D()->SetBones(m_pAniData[(int)type]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(m_pAniData[(int)type]->GetAnimClip());
	GetObj()->MeshRender()->SetMesh(m_pAniData[(int)type]);
}

void CNpcScript::AnimClipReset()
{
	if (m_bisAniReset == false) {
		GetObj()->Animator3D()->SetClipTime(0, 0.f);
		m_bisAniReset = true;
	}
}

void CNpcScript::OnCollisionEnter(CCollider* _pOther)
{
	// 대화하겠다는 텍스트 띄우기
	
	// 클릭하면 대화 상자 띄우기 (istalk true로 만들기) -> SetUiRenderCheck true로
	m_bIsCollision = true;
	m_pPlayer = _pOther->GetObj();
}

void CNpcScript::OnCollisionExit(CCollider* _pOther)
{
	
}

void CNpcScript::SetCameraState(CAMERA_STATE _eCamState)
{
	m_pCam = FindCam(L"MainCam", L"Default");

	Vector3 npcPos = GetObj()->Transform()->GetLocalPos();

	CToolCamScript* camScript = m_pCam->GetScript<CToolCamScript>();

	if (_eCamState == CAMERA_STATE::FIXED_CAMERA) {
		//camScript->DeleteCamParent(true);
		m_pCam->ClearParent();
		camScript->GetTestObj()->ClearParent();
		camScript->SetIsChild(false);
		camScript->ResetNpcCamAngle();
	}

	camScript->SetNpcPos(npcPos);
	camScript->SetCamState(_eCamState);
	
}

CGameObject* CNpcScript::FindCam(wstring _wstr, wstring _Layerwstr)
{
	CGameObject* cam;
	vector<CGameObject*> objects;
	if (m_bGetParentCamera)
		objects = CSceneMgr::GetInst()->GetCurScene()->FindLayer(_Layerwstr)->GetParentObj();
	else if (!m_bGetParentCamera)
		objects = CSceneMgr::GetInst()->GetCurScene()->FindLayer(_Layerwstr)->GetObjects(); m_bGetParentCamera = true;
	
	vector<CGameObject*>::iterator iter = objects.begin();

	for (; iter != objects.end();) {
		if ((*iter)->GetName() == _wstr)
			cam = *iter;
		++iter;
	}

	return cam;
}