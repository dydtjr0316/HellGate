#include "pch.h"
#include "NpcScript.h"
#include "RenderMgr.h"
#include "ToolCamScript.h"

CNpcScript::CNpcScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT) //CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT),
	, m_eReqState{ REQUEST_STATE::NOT_RECIEVE }
{
	// Request Box Create
	m_pRequestMark = new CGameObject;
	m_pRequestMark->SetName(L"Request_Box");
	m_pRequestMark->FrustumCheck(false);
	m_pRequestMark->AddComponent(new CTransform);
	m_pRequestMark->AddComponent(new CMeshRender);
	m_pRequestMark->Transform()->SetLocalScale(Vector3(10.f, 20.f, 10.f));
	m_pRequestMark->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));
	m_pRequestMark->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	m_pRequestMark->MeshRender()->SetMaterial(pMtrl->Clone());
	// add GameObject
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Npc")->AddGameObject(m_pRequestMark);

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
	m_pConversationBox->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));
	m_pConversationBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UiBoard").GetPointer());
	m_pConversationBox->SetUiRenderCheck(false);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pConversationBox);
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


	RequestPos = Vector3(NpcPos.x, NpcPos.y + 200.f, NpcPos.z);
	m_pRequestMark->Transform()->SetLocalPos(RequestPos);
	m_pRequestMark->Transform()->SetLocalRot(PlayerRot + Vector3(0.f, XM_PI, 0.f));

	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {

		POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();

		if (m_bIsCollision == true && m_bIsTalk == false) { // && 위치 값 같게 하기 (여기선 굳이 안 해도 ㄱㅊ을 것 같기도)
			m_bIsTalk = true;
			m_pConversationBox->SetUiRenderCheck(true);
			// 첫 대화가 나와야 함
			// 클릭할 때마다 다른 대화
			// 마지막 대화 끝나면 

			// 카메라 회전
			SetCameraState(CAMERA_STATE::NPC_CAMERA);
			AnimClipReset();
			SetAnimation(NPC_ANI_TYPE::TALK);
			
		}

		if (m_bIsTalk == true) { // 필요없을 것 같은데
			++m_iClickNum;

			if (m_iClickNum == 3) {	// 3번 누르면 나가는 걸로 가정
				SetCameraState(CAMERA_STATE::FIXED_CAMERA);
				SetAnimation(NPC_ANI_TYPE::IDLE);
				m_bisAniReset = false;
				m_pConversationBox->SetUiRenderCheck(false);
				m_bIsTalk = false;
				m_bIsCollision = false;
				m_iClickNum = 0;
			}
		}

	}
}

void CNpcScript::SetReqMarkMesh(const REQUEST_STATE& _eType)
{
	GetObj()->MeshRender()->SetMesh(m_ReqMarkMeshData[(UINT)_eType]);
	
	for (int i = 0; i < m_ReqMarkMtrlData[(UINT)_eType].size(); ++i) {
		GetObj()->MeshRender()->SetMaterial(m_ReqMarkMtrlData[(UINT)_eType][i], i);
	}
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
}

void CNpcScript::OnCollisionExit(CCollider* _pOther)
{
	cout << "나감" << endl;
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
	}

	camScript->SetNpcPos(npcPos);
	camScript->SetCamState(_eCamState);


	
}