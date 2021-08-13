#include "pch.h"
#include "PlayerScript.h"
#include "BulletScript.h"
#include "RenderMgr.h"
#include "StaticUI.h"
#include "Quest.h"
#include "Sound.h"
#include <iostream>


int anicnt = 0;

using namespace std;

bool checkOnce = true;
int movePacketSendCnt;
bool ReckonerMove = false;

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
	m_fSpeed = PLAYER_SPEED_IDLE;
	m_eAniType = Ani_TYPE::IDLE;

	// ui create
	Ptr<CTexture> UiTexture[2] = {
		CResMgr::GetInst()->FindRes<CTexture>(L"UiHug"),
		CResMgr::GetInst()->FindRes<CTexture>(L"UiTemper")
	};

	Vector3 vScale(350.f, 10.f, 1.f);
	CGameObject* pObject = new CGameObject;
	tResolution res = CRenderMgr::GetInst()->GetResolution();

	for (int i = 0; i < 4; ++i) {
		pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		if (i == 0 || i == 1) {
			if (i == 1) {
				vScale = Vector3(350.f, 20.f, 1.f);
			}
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) 
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (10.f * (i + 1) + (10.f * i))
				, 1.f));
		}
		else if (i == 2 || i == 3) {
			vScale = Vector3(360.f, 2.f, 1.f);
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) 
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (15.f * (i - 1) + (5.f * (i - 2)))
				, 1.f));
		}
		pObject->Transform()->SetLocalScale(vScale);
		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

		if (i == 0 || i == 1)
			m_vUiBar.push_back(pObject);
		else if (i == 2 || i == 3)
			m_vUnderBar.push_back(pObject);
	}

	vScale = Vector3(350.f, 10.f, 1.f);

	for (int i = 0; i < 4; ++i) {

		pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		if (i == 0 || i == 1) {
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 600.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (10.f * (i + 1) + (10.f * i))
				, 1.f));
		}
		else if (i == 2 || i == 3) {
			vScale = Vector3(360.f, 2.f, 1.f);
			pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 600.f
				, (res.fHeight / 2.f) - (vScale.y / 2.f) - (15.f * (i - 1) + (5.f * (i - 2)))
				, 1.f));
		}
		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->SetUiRenderCheck(false);
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

		if (i == 0 || i == 1)
			m_vUiBar.push_back(pObject);
		else if (i == 2 || i == 3)
			m_vUnderBar.push_back(pObject);
	}

	vScale = Vector3(40.f, 40.f, 1.f);

	for (int i = 0; i < 2; ++i) {

		pObject = new CGameObject;
		pObject->SetName(L"UI Object");
		pObject->FrustumCheck(false);	// 절두체 컬링 사용하지 않음
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);

		pObject->Transform()->SetLocalPos(Vector3(-(res.fWidth / 2.f) + (vScale.x / 2.f) + 10.f + (410 * i)
			, (res.fHeight / 2.f) - (vScale.y / 2.f) - 10.f
			, 1.f));
		pObject->Transform()->SetLocalScale(vScale);

		// MeshRender 설정
		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
		pObject->MeshRender()->SetMaterial(pMtrl->Clone());
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, UiTexture[i].GetPointer());
		// AddGameObject
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pObject);

		m_vUiButton.push_back(pObject);
	}

}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	int a = 1;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &a);
}

