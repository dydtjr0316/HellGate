#include "pch.h"
#include "NpcScript.h"
#include "RenderMgr.h"
#include "ToolCamScript.h"

CNpcScript::CNpcScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT) //CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),
	, m_eReqState{ REQUEST_STATE::NOT_RECIEVE }
{
	// 0729ȿ��
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

	// ����ǥ �߰�
	SetReqMarkData(pMeshData->GetMesh(), pMeshData->GetMtrl());
	// ��ǳ�� �߰�
	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\Npc\\chat.fbx", FBX_TYPE::NPC);
	SetReqMarkData(pMeshData->GetMesh(), pMeshData->GetMtrl());
	// request state ����
	//m_eReqState = REQUEST_STATE::REQUESTING;

	// Conversation Box Create
	Vector3 vScale(1100.f, 230.f, 1.f);

	m_pConversationBox = new CGameObject;
	m_pConversationBox->SetName(L"Conversation_Box");
	m_pConversationBox->FrustumCheck(false);	// ����ü �ø� x
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

	//vector<REQUEST_STATE> vTemp;
	//// Quest bool�� �ʱ�ȭ
	//for (int i = 0; i < 2; ++i) {
	//	for (int j = 0; j < 2; ++j) {
	//		vTemp.push_back(REQUEST_STATE::NOT_RECIEVE);
	//	}
	//	m_vIsQuest.push_back(vTemp);
	//	vTemp.clear();
	//}

	// Quest bool�� �ʱ�ȭ
	for (int i = 0; i < 2; ++i) {
		m_vIsQuest.push_back(REQUEST_STATE::NOT_RECIEVE);
	}
}

CNpcScript::~CNpcScript()
{
}

void CNpcScript::update()
{
	CTransform* NpcTrans = GetObj()->Transform();
	Vector3 NpcPos = NpcTrans->GetLocalPos();
	Vector3 RequestPos{};
	Vector3 PlayerRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();


	RequestPos = Vector3(NpcPos.x, NpcPos.y + 150.f, NpcPos.z);
	m_pRequestMark->Transform()->SetLocalPos(RequestPos);

	if(m_eReqState == REQUEST_STATE::NOT_RECIEVE)
		m_pRequestMark->Transform()->SetLocalRot(PlayerRot + Vector3(0.0f, XM_PI, 0.f)); // - XM_PI / 2 ��ǳ���� �� x
	else if (m_eReqState == REQUEST_STATE::REQUESTING) {
		m_pRequestMark->Transform()->SetLocalRot(PlayerRot + Vector3(-XM_PI / 2, 0.f, 0.f)); // - XM_PI / 2 ��ǳ���� �� x
		m_pRequestMark->Transform()->SetLocalScale(Vector3(2.0f, 2.0f, 2.0f));
	}

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {

		POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();

		if (m_bIsTalk == true) { // �ʿ���� �� ������
			++m_iClickNum;

			if (m_iClickNum == 3) {	// 3�� ������ ������ �ɷ� ����
				SetCameraState(CAMERA_STATE::FIXED_CAMERA);
				SetAnimation(NPC_ANI_TYPE::IDLE);
				m_bisAniReset = false;
				m_pConversationBox->SetUiRenderCheck(false);
				m_bIsTalk = false;
				m_bIsCollision = false;
				m_iClickNum = 0;

				// -------���� �����ؾ� ��
				// 3�� ������ req mark ������� �ؾ� ��
				//m_pRequestMark->SetUiRenderCheck(false);
				// ����ǥ�� ����
				//m_eReqState = REQUEST_STATE::NOT_RECIEVE;
				//SetReqMarkMesh(m_eReqState); 
				// m_pRequestMark->Animator3D()->SetAniUse(true);
				// -----------------
			}
		}

		if (m_bIsCollision == true && m_bIsTalk == false) { // && ��ġ �� ���� �ϱ� (���⼱ ���� �� �ص� ������ �� ���⵵)
			// ��ǳ������ ����
			m_pRequestMark->Animator3D()->SetAniUse(false);
			m_eReqState = REQUEST_STATE::REQUESTING;
			SetReqMarkMesh(m_eReqState);
			m_bIsTalk = true;
			m_pConversationBox->SetUiRenderCheck(true);

			// ī�޶� ȸ��
			SetCameraState(CAMERA_STATE::NPC_CAMERA);
			AnimClipReset();
			SetAnimation(NPC_ANI_TYPE::TALK);

			// ù ��ȭ�� ���;� ��
			// Ŭ���� ������ �ٸ� ��ȭ
			// ������ ��ȭ ������ �̼� true�� 
		}

		// ��ȭ ���� �ؽ�Ʈ �ٲٱ�
		ChangeBoxTexture();
	}
}

