#include "pch.h"
#include "MonsterScript.h"
#include "RenderMgr.h"
#include "ItemMgr.h"
#include "Terrain.h"
#include "Quest.h"
#include "BulletScript.h"


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
void CMonsterScript::SetPacketMove(sc_packet_monster_automove* p)
{
	if (p == nullptr)return;
	if (this == nullptr)return;
	if (GetID() != p->id)return;
	m_Packet_autoMove = p;
}

void CMonsterScript::update()
{
	Move();
	Attack();
	//------
	// ui
	//------
	if (m_bSetChild == false) {
		//GetObj()->AddChild(m_pChildDummy);
		m_pChildDummy->AddChild(m_pUi);
		m_bSetChild = true;
	}

	Vector3 MonsterPos = GetObj()->Transform()->GetLocalPos();
	Vector3 DummyPos = m_pChildDummy->Transform()->GetLocalPos();
	Vector3 UiPos = m_pUi->Transform()->GetLocalPos();
	Vector3 UiUnderPos = m_pUnderUi->Transform()->GetLocalPos();
	Vector3 PlayerRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();
	Vector3 UIscale = m_pUi->Transform()->GetLocalScale();

	DummyPos = MonsterPos;
	m_pChildDummy->Transform()->SetLocalRot(Vector3(PlayerRot + Vector3(0.f, XM_PI, 0.f)));
	m_pChildDummy->Transform()->SetLocalPos(DummyPos);

	// Ui Under Bar
	UiUnderPos = Vector3(MonsterPos.x, MonsterPos.y + 300.f, MonsterPos.z);
	m_pUnderUi->Transform()->SetLocalPos(UiUnderPos);
	m_pUnderUi->Transform()->SetLocalRot(PlayerRot + Vector3(0.f, XM_PI, 0.f));

	// Ui Bar
	// 체력 줄이는
	m_pUi->Transform()->SetLocalScale(Vector3(static_cast<float>(m_sHp * 3.5f), UIscale.y, UIscale.z));
	UiPos = Vector3(0.0f, 300.f, 0.f);
	float decresedHp = 350.f - static_cast<float>(m_sHp * 3.5f);
	UiPos.x -= decresedHp / 2;
	UiPos.y = 300.f;
	m_pUi->Transform()->SetLocalPos(UiPos);


}