void CPlayerScript::update()
{	
	int id = GetObj()->GetID();
	if (id >= MAX_USER)return;
	CTerrain* pTerrain = GetObj()->GetScript<CPlayerScript>()->GetTerrain();
	CPlayerScript* player = GetObj()->GetScript<CPlayerScript>();
	const Vector3& xmf3Scale = GetObj()->GetScript<CPlayerScript>()->Transform()->GetLocalScale();
	Vector3 localPos = GetObj()->Transform()->GetLocalPos();
	CTransform* playerTrans = GetObj()->Transform();
	Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vector3 vRot = GetObj()->Transform()->GetLocalRot();
	Vector3 worldDir;
	KEY_TYPE tempAnimation;
	char dir = MV_IDLE;
	bool moveKeyInput = false;
	op_Move();


	// 공격 애니메이션
	if (KEY_TAB(KEY_TYPE::KEY_R))
	{
		//CSound::GetInst()->Play(Sound_Type::HIT);
		player->AnimClipReset();
		player->SetAttack(true);
		g_netMgr.Send_Player_Animation_Packet(id, player->GetAttack(), Ani_TYPE::ATTACK);
		anicnt++;
	}

	if (player->GetAttack() && player->GetCnt(PlAYER_ANICNT_TYPE::ATTACK_CNT) < GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
		player->SetCnt(player->GetCnt(PlAYER_ANICNT_TYPE::ATTACK_CNT) + DT, PlAYER_ANICNT_TYPE::ATTACK_CNT);
		player->SetAnimation(Ani_TYPE::ATTACK);
		moveKeyInput = true;
	}
	else if (player->GetCnt(PlAYER_ANICNT_TYPE::ATTACK_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength)
	{
		Attack_Default();

		player->SetAttack(false);
		player->SetCnt(0.f, PlAYER_ANICNT_TYPE::ATTACK_CNT);
		g_netMgr.Send_Player_Animation_Packet(id, player->GetAttack(), Ani_TYPE::ATTACK);

	}

	// 아이템 줍기 애니메이션 효림 0807(서버 붙일 거)
	if (KEY_TAB(KEY_TYPE::KEY_E)) {
		player->AnimClipReset();
		//player->SetAnimation(Ani_TYPE::PICK_UP);
		//CSound::GetInst()->Play(Sound_Type::GET_COIN);
		m_bIsPick = true;
		PickUp_Default();
		g_netMgr.Send_Player_Animation_Packet(id, m_bIsPick, Ani_TYPE::PICK_UP);


	}
	if (m_bIsPick == true && player->GetCnt(PlAYER_ANICNT_TYPE::PICKUP_CNT) < GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
		player->SetCnt(player->GetCnt(PlAYER_ANICNT_TYPE::PICKUP_CNT) + DT, PlAYER_ANICNT_TYPE::PICKUP_CNT);
		player->SetAnimation(Ani_TYPE::PICK_UP);
	}
	else if (player->GetCnt(PlAYER_ANICNT_TYPE::PICKUP_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength)
	{

		m_bIsPick = false;
		player->SetCnt(0.f, PlAYER_ANICNT_TYPE::PICKUP_CNT);
		g_netMgr.Send_Player_Animation_Packet(id, m_bIsPick, Ani_TYPE::PICK_UP);


	}

	// 데미지 애니메이션
	if (KEY_TAB(KEY_TYPE::KEY_Q)) {
		player->AnimClipReset();
		player->SetDamage(true);
		g_netMgr.Send_Player_Animation_Packet(id, player->GetDamage(), Ani_TYPE::DAMAGE);
	}
	if (player->GetDamage() && player->GetCnt(PlAYER_ANICNT_TYPE::DAMAGE_CNT) < GetObj()->Animator3D()->GetAnimClip(0).dTimeLength) {
		player->SetCnt(player->GetCnt(PlAYER_ANICNT_TYPE::DAMAGE_CNT) + DT, PlAYER_ANICNT_TYPE::DAMAGE_CNT);
		player->SetAnimation(Ani_TYPE::DAMAGE);
		moveKeyInput = true;
	}
	else if (player->GetCnt(PlAYER_ANICNT_TYPE::DAMAGE_CNT) > GetObj()->Animator3D()->GetAnimClip(0).dTimeLength)
	{
		//Attack_Default();

		player->SetDamage(false);
		player->SetCnt(0.f, PlAYER_ANICNT_TYPE::DAMAGE_CNT);
		g_netMgr.Send_Player_Animation_Packet(id, player->GetDamage(), Ani_TYPE::DAMAGE);

	}

	// stamina ui
	ClickUiButton();

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		vRot.y += vDrag.x * DT * ROTATE_SPEED;
		GetObj()->Transform()->SetLocalRot(vRot);
	}

	if (KEY_TAB(KEY_TYPE::KEY_SPACE) || KEY_AWAY(KEY_TYPE::KEY_SPACE))
	{
		player->SetChangeSpeed();
		//cout << localPos.x << "\t" << localPos.z << endl;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		worldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += worldDir * player->GetSpeed() * DT;
		dir = MV_FRONT;
		player->SetPlayerDir(worldDir * player->GetSpeed() * DT);
		player->SetAnimation(Ani_TYPE::WALK_F);
		moveKeyInput = true;
		//cout  << GetObj()->Transform()->GetLocalPos().x << " || " << GetObj()->Transform()->GetLocalPos().z << endl;
		//cout << worldDir.z << endl;
		//cout << DT << endl;

	}

	else if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		worldDir = playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += worldDir * player->GetSpeed() * DT;

		dir = MV_BACK;
		player->SetAnimation(Ani_TYPE::WALK_D);
		moveKeyInput = true;
	}
	else if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		worldDir = playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += worldDir * player->GetSpeed() * DT;

		dir = MV_LEFT;
		player->SetAnimation(Ani_TYPE::WALK_F);
		moveKeyInput = true;
	}

	else if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		worldDir = -playerTrans->GetWorldDir(DIR_TYPE::RIGHT);
		localPos += worldDir * player->GetSpeed() * DT;

		dir = MV_RIGHT;
		player->SetAnimation(Ani_TYPE::WALK_F);
		moveKeyInput = true;
	}
	if (moveKeyInput)
	{
		int z = (int)(localPos.z / xmf3Scale.z);

		float fHeight = pTerrain->GetHeight(localPos.x, localPos.z, ((z % 2) != 0)) * 2.f;


		if (localPos.y != fHeight)
			localPos.y = fHeight;
		playerTrans->SetLocalPos(localPos);
	}
	else
	{
		player->SetAnimation(Ani_TYPE::IDLE);
	}
	if (KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_D))
		player->GetReckoner()->DeadReckoning(GetObj());

	if (((player->GetReckoner()->isFollowing() || !ReckonerMove) &&
		((KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_D)))))
	{
		ReckonerMove = true;
		system_clock::time_point start = system_clock::now();
		g_netMgr.Send_Move_Packet(dir, localPos, worldDir, vRot.y, start, DT, ReckonerMove);

		player->GetReckoner()->SetDirVec(worldDir);
		player->GetReckoner()->SetRotateY(vRot.y);
		player->GetReckoner()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
		CountTime();
	}

	if ((KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_S) || KEY_AWAY(KEY_TYPE::KEY_D)))
	{
		ReckonerMove = false;
		g_netMgr.Send_Stop_Packet(false);
		SetTime_Zero();
	}


	if (m_ftimeCount >= m_fDelayTime)
	{
		system_clock::time_point start = system_clock::now();
		g_netMgr.Send_Move_Packet(dir, localPos, worldDir, vRot.y, start, DT, ReckonerMove);


		player->GetReckoner()->SetDirVec(worldDir);
		player->GetReckoner()->SetRotateY(vRot.y);
		player->GetReckoner()->SetLocalPos(GetObj()->Transform()->GetLocalPos());
		SetTime_Zero();
	}

	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		MeshRender()->SetMaterial(m_pOriginMtrl);
	}

	// quest::find item
	if (GetObj()->Quest()->GetDoQuest(QUEST_TYPE::GET_ITEM) == false)
		FindQuestItem();
}

