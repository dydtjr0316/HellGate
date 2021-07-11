#include "pch.h"
#include "MonsterScript.h"
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
		
		if (i == 1) 
			vScale = Vector3(360.f, 2.f, 1.f);
	
		pMonsterUi->Transform()->SetLocalScale(vScale);
		pMonsterUi->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));

		// MeshRender 설정
		Ptr<CMesh> hp = new CMesh;
		hp = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");


		pMonsterUi->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pMonsterUi->MeshRender()->SetMaterial(pMtrl->Clone());

		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster")->AddGameObject(pMonsterUi);

		if (i == 0) {
			m_pUi = pMonsterUi;
		}
		else {
			m_pUnderUi = pMonsterUi;
		}
	}

	// Child Dummy 
	CGameObject* childDummy = new CGameObject;
	childDummy->SetName(L"ChildDummy");
	childDummy->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
	childDummy->AddComponent(new CTransform);
	childDummy->Transform()->SetLocalPos(Vector3(0.0f, 0.0f, 0.0f));
	childDummy->Transform()->SetLocalScale(Vector3(1.f, 1.f, 1.f));

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Monster")->AddGameObject(childDummy);
	m_pChildDummy = childDummy;
}

CMonsterScript::~CMonsterScript()
{
	DeleteObject(m_pUi);
	DeleteObject(m_pUnderUi);
	DeleteObject(m_pChildDummy);
}