void CNpcScript::DecideQuestType()
{
	if (m_vIsQuest[0] == REQUEST_STATE::NOT_RECIEVE && m_vIsQuest[1] == REQUEST_STATE::NOT_RECIEVE) {
		if (GetObj()->GetName() == L"Npc_1")
			m_eQuestType = NPC_QUEST::KILL_MONSTER;
		else if(GetObj()->GetName() == L"Npc_2")
			m_eQuestType = NPC_QUEST::BUY_WEAPON;
	}
	else if(m_vIsQuest[0] == REQUEST_STATE::REQUESTING && m_vIsQuest[1] == REQUEST_STATE::NOT_RECIEVE
		|| m_vIsQuest[0] == REQUEST_STATE::COMPLETE && m_vIsQuest[1] == REQUEST_STATE::REQUESTING){
		m_eQuestType = NPC_QUEST::WHY;
	}
	else if (m_vIsQuest[0] == REQUEST_STATE::REQUEST_RESOLUTION && m_vIsQuest[1] == REQUEST_STATE::NOT_RECIEVE
		|| m_vIsQuest[0] == REQUEST_STATE::COMPLETE && m_vIsQuest[1] == REQUEST_STATE::REQUEST_RESOLUTION) {
		m_eQuestType = NPC_QUEST::DONE;
	}
	else if (m_vIsQuest[0] == REQUEST_STATE::COMPLETE && m_vIsQuest[1] == REQUEST_STATE::NOT_RECIEVE) {
		if (GetObj()->GetName() == L"Npc_1")
			m_eQuestType = NPC_QUEST::GET_ITEM;
		else if (GetObj()->GetName() == L"Npc_2")
			m_eQuestType = NPC_QUEST::BUY_POTION;
	}
}

void CNpcScript::ChangeBoxTexture()
{
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
				break;
			}
		}
		else if (m_eQuestType == NPC_QUEST::WHY) {
			m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_why").GetPointer());
		}
		else if (m_eQuestType == NPC_QUEST::DONE) {
			m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"npc1_done").GetPointer());
		}
	}
	// NPC_2
	else if (GetObj()->GetName() == L"Npc_2") {
		if (m_eQuestType == NPC_QUEST::BUY_WEAPON) {
			switch (m_iClickNum) {
			case 0:
				cout << "1" << endl;
				break;
			case 1:
				break;
			case 2:
				break;
			}
		}
		else if (m_eQuestType == NPC_QUEST::BUY_POTION) {
			switch (m_iClickNum) {
			case 0:
				cout << "1" << endl;
				break;
			case 1:
				break;
			case 2:
				break;
			}
		}
		else if (m_eQuestType == NPC_QUEST::WHY) {

		}
		else if (m_eQuestType == NPC_QUEST::DONE) {

		}
	}

	//NPC_3
	else if (GetObj()->GetName() == L"Npc_3") {

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
	// ��ȭ�ϰڴٴ� �ؽ�Ʈ ����
	
	// Ŭ���ϸ� ��ȭ ���� ���� (istalk true�� �����) -> SetUiRenderCheck true��
	m_bIsCollision = true;
}

void CNpcScript::OnCollisionExit(CCollider* _pOther)
{
	cout << "����" << endl;
}

void CNpcScript::SetCameraState(CAMERA_STATE _eCamState)
{

	CGameObject* cam;
	vector<CGameObject*> objects;
	objects = CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Default")->GetObjects();

	vector<CGameObject*>::iterator iter = objects.begin();

	for (; iter != objects.end();) {
		if ((*iter)->GetName() == L"MainCam")
 			cam = *iter;
		++iter;
	}

	Vector3 npcPos = GetObj()->Transform()->GetLocalPos();

	CToolCamScript* camScript = cam->GetScript<CToolCamScript>();

	if (_eCamState == CAMERA_STATE::FIXED_CAMERA) {
		//camScript->DeleteCamParent(true);
		cam->ClearParent();
		camScript->GetTestObj()->ClearParent();
		camScript->SetIsChild(false);
		camScript->ResetNpcCamAngle();
	}

	camScript->SetNpcPos(npcPos);
	camScript->SetCamState(_eCamState);
	
}