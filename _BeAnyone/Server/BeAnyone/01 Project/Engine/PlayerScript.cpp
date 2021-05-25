#include "pch.h"
#include "PlayerScript.h"
#include <iostream>

using namespace std;

bool checkOnce = true;
int movePacketSendCnt;

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
{
	m_fSpeed = PLAYER_SPEED_IDLE;
	m_eAniType = Ani_TYPE::IDLE;
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
	CTerrain* pTerrain = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain();
	CPlayerScript* player = g_Object.find(g_myid)->second->GetScript<CPlayerScript>();
	const Vector3& xmf3Scale = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->Transform()->GetLocalScale();
	Vector3 localPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();
	CTransform* playerTrans = g_Object.find(g_myid)->second->Transform();
	Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
	Vector3 vRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();
	Vector3 worldDir;

	char dir = MV_IDLE;
	bool moveKeyInput = false;

	op_Move();

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		vRot.y += vDrag.x * DT * ROTATE_SPEED;

		g_Object.find(g_myid)->second->Transform()->SetLocalRot(vRot);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_SPACE) || KEY_AWAY(KEY_TYPE::KEY_SPACE))
	{
		player->SetChangeSpeed();
	}

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		worldDir = -playerTrans->GetWorldDir(DIR_TYPE::FRONT);
		localPos += worldDir * player->GetSpeed() * DT;

		dir = MV_FRONT;
		player->SetAnimation(Ani_TYPE::WALK_F);
		moveKeyInput = true;

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
		float fHeight = pTerrain->GetHeight(localPos.x, localPos.z, ((z % 2) != 0)) * 2.f + 100.f;

		if (localPos.y != fHeight)
			localPos.y = fHeight;

		playerTrans->SetLocalPos(localPos);
		//g_netMgr.Send_Move_Packet(dir, localPos, vRot.y);
	}
	else
	{
		player->SetAnimation(Ani_TYPE::IDLE);
		//g_netMgr.Send_Move_Packet(dir, localPos, vRot.y);
	}
	if (KEY_HOLD(KEY_TYPE::KEY_W)|| KEY_HOLD(KEY_TYPE::KEY_A)|| KEY_HOLD(KEY_TYPE::KEY_S)|| KEY_HOLD(KEY_TYPE::KEY_D))
		player->GetReckoner()->DeadReckoning(g_Object.find(g_myid)->second);



	if (player->GetReckoner()->isFollowing() || frameCnt % (((int)CTimeMgr::GetInst()->GetFPS()>>3)+1) == 0)
	{
		movePacketSendCnt++;
		g_netMgr.Send_Move_Packet(dir, localPos, worldDir, vRot.y, system_clock::now());
		player->GetReckoner()->SetDirVec(worldDir);
		player->GetReckoner()->SetRotateY(vRot.y);
		player->GetReckoner()->SetLocalPos(g_Object.find(g_myid)->second->Transform()->GetLocalPos());

		Vector2 real(g_Object.find(g_myid)->second->Transform()->GetLocalPos().x, g_Object.find(g_myid)->second->Transform()->GetLocalPos().z);
		Vector2 follower(player->GetReckoner()->GetLocalPos().x, player->GetReckoner()->GetLocalPos().z);



		//float distance = (real.x - follower.x) * (real.x - follower.x) + (real.y - follower.y) * (real.y - follower.y);
		//cout << "***********recokner***********" << endl;
		//cout << player->GetReckoner()->GetLocalPos().x << ", " << player->GetReckoner()->GetLocalPos().z << endl;
		//cout << "***********REAL***********" << endl;
		//cout << real.x << ", " << real.y << endl;

		//cout << "거리 : " << distance << endl;
		//cout << "오차 : " << (m_fSpeed * DT) * (m_fSpeed * DT) << endl;
		//cout << "클라 송신 횟수 : " << movePacketSendCnt << endl << endl;
	}



	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		MeshRender()->SetMaterial(m_pOriginMtrl);
	}



}


void CPlayerScript::op_Move()
{
	sc_packet_move* p = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetOtherMovePacket();
	CPlayerScript* pScript = g_Object.find(g_myid)->second->GetScript<CPlayerScript>();
	//cout <<"op MOVE  :  "<< p->id << endl;
	if (p == nullptr)
		return;
	if (g_Object.count(p->id) == 0)return;

	pScript->Search_Origin_Points(p->id, pScript->GetRTT());
	pScript->Compute_Bezier( pScript->GetOriginPoint(), pScript->GetInterpolationPoint());

	CTransform* ObjTrans = g_Object.find(p->id)->second->Transform();;
	ObjTrans->SetLocalRot(Vector3(0.f, p->rotateY, 0.f));
	ObjTrans->SetLocalPos(Vector3(pScript->GetInterpolationPoint()[pScript->GetInterpolationCnt()].x,
		p->localVec.y, 
		pScript->GetInterpolationPoint()[pScript->GetInterpolationCnt()].y));

	pScript->InterpolationCnt_PP();

	if (pScript->GetInterpolationCnt() == 4)
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->DeleteOherMovePaacket();


	//CTransform* ObjTrans = g_Object.find(p->id)->second->Transform();;
	//ObjTrans->SetLocalRot(Vector3(0.f, p->rotateY, 0.f));
	//ObjTrans->SetLocalPos(p->localVec);
	//switch (p->dir)
	//{
	//case MV_FRONT:
	//case MV_LEFT:
	//case MV_RIGHT:
	//	SetAnimation(p->id, Ani_TYPE::WALK_F);
	//	break;
	//case MV_BACK:
	//	SetAnimation(p->id, Ani_TYPE::WALK_D);
	//	break;
	//case MV_IDLE:
	//	SetAnimation(p->id, Ani_TYPE::IDLE);
	//	break;
	//default:
	//	cout << "Unknown Direction from Client move packet!\n";
	//	DebugBreak();
	//	exit(-1);
	//}
}

void CPlayerScript::SetOtherMovePacket(sc_packet_move* p, const float& rtt)
{
	 m_movePacketTemp = new sc_packet_move;
	 m_movePacketTemp = p; 
	 m_fRTT = rtt;
}


void CPlayerScript::Search_Origin_Points(const int& id, const float& rtt)
{
	CTransform* ObjTrans = g_Object.find(id)->second->Transform();
	Vector3 tempLocalPos = ObjTrans->GetLocalPos();
	float tempSpeed = g_Object.find(id)->second->GetScript<CPlayerScript>()->GetSpeed();
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
	for (int i = 4; i < 1; i--) {
		tempLocalPos += tempWorldDir * tempSpeed * rtt / i;
		g_Object.find(id)->second->GetScript<CPlayerScript>()->SetInterpolation_Point(4 - i, tempLocalPos.x, tempLocalPos.z);
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

	for (i = 0; i < 4; i++)
	{
		dest[i] = g_Object.find(g_myid)->second->
			GetScript<CPlayerScript>()->Search_Interpolation_Points(points, i * dt);
	}
}

void CPlayerScript::OnPlayerUpdateCallback()
{
	
}

void CPlayerScript::SetAnimation(const Ani_TYPE& type)
{
	g_Object.find(g_myid)->second->Animator3D()->SetBones(m_pAniData[(int)type]->GetBones());
	g_Object.find(g_myid)->second->Animator3D()->SetAnimClip(m_pAniData[(int)type]->GetAnimClip());
	g_Object.find(g_myid)->second->MeshRender()->SetMesh(m_pAniData[(int)type]);
	m_eAniType = type;
}

void CPlayerScript::SetAnimation(const int& other_id, const Ani_TYPE& type)
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