void CMonsterScript::OnCollisionEnter(CCollider* _pOther)
{
	// 충돌이 발생하고, 상대 물체가 총일이면 스스로를 삭제

	_pOther->GetObj()->GetLayerIdx();

	if (L"Attack Object" == _pOther->GetObj()->GetName())
	{
		// 여기 두번들어감 // 용석
		g_netMgr.Send_Attack_Packet(m_sId);
		m_bisMoving = false;

		// 죽으면 하는 걸로 바꿔야 함 일단 오류나니까 여기서 처리하기
		CItemMgr::GetInst()->SetIsMake(true);
		g_netMgr.Send_ItemCreate_Paket(GetObj()->Transform()->GetLocalPos());

		m_pPlayer = _pOther->GetObj()->GetScript<CBulletScript>()->GetPlayer();
		if(m_pPlayer->Quest()->GetDoQuest(QUEST_TYPE::KILL_MONSTER) == false)
			m_pPlayer->Quest()->AddQuestcount(QUEST_TYPE::KILL_MONSTER);
		
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

void CMonsterScript::SetAnimation(const uShort& other_id, const MONSTER_ANI_TYPE& type)
{
	g_Object.find(other_id)->second->Animator3D()->SetBones(m_pAniData[(int)type]->GetBones());
	g_Object.find(other_id)->second->Animator3D()->SetAnimClip(m_pAniData[(int)type]->GetAnimClip());
	g_Object.find(other_id)->second->MeshRender()->SetMesh(m_pAniData[(int)type]);
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

void CMonsterScript::Move()
{
	int id = GetID();
	if (g_Object.count(GetID()) == 0)return;
	if(g_Object.find(GetID())->second==nullptr)return;
	

	CGameObject* monster = g_Object.find(GetID())->second;
	CTransform* monsterTrans = monster->Transform();
	Vector3 monsterPos = monsterTrans->GetLocalPos();
	Vector3 worldDir;
	CMonsterScript* monsterScript = monster->GetScript<CMonsterScript>();
	CTerrain* pTerrain = monsterScript->GetTerrain();
	const Vector3& xmf3Scale = monster->GetScript<CMonsterScript>()->Transform()->GetLocalScale();
	string temp;
	MONSTER_AUTOMOVE_DIR monsterDir;
	GetObj()->Transform()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
	Vector3 tempWorldPos(0.f, 0.f, 0.f);
	if (monsterScript->GetPacketMove() != nullptr&& m_bisMoving)
	{
		if (GetID() == 1000)
		{
			cout << "dir : " << (int)m_eDir << endl;
			cout << monsterTrans->GetLocalPos().x << " , " << monsterTrans->GetLocalPos().z << endl;
		}
		monsterDir = (MONSTER_AUTOMOVE_DIR)monsterScript->GetDir();
		if ((int)monsterDir >= 0 && (int)monsterDir <= 6)
		{
			switch (monsterDir)
			{
			case MONSTER_AUTOMOVE_DIR::FRONT:
				if (monster->GetName() == L"GreenMonster") {
					monsterTrans->SetLocalRot(Vector3(0.f, XM_PI, 0.f));
					//tempWorldPos.z = 1.f;
					worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::FRONT);
				}
				else {
					monsterTrans->SetLocalRot(Vector3(XM_PI / 2, XM_PI + m_fAngleY, 0.f));
					worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::UP);
					//tempWorldPos.z = 1.f;
					monsterPos += worldDir * 20.f * DT;
					
				}
				break;
			case MONSTER_AUTOMOVE_DIR::BACK:
				//worlddir 변경
				// 밑에꺼 처럼 좌표 변경하는 코드
				if (monster->GetName() == L"GreenMonster") {
					monsterTrans->SetLocalRot(Vector3(0.f, 0.f, 0.f));
					worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::FRONT);
				}
				else {
					monsterTrans->SetLocalRot(Vector3(XM_PI / 2, 0.f + m_fAngleY, 0.f));
					//tempWorldPos.z = -1.f;
					worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::UP);
					
				}
				break;
			case MONSTER_AUTOMOVE_DIR::LEFT:
				if (monster->GetName() == L"GreenMonster") {
					monsterTrans->SetLocalRot(Vector3(0.f, XM_PI / 2, 0.f));
					worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::FRONT);
				}
				else {
					monsterTrans->SetLocalRot(Vector3(XM_PI / 2, XM_PI / 2 + m_fAngleY, 0.f));
					//tempWorldPos.x = -1.f;
					worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::UP);
					
				}
				break;
			case MONSTER_AUTOMOVE_DIR::RIGHT:
				if (monster->GetName() == L"GreenMonster") {
					monsterTrans->SetLocalRot(Vector3(0.f, -XM_PI / 2, 0.f));
					worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::FRONT);
				}
				else {
					monsterTrans->SetLocalRot(Vector3(XM_PI / 2, -XM_PI / 2 + m_fAngleY, 0.f));
					//tempWorldPos.x = 1.f;
					worldDir = -monsterTrans->GetWorldDir(DIR_TYPE::UP);
					
				}

				break;

			case MONSTER_AUTOMOVE_DIR::AUTO:
				// a* 사용할곳
				break;
			case MONSTER_AUTOMOVE_DIR::IDLE:
				//monsterScript->SetPacketMove(nullptr);
				break;
			default:
				break;
			}

			if (m_bisDirChange)
			{
				g_netMgr.Send_MonsterDir_Packet(m_sId, worldDir);
				m_bisDirChange = false;
			}


			 monsterPos += worldDir * 100.f * DT;
			int z = (int)(monsterPos.z / xmf3Scale.z);

			float fHeight = pTerrain->GetHeight(monsterPos.x, monsterPos.z, ((z % 2) != 0)) * 2.f;

			if (monsterPos.y != fHeight)
				monsterPos.y = fHeight;
			monsterTrans->SetLocalPos(monsterPos);

		}
	}
}

