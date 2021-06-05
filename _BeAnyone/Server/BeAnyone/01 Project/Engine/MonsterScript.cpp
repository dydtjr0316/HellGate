#include "pch.h"
#include "MonsterScript.h"
#include "MonsterHpUiScript.h"
#include "RenderMgr.h"
int attackcnt = 0;
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
	DeleteObject(m_pUi);
	DeleteObject(m_pUnderUi);
}


void CMonsterScript::update()
{

	if (m_bisAttack)
	{
		m_fAnimationCnt += DT;
		cout << m_fAnimationCnt << endl;
		SetAnimation(MONSTER_ANI_TYPE::DEAD);
	}
	if (m_fAnimationCnt > 1.5f)
	{
		m_bisAttack = false;

		cout << "update " << endl;
		g_netMgr.Send_MonsterDead_Packet(m_sId);
		DeleteObject(GetObj());
		CEventMgr::GetInst()->update();
		g_Object.erase(m_sId);
	}



	//------
	// ui
	//------

	Vector3 MonsterPos = GetObj()->Transform()->GetLocalPos();

	Vector3 UiPos = m_pUi->Transform()->GetLocalPos();
	Vector3 PlayerRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();
	Vector3 UIscale = m_pUi->Transform()->GetLocalScale();

	UiPos.x = MonsterPos.x - 100.f;
	UiPos.y = MonsterPos.y + 300.f;
	UiPos.z = MonsterPos.z;
	m_pUi->Transform()->SetLocalPos(UiPos);

	m_pUi->Transform()->SetLocalRot(PlayerRot + Vector3(0.f, XM_PI, 0.f));

	// 체력 줄이는
	m_pUi->Transform()->SetLocalScale(Vector3(static_cast<float>(m_sHp*3.5f), UIscale.y, UIscale.z));
	
	


	// Transform 월드 좌표정보 얻기
	//Vector3 vPos = Transform()->GetLocalPos();

	//// 수정된 좌표를 다시 세팅하기.
	//Transform()->SetLocalPos(vPos);
}

void CMonsterScript::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌이 발생하고, 상대 물체가 총일이면 스스로를 삭제

	_pOther->GetObj()->GetLayerIdx();

	if (L"Attack Object" == _pOther->GetObj()->GetName())
	{
		// 여기 두번들어감 // 용석
		g_netMgr.Send_Attack_Packet(m_sId);

	}
}

void CMonsterScript::OnCollisionExit(CCollider* _pOther)
{
}

void CMonsterScript::SetAnimation(const MONSTER_ANI_TYPE& type)
{
	GetObj()->Animator3D()->SetBones(m_pAniData[(int)type]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(m_pAniData[(int)type]->GetAnimClip());
	GetObj()->MeshRender()->SetMesh(m_pAniData[(int)type]);
}

void CMonsterScript::DecreaseHp()
{

}


