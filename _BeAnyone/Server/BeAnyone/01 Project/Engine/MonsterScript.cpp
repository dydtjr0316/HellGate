#include "pch.h"
#include "MonsterScript.h"
#include "MonsterHpUiScript.h"
#include "RenderMgr.h"

CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
{
	//----------------
	// monster hp ui
	//----------------
	Vector3 vScale(350.f, 10.f, 1.f);

	for (int i = 0; i < 2; ++i) {
		CGameObject* pMonsterUi = new CGameObject;
		pMonsterUi->SetName(L"UI Object");
		pMonsterUi->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pMonsterUi->AddComponent(new CTransform);
		pMonsterUi->AddComponent(new CMeshRender);
		tResolution res = CRenderMgr::GetInst()->GetResolution();
		if (i == 0) {
			if (i == 1) {
				vScale = Vector3(350.f, 20.f, 1.f);
			}
			pMonsterUi->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 60.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (10.f * (i + 1) + (10.f * i))
				, 1.f));
		}
		else if (i == 1) {
			vScale = Vector3(360.f, 2.f, 1.f);
			pMonsterUi->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 60.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (15.f * (i - 1) + (5.f * (i - 2)))
				, 1.f));
		}
		pMonsterUi->Transform()->SetLocalScale(vScale);
		pMonsterUi->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));

		// MeshRender 설정
		Ptr<CMesh> hp = new CMesh;
		hp = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");


		pMonsterUi->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pMonsterUi->MeshRender()->SetMaterial(pMtrl->Clone());

		if (i == 0) {
			pMonsterUi->AddComponent(new CMonsterHpUiScript);
			CMonsterHpUiScript* uiScript = pMonsterUi->GetScript<CMonsterHpUiScript>();
			uiScript->SetObject(pMonsterUi);
			uiScript->SetMonsterObject(GetObj());
			uiScript->SetPlayerObject(g_Object.find(g_myid)->second);
		}

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster")->AddGameObject(pMonsterUi);

		if (i == 0) {
			m_pUi = pMonsterUi;
		}
		else {
			m_pUnderUi = pMonsterUi;
		}
	}
}

CMonsterScript::~CMonsterScript()
{
	SAFE_DELETE(m_pUi);
	SAFE_DELETE(m_pUnderUi);
}


void CMonsterScript::update()
{
	//------
	// ui
	//------

	Vector3 MonsterPos = GetObj()->Transform()->GetLocalPos();

	Vector3 UiPos = m_pUi->Transform()->GetLocalPos();
	Vector3 PlayerRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();

	UiPos = MonsterPos;
	UiPos.x = MonsterPos.x - 100.f;
	UiPos.y = MonsterPos.y + 300.f;
	UiPos.z = MonsterPos.z;

	m_pUi->Transform()->SetLocalPos(UiPos);
	m_pUi->Transform()->SetLocalRot(PlayerRot + Vector3(0.f, XM_PI, 0.f));

	// 체력 줄이는
	DecreaseHp();


	// Transform 월드 좌표정보 얻기
	Vector3 vPos = Transform()->GetLocalPos();

	// 수정된 좌표를 다시 세팅하기.
	Transform()->SetLocalPos(vPos);
}

void CMonsterScript::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌이 발생하고, 상대 물체가 총일이면 스스로를 삭제
	if (L"Attack Object" == _pOther->GetObj()->GetName())
	{
		DeleteObject(GetObj());	// -->삭제 이벤트 등록
		CEventMgr::GetInst()->update();
		g_Object.erase(m_sId);
	}
}

void CMonsterScript::OnCollisionExit(CCollider* _pOther)
{
}

void CMonsterScript::DecreaseHp()
{
	if (KEY_TAB(KEY_TYPE::KEY_E)) {
		Vector3 uiScale = m_pUi->Transform()->GetLocalScale();

		uiScale.x -= static_cast<float>(m_sHp);
		m_pUi->Transform()->SetLocalScale(uiScale);
	}
	
}