void CMonsterScript::Attack()
{
	CGameObject* monster = GetObj();
	uShort monsterid = GetObj()->GetID();
	CTransform* monsterTrans = monster->Transform();
	Vector3 monsterPos = monsterTrans->GetLocalPos();
	Vector3 worldDir;
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
		monsterScript->SetAniReset(false); // m_bisAniReset = false;
		g_netMgr.Send_MonsterDead_Packet(monsterid);
		//m_Packet_autoMove->eDir = (char)MONSTER_AUTOMOVE_DIR::AUTO;

		// 여기가 죽는 부분
		DeleteObject(GetObj());
		CEventMgr::GetInst()->update();
		g_Object.erase(monsterid);

	}

	// is damaged
	if (monsterScript->GetIsDamage() && monsterScript->GetBisAttack() == false) {// (m_bisDamaged == true && monsterScript->GetBisAttack() == false) {
		monsterScript->AnimClipReset();
		monsterScript->Setcnt(monsterScript->Getcnt(MONSTER_ANICNT_TYPE::DAMAGE_CNT) + DT, MONSTER_ANICNT_TYPE::DAMAGE_CNT);
		SetAnimation(MONSTER_ANI_TYPE::DAMAGE);
		
		m_bisMoving = false;

		g_netMgr.Send_Monster_Animation_Packet(m_sId, MONSTER_ANI_TYPE::DAMAGE);
	}
	if (monsterScript->Getcnt(MONSTER_ANICNT_TYPE::DAMAGE_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
		monsterScript->SetIsDamage(false); //m_bisDamaged = false;
		monsterScript->SetAniReset(false); //m_bisAniReset = false;
		monsterScript->Setcnt(0.f, MONSTER_ANICNT_TYPE::DAMAGE_CNT);
		SetAnimation(MONSTER_ANI_TYPE::IDLE);
		m_bisMoving = true;
		g_netMgr.Send_Monster_Animation_Packet(monsterid, MONSTER_ANI_TYPE::IDLE);
		// 서버에 패킷 보내야 함

		// 플레이어에게 공격
		if (GetObj()->GetName() == L"GreenMonster")
			AttackToPlayer(MOB_TYPE::GREEN);
		else
			AttackToPlayer(MOB_TYPE::YELLOW);
	}
	GetObj()->GetID();
	// attack
	if (monsterScript->GetIsPunch()) {// (m_bisPunch) {
		// 몬스터 공격하는곳

		monsterScript->AnimClipReset();
		monsterScript->Setcnt(monsterScript->Getcnt(MONSTER_ANICNT_TYPE::ATTACK_CNT) + DT, MONSTER_ANICNT_TYPE::ATTACK_CNT);
		SetAnimation(MONSTER_ANI_TYPE::ATTACK);
		m_bisMoving = false;
		g_netMgr.Send_Monster_Animation_Packet(monsterid, MONSTER_ANI_TYPE::ATTACK);

		// packet
	}
	if (monsterScript->Getcnt(MONSTER_ANICNT_TYPE::ATTACK_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
		monsterScript->SetIsPunch(false); // m_bisPunch = false;
		monsterScript->SetAniReset(false); // m_bisAniReset = false;
		monsterScript->Setcnt(0.f, MONSTER_ANICNT_TYPE::ATTACK_CNT);
		SetAnimation(MONSTER_ANI_TYPE::IDLE);
		m_bisMoving = true;
		g_netMgr.Send_Monster_Animation_Packet(monsterid, MONSTER_ANI_TYPE::IDLE);
		// packet
	}
}

void CMonsterScript::AttackToPlayer(MOB_TYPE _eType)
{
	Vector3 playerDir = m_pPlayer->Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vector3 monsterDir{};
	Vector3 monsterRot = GetObj()->Transform()->GetLocalRot();


	if (_eType == MOB_TYPE::YELLOW)
		monsterDir = GetObj()->Transform()->GetWorldDir(DIR_TYPE::UP);

	Vector3 a = m_pPlayer->Transform()->GetLocalPos() - GetObj()->Transform()->GetLocalPos();
	Vector3 b = XMVector3Cross(a, -monsterDir);
	Vector3 c = XMVector3Dot(b, Vector3(0.0f, 1.f, 0.0f));

	Vector3 angle = XMVector3AngleBetweenVectors(playerDir, monsterDir);

	if (c.x >= 0)
		angle.x = angle.x + XM_PI;
	else
		angle.x = XM_PI - angle.x;// +XM_PI;

	if (_eType == MOB_TYPE::YELLOW)
	{
		GetObj()->Transform()->SetLocalRot(Vector3(monsterRot.x, monsterRot.y + angle.x, monsterRot.z));

	}
	m_bisDirChange = true;
	m_fAngleY = angle.x;
	SetIsPunch(true);
}