void CMonsterScript::update()
{
	//if (g_Object.count(m_sId) == 0)return;
	
	CGameObject* monster = GetObj();
	CTransform* monsterTrans = monster->Transform();
	Vector3 monsterPos = monsterTrans->GetLocalPos();
	Vector3 worldDir;
	CMonsterScript* monsterScript = monster->GetScript<CMonsterScript>();
	if (m_Packet_autoMove != nullptr)
	{
		switch ((MONSTER_AUTOMOVE_DIR)m_Packet_autoMove->eDir)
		{
		case MONSTER_AUTOMOVE_DIR::FRONT:
			//worlddir 변경
			// 밑에꺼 처럼 좌표 변경하는 코드1
			//monsterPos.z += DT * 200;

			worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::UP);
			monsterPos += worldDir * 20.f * DT;
			monsterTrans->SetLocalPos(monsterPos);

			break;
		case MONSTER_AUTOMOVE_DIR::BACK:
		/*	monsterTrans->SetLocalRot(Vector3(XM_PI, 0.f, 0.f));
			worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::UP);
			monsterPos += worldDir * 20.f * DT;
			monsterTrans->SetLocalPos(monsterPos);*/
			break;
		case MONSTER_AUTOMOVE_DIR::LEFT:
			break;
		case MONSTER_AUTOMOVE_DIR::RIGHT:
			break;
		case MONSTER_AUTOMOVE_DIR::AUTO:
			// a* 사용할곳
			break;
		case MONSTER_AUTOMOVE_DIR::IDLE:
			m_Packet_autoMove = nullptr;
			break;
		default:
			break;
		}

		if ((MONSTER_AUTOMOVE_DIR)m_Packet_autoMove->eDir != MONSTER_AUTOMOVE_DIR::IDLE)
			monsterTrans->SetLocalPos(monsterPos);
	}

	if (monsterScript->GetBisAttack())
	{
		monsterScript->AnimClipReset();
		monsterScript->Setcnt(monsterScript->Getcnt(MONSTER_ANICNT_TYPE::DEATH_CNT) + DT, MONSTER_ANICNT_TYPE::DEATH_CNT);
		SetAnimation(MONSTER_ANI_TYPE::DEAD);
	}
	if (monsterScript->Getcnt(MONSTER_ANICNT_TYPE::DEATH_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength && monsterScript->GetBisAttack())
	{
		monsterScript->SetBisAttack(false);
		monsterScript->Setcnt(0.f, MONSTER_ANICNT_TYPE::DEATH_CNT);
		m_bisAniReset = false;
		g_netMgr.Send_MonsterDead_Packet(m_sId);
		DeleteObject(GetObj());
		CEventMgr::GetInst()->update();
		g_Object.erase(m_sId);

	}

	// is damaged
	if (m_bisDamaged == true && monsterScript->GetBisAttack() == false) {
		monsterScript->AnimClipReset();
		monsterScript->Setcnt(monsterScript->Getcnt(MONSTER_ANICNT_TYPE::DAMAGE_CNT) + DT, MONSTER_ANICNT_TYPE::DAMAGE_CNT);
		SetAnimation(MONSTER_ANI_TYPE::DAMAGE);
	}
	if (monsterScript->Getcnt(MONSTER_ANICNT_TYPE::DAMAGE_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
		m_bisDamaged = false;
		m_bisAniReset = false;
		monsterScript->Setcnt(0.f, MONSTER_ANICNT_TYPE::DAMAGE_CNT);
		SetAnimation(MONSTER_ANI_TYPE::IDLE);
		// 서버에 패킷 보내야 함
	}

	// attack
	if (m_bisPunch == true && KEY_TAB(KEY_TYPE::KEY_NUM3)) {
		monsterScript->AnimClipReset();
		monsterScript->Setcnt(monsterScript->Getcnt(MONSTER_ANICNT_TYPE::ATTACK_CNT) + DT, MONSTER_ANICNT_TYPE::ATTACK_CNT);
		SetAnimation(MONSTER_ANI_TYPE::ATTACK);
	}
	if (monsterScript->Getcnt(MONSTER_ANICNT_TYPE::ATTACK_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
		m_bisPunch = false;
		m_bisAniReset = false;
		monsterScript->Setcnt(0.f, MONSTER_ANICNT_TYPE::ATTACK_CNT);
		SetAnimation(MONSTER_ANI_TYPE::IDLE);
	}

	//------
	// ui
	//------

	if (m_bSetChild == false) {
		GetObj()->AddChild(m_pChildDummy);
		m_pChildDummy->AddChild(m_pUi);
		m_bSetChild = true;
	}

	Vector3 MonsterPos = GetObj()->Transform()->GetLocalPos();

	Vector3 UiPos = m_pUi->Transform()->GetLocalPos();
	Vector3 UiUnderPos = m_pUnderUi->Transform()->GetLocalPos();
	Vector3 PlayerRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();
	Vector3 UIscale = m_pUi->Transform()->GetLocalScale();

	m_pChildDummy->Transform()->SetLocalRot(Vector3(PlayerRot.x, PlayerRot.z, -PlayerRot.y) + Vector3(-XM_PI / 2, 0, XM_PI));

	// Ui Under Bar
	UiUnderPos = Vector3(MonsterPos.x, MonsterPos.y + 300.f, MonsterPos.z);
	m_pUnderUi->Transform()->SetLocalPos(UiUnderPos);
	m_pUnderUi->Transform()->SetLocalRot(PlayerRot + Vector3(0.f, XM_PI, 0.f));

	// Ui Bar
	// 체력 줄이는
	m_pUi->Transform()->SetLocalScale(Vector3(static_cast<float>(m_sHp * 3.5f), UIscale.y, UIscale.z));
	UiPos = Vector3(0.0f, 0.f, 0.f);
	float decresedHp = 350.f - static_cast<float>(m_sHp * 3.5f);
	UiPos.x -= decresedHp / 2;
	UiPos.y = 300.f;
	m_pUi->Transform()->SetLocalPos(UiPos);
	//m_pUi->Transform()->SetLocalRot(PlayerRot + Vector3(0.f, XM_PI, 0.f));

}

void CMonsterScript::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌이 발생하고, 상대 물체가 총일이면 스스로를 삭제

	_pOther->GetObj()->GetLayerIdx();

	if (L"Attack Object" == _pOther->GetObj()->GetName())
	{
		// 여기 두번들어감 // 용석
		g_netMgr.Send_Attack_Packet(m_sId);
		cout << "sid : " << m_sId << endl;
		
		m_bisDamaged = true;
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

void CMonsterScript::AnimClipReset()
{
	if (m_bisAniReset == false) {
		GetObj()->Animator3D()->SetClipTime(0, 0.f);
		m_bisAniReset = true;
	}
}

void CMonsterScript::DecreaseHp()
{

}