void CPlayerScript::op_Move()
{
	sc_packet_move* p = GetObj()->GetScript<CPlayerScript>()->GetOtherMovePacket();

	if (p == nullptr)return;
	if (g_Object.count(p->id) == 0)return;
	if (GetObj()->GetID() == p->id)return;
	if (!p->isMoving)return;
	if (p->id >= START_MONSTER)return;

	CPlayerScript* player = g_Object.find(p->id)->second->GetScript<CPlayerScript>();
	CTransform* playerTrans = g_Object.find(p->id)->second->Transform();
	CTerrain* pTerrain = g_Object.find(p->id)->second->GetScript<CPlayerScript>()->GetTerrain();
	const Vector3& xmf3Scale = g_Object.find(p->id)->second->GetScript<CPlayerScript>()->Transform()->GetLocalScale();
	Vector3 temp;

	
	if (player->GetBisFrist())
	{
		temp = p->localVec + p->dirVec * p->speed * DT;
		player->SetBisFrist(false);
	}
	else
		temp = playerTrans->GetLocalPos() + p->dirVec * p->speed * DT;


	playerTrans->SetLocalRot(p->rotateY);

	int z = (int)(temp.z / xmf3Scale.z);
	float fHeight = pTerrain->GetHeight(temp.x, temp.z, ((z % 2) != 0)) * 2.f /*+ 100.f*/;

	if (temp.y != fHeight)
		temp.y = fHeight;

	playerTrans->SetLocalPos(temp);

	{
		//CPlayerScript* pScript = g_Object.find(g_myid)->second->GetScript<CPlayerScript>();
		//CPlayerScript* player = g_Object.find(p->id)->second->GetScript<CPlayerScript>();


		//pScript->Search_Origin_Points(p->id, pScript->GetRTT());


		//pScript->Compute_Bezier(player->GetOriginPoint(), player->GetInterpolationPoint());

		//CTransform* ObjTrans = g_Object.find(p->id)->second->Transform();;
		//ObjTrans->SetLocalRot(Vector3(0.f, p->rotateY, 0.f));



		//if (player->GetInterpolationCnt() != 4)
		//{
		//	ObjTrans->SetLocalPos(Vector3(player->GetInterpolationPoint()[player->GetInterpolationCnt()].x,
		//		p->localVec.y,
		//		player->GetInterpolationPoint()[player->GetInterpolationCnt()].y));
		//	cout << ObjTrans->GetLocalPos().x << endl;
		//	cout << ObjTrans->GetLocalPos().z << endl<<endl;
		//	player->InterpolationCnt_PP();
		//}
		//else
		//{
		//	g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->DeleteOherMovePaacket();
		//}
	}
}

