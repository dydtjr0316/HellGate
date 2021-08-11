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
	CGameObject* pObejct = new CGameObject;
	pObejct->SetName(L"QuestBox");
	pObejct->FrustumCheck(false);
	pObejct->AddComponent(new CTransform);
	pObejct->AddComponent(new CMeshRender);
	pObejct->Transform()->SetLocalPos(Vector3((res.fWidth / 2.f) - (QuestBoxScale.x / 2.f), (res.fHeight / 2.f) - (QuestBoxScale.y / 2.f) - 30.f, 1.f));
	pObejct->Transform()->SetLocalScale(QuestBoxScale);
	//MeshRender 설정
	pObejct->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObejct->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"QuestBoxMtrl"));
	pObejct->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"0").GetPointer());
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObejct);
	m_pQuestBox = pObejct;
	for (int i = 0; i < (UINT)QUEST_TYPE::END; ++i) {
		m_vQuestCheck.push_back(0);
		m_bDoQuest[i] = true;
	}
}

CQuest::~CQuest()
{
	
}

void CQuest::update()
{
	// KIIL_MONSTER
	//GetObj()->
	if (m_vQuestCheck[(UINT)QUEST_TYPE::KILL_MONSTER] == 3) {
		m_pQuestBox->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"2").GetPointer());
		
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