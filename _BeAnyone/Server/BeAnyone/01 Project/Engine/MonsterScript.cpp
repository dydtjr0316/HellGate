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
		pMonsterUi->FrustumCheck(false);	// ����ü �ø� ������� ����
		pMonsterUi->AddComponent(new CTransform);
		pMonsterUi->AddComponent(new CMeshRender);
		tResolution res = CRenderMgr::GetInst()->GetResolution();
		
		if (i == 1) 
			vScale = Vector3(360.f, 2.f, 1.f);
	
		pMonsterUi->Transform()->SetLocalScale(vScale);
		pMonsterUi->Transform()->SetLocalRot(Vector3(0.f, 0.f, 0.f));

		// MeshRender ����
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
	childDummy->FrustumCheck(false);	// ����ü �ø� ������� ����
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
	CGameObject* monster = g_Object.find(m_sId)->second;
	CMonsterScript* monsterScript = monster->GetScript<CMonsterScript>();
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
		g_netMgr.Send_MonsterDead_Packet(m_sId);
		DeleteObject(GetObj());
		CEventMgr::GetInst()->update();
		g_Object.erase(m_sId);
	}

	// is damaged
	if (m_bisDamaged == true) {
		monsterScript->AnimClipReset();
		monsterScript->Setcnt(monsterScript->Getcnt(MONSTER_ANICNT_TYPE::DAMAGE_CNT) + DT, MONSTER_ANICNT_TYPE::DAMAGE_CNT);
		SetAnimation(MONSTER_ANI_TYPE::DAMAGE);
	}
	if (monsterScript->Getcnt(MONSTER_ANICNT_TYPE::DAMAGE_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength && monsterScript->GetBisAttack()) {
		m_bisDamaged = false;
		monsterScript->Setcnt(0.f, MONSTER_ANICNT_TYPE::DAMAGE_CNT);
		SetAnimation(MONSTER_ANI_TYPE::IDLE);
		// ������ ��Ŷ ������ ��
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
	// ü�� ���̴�
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
	// �浹�� �߻��ϰ�, ��� ��ü�� �����̸� �����θ� ����

	_pOther->GetObj()->GetLayerIdx();

	if (L"Attack Object" == _pOther->GetObj()->GetName())
	{
		// ���� �ι��� // �뼮
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

void CMonsterScript::DecreaseHp()
{

}