void CPlayerScript::SetOtherMovePacket(sc_packet_move* p, const float& rtt)
{
	m_movePacketTemp = new sc_packet_move;
	m_movePacketTemp = p;
	m_fRTT = rtt;
}

void CPlayerScript::SetOrigin_Point(const int& index, const float& x, const float& y)
{
	m_v2Origin_Point[index].x = x;
	m_v2Origin_Point[index].y = y;
}

void CPlayerScript::SetInterpolation_Point(const int& index, const float& x, const float& y)
{
	m_v2Interpolation_Point[index].x = x;
	m_v2Interpolation_Point[index].y = y;
}

void CPlayerScript::Search_Origin_Points(const int& id, const float& rtt)
{
	sc_packet_move* recvPacket = GetObj()->GetScript<CPlayerScript>()->GetOtherMovePacket();
	Vector3 tempLocalPos = recvPacket->localVec;
	
	float tempSpeed = recvPacket->speed;
	
	Vector3 tempWorldDir;
	Vector3 tempARR_WorldDir[3];

	Matrix matRot = DirectX::XMMatrixRotationX(0.f);
	matRot *= DirectX::XMMatrixRotationY(m_movePacketTemp->rotateY);
	matRot *= DirectX::XMMatrixRotationZ(0.f);

	static Vector3 arrDefault[(UINT)DIR_TYPE::END] = { Vector3::Right, Vector3::Up, Vector3::Front };

	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		tempARR_WorldDir[i] = XMVector3TransformNormal(arrDefault[i], matRot);
	}
	switch (m_movePacketTemp->dir)
	{
	case MV_FRONT:
		tempWorldDir = -tempARR_WorldDir[(UINT)DIR_TYPE::FRONT];
		break;
	case MV_LEFT:
		tempWorldDir = tempARR_WorldDir[(UINT)DIR_TYPE::FRONT];
		break;
	case MV_RIGHT:
		tempWorldDir = tempARR_WorldDir[(UINT)DIR_TYPE::RIGHT];
		break;
	case MV_BACK:
		tempWorldDir = -tempARR_WorldDir[(UINT)DIR_TYPE::RIGHT];
		break;
	case MV_IDLE:
		break;
	default:
		cout << "Unknown Direction from Client move packet!\n";
		DebugBreak();
		exit(-1);
	}
	

	for (int i = 0; i < 4; i++) {
		tempLocalPos.x += (tempWorldDir.x * tempSpeed * (float)(DT));
		tempLocalPos.z += (tempWorldDir.z * tempSpeed * (float)(DT));
		g_Object.find(id)->second->GetScript<CPlayerScript>()->SetOrigin_Point(i, tempLocalPos.x, tempLocalPos.z);
		
	}
}

Vector2 CPlayerScript::Search_Interpolation_Points(Vector2* points, float time)
{
	float ax, bx, cx, ay, by, cy, time_Double, time_Triple;
	Vector2 result;

	cx = 3.0f * (points[1].x - points[0].x);
	bx = 3.0f * (points[2].x - points[1].x) - cx;
	ax = (points[3].x - points[0].x) - cx - bx;

	cy = 3.0f * (points[1].y - points[0].y);
	by = 3.0f * (points[2].y - points[1].y) - cy;
	ay = (points[3].y - points[0].y) - cy - by;

	time_Double = time * time;
	time_Triple = time_Double * time;

	result.x = (ax * time_Triple) + (bx * time_Double) + (cx * time) + points[0].x;
	result.y = (ay * time_Triple) + (bx * time_Double) + (cy * time) + points[0].y;

	return result;
}

void CPlayerScript::Compute_Bezier(Vector2* points, Vector2* dest)
{
	float dt;
	int i;
	dt = DT / 3.f;
	sc_packet_move* recvPacket = GetObj()->GetScript<CPlayerScript>()->GetOtherMovePacket();
	CPlayerScript* player = g_Object.find(recvPacket->id)->second->GetScript<CPlayerScript>();
	for (i = 0; i < 4; i++)
	{

		Vector2 inst = g_Object.find(recvPacket->id)->second->
			GetScript<CPlayerScript>()->Search_Interpolation_Points(points, i * dt);

		player->SetInterpolation_Point(i, inst.x, inst.y);

	}
}

void CPlayerScript::SetAnimation(const Ani_TYPE& type)
{
	GetObj()->Animator3D()->SetBones(m_pAniData[(int)type]->GetBones());
	GetObj()->Animator3D()->SetAnimClip(m_pAniData[(int)type]->GetAnimClip());
	GetObj()->MeshRender()->SetMesh(m_pAniData[(int)type]);

	//GetObj()->GetScript<CPlayerScript>()->SetAnimationType(type);
}

void CPlayerScript::SetAnimation(const uShort& other_id, const Ani_TYPE& type)
{
	g_Object.find(other_id)->second->Animator3D()->SetBones(m_pAniData[(int)type]->GetBones());
	g_Object.find(other_id)->second->Animator3D()->SetAnimClip(m_pAniData[(int)type]->GetAnimClip());
	g_Object.find(other_id)->second->MeshRender()->SetMesh(m_pAniData[(int)type]);
}

bool CPlayerScript::isInMap(const Vector3& localPos)
{
	if ((localPos.x > 200 && localPos.x < 6200) && (localPos.z > 200 && localPos.z < 6200))return true;
	else return false;
}

void CPlayerScript::OnCollisionEnter(CCollider* _pOther)
{
}

void CPlayerScript::OnCollision(CCollider* _pOther)
{
	BoundingSphere myBS = Collider()->GetBoundingSphere();
	BoundingSphere otherBS = _pOther->Collider()->GetBoundingSphere();

	
	Vector3 localPos = GetObj()->Transform()->GetLocalPos();
	localPos += -GetObj()->GetScript<CPlayerScript>()->GetPlayerDir() * 2;
	GetObj()->Transform()->SetLocalPos(localPos);

}

void CPlayerScript::OnCollisionExit(CCollider* _pOther)
{
}

void CPlayerScript::Attack_Default()
{
	CPlayerScript* player = GetObj()->GetScript<CPlayerScript>();
	Vector3 vPos = player->Transform()->GetLocalPos();

	vector<CGameObject*> vecObj;
	CSceneMgr::GetInst()->FindGameObjectByTag(L"Attack Object", vecObj);

	if (!vecObj.empty())
	{
		cout << "총알 객체 생성 안됌" << endl;
		return;
	}
	else
		cout << "생성" << endl << endl;

	CGameObject* pBullet = new CGameObject;
	pBullet->SetName(L"Attack Object");

	vPos += -player->Transform()->GetWorldDir(DIR_TYPE::FRONT) * player->Collider()->GetBoundingSphere().Radius;
	pBullet->AddComponent(new CTransform());
	pBullet->Transform()->SetLocalPos(vPos);
	pBullet->AddComponent(new CCollider);
	pBullet->Collider()->SetColliderType(COLLIDER_TYPE::BOX);
	pBullet->Collider()->SetBoundingSphere(BoundingSphere(vPos, 100.f));
	pBullet->AddComponent(new CBulletScript);
	CBulletScript* bulletScript = pBullet->GetScript<CBulletScript>();
	bulletScript->SetPlayer(GetObj());

	CreateObject(pBullet, L"Bullet");
}

void CPlayerScript::PickUp_Default()
{
	Vector3 vPos = GetObj()->Transform()->GetLocalPos();

	/*vector<CGameObject*> vecObj;
	CSceneMgr::GetInst()->FindGameObjectByTag(L"PickUP Object", vecObj);

	if (!vecObj.empty())
	{
		cout << "줍기 객체 생성 실패" << endl;
		return;
	}
	else
		cout << "줍기 객체 생성" << endl << endl;*/

	CGameObject* pBullet = new CGameObject;
	pBullet->SetName(L"PickUP Object");
	
	pBullet->AddComponent(new CTransform());
	pBullet->AddComponent(new CCollider);
	pBullet->Transform()->SetLocalPos(vPos);
	pBullet->Collider()->SetColliderType(COLLIDER_TYPE::BOX);
	//	기존 Sphere 값 받아와서 GetHeight에서 200.f 만큼 더한값을 더해줘서 콜리전 생성.
	BoundingSphere sphere = Collider()->GetBoundingSphere();
	sphere.Center.y += 200.f;
	pBullet->Collider()->SetBoundingSphere(BoundingSphere( sphere.Center, sphere.Radius * 5.f));
	pBullet->AddComponent(new CBulletScript);
	pBullet->GetScript<CBulletScript>()->SetBulletType(BULLET_TYPE::PICKUP);
	pBullet->GetScript<CBulletScript>()->SetUIObj(m_pItemUIObj);


	CreateObject(pBullet, L"Player");
}

void CPlayerScript::ClickUiButton()
{
	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) {
		POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();

		for (int i = 0; i < m_vUiButton.size(); ++i) {
			Vector3 vUiButtonPos = m_vUiButton[i]->Transform()->GetLocalPos();
			Vector3 vUiButtonScale = m_vUiButton[i]->Transform()->GetLocalScale();
			tResolution res = CRenderMgr::GetInst()->GetResolution();

			vUiButtonPos.x += res.fWidth / 2;
			vUiButtonPos.y += -(res.fHeight / 2);

			if (pMousePos.x >= vUiButtonPos.x - (vUiButtonScale.x / 2) && pMousePos.x <= vUiButtonPos.x + (vUiButtonScale.x / 2)
				&& pMousePos.y <= -vUiButtonPos.y + (vUiButtonScale.x / 2) && pMousePos.y >= -vUiButtonPos.y - (vUiButtonScale.x / 2)) {

				if (m_bUiCheck == true) {
					// m_pObject->Transform()->SetLocalScale(Vector3(90.f, 90.f, 1.f));
					m_vUiBar[(UINT)UI_BAR::HUG + i]->SetUiRenderCheck(true);
					m_vUnderBar[(UINT)UI_BAR::HUG + i]->SetUiRenderCheck(true);
					m_bUiCheck = false;
				}
				else if (m_bUiCheck == false) {
					m_vUiBar[(UINT)UI_BAR::HUG + i]->SetUiRenderCheck(false);
					m_vUnderBar[(UINT)UI_BAR::HUG + i]->SetUiRenderCheck(false);
					m_bUiCheck = true;
				}
			}
		}
	}
}

void CPlayerScript::FindQuestItem()
{
	GetObj()->Quest()->SetQuestcount(QUEST_TYPE::GET_ITEM, m_pItemUIObj->StaticUI()->GetQuestItemCount());
}