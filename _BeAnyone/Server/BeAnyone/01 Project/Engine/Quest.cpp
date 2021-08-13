#include "pch.h"
#include "Quest.h"
#include "Transform.h"
#include "MeshRender.h"
#include "ResMgr.h"
#include "RenderMgr.h"
// ㅅㅂ 뭐지 얘네 왜 넣어야 함?
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"

CQuest::CQuest()
	:CComponent(COMPONENT_TYPE::QUEST)
{
	tResolution res = CRenderMgr::GetInst()->GetResolution();
	Vector3	QuestBoxScale = Vector3(300.f, 400.f, 1.f);
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"QuestBoxBase");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vector3((res.fWidth / 2.f) - (QuestBoxScale.x / 2.f), (res.fHeight / 2.f) - (QuestBoxScale.y / 2.f) - 30.f, 1.f));
	pObject->Transform()->SetLocalScale(QuestBoxScale);
	//MeshRender 설정
	float fUI = 1.0f;
	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UITexMtrl"));
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"QuestBase").GetPointer());
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fUI);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
	m_pQuestBoxBase = pObject;
	for (int i = 0; i < (UINT)QUEST_TYPE::END; ++i) {
		m_vQuestCheck.push_back(0);
		m_bDoQuest[i] = false;
	}
	
	Vector3 QuestBasePos = m_pQuestBoxBase->Transform()->GetLocalPos();
	for (int i = 0; i < 2; ++i) {
		QuestBoxScale = Vector3(300.f, 60.f, 1.f);
		pObject = new CGameObject;
		pObject->SetName(L"QuestBox");
		pObject->FrustumCheck(false);
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->Transform()->SetLocalPos(Vector3(QuestBasePos.x, QuestBasePos.y + 200.f - (QuestBoxScale.y / 2.f) - (i * QuestBoxScale.y), 1.f));
		pObject->Transform()->SetLocalScale(QuestBoxScale);
		//MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"QuestBoxMtrl"));
		pObject->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"QuestBase").GetPointer());
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);
		m_pQuestBox.push_back(pObject);
		m_vExistQuestBox.push_back(QUESTBOX_TYPE::EMPTY);
	}
}

CQuest::~CQuest()
{
	
}

void CQuest::update()
{
	// KIIL_MONSTER
	if (m_vQuestCheck[(UINT)QUEST_TYPE::KILL_MONSTER] == 3) {
		for (int i = 0; i < 2; ++i) {
			if(m_vExistQuestBox[i] == QUESTBOX_TYPE::NPC_1)
				m_pQuestBox[i]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"MonsterKill_Complete").GetPointer());
		}

		
	}
	// Get item
	if (m_vQuestCheck[(UINT)QUEST_TYPE::GET_ITEM] >= 3) {
		for (int i = 0; i < 2; ++i) {
			if (m_vExistQuestBox[i] == QUESTBOX_TYPE::NPC_1)
				m_pQuestBox[i]->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"GetItem_Complete").GetPointer());
		}
	}
}

void CQuest::lateupdate()
{
}

void CQuest::finalupdate()
{
}

void CQuest::SaveToScene(FILE* _pFile)
{
}

void CQuest::LoadFromScene(FILE* _pFile)
{
}