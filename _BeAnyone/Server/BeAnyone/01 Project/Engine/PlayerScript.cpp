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



	if (/*(player->GetReckoner()->isFollowing() ||*/ frameCnt % 4/*(((int)CTimeMgr::GetInst()->GetFPS()/10)+1)*/ == 0//)
		&& (KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_D))
		// 1. 예측모델과의 오차가 커질때(얼마나 커졌을때할지는 다시 : 아직은 ㄱㅊ)
		// 2. 1초에 8번 보냄( 아직 판단 불가 )
		// 3. 키를 눌러야만 가능 - 이게 중요한듯 - but 뗄때도 보내주던가해야 idle 상태로 복귀 가능할듯
		)
	{
		system_clock::time_point start = system_clock::now();
		g_netMgr.Send_Move_Packet(dir, localPos, worldDir, vRot.y,start );
		player->GetReckoner()->SetDirVec(worldDir);
		player->GetReckoner()->SetRotateY(vRot.y);
		player->GetReckoner()->SetLocalPos(g_Object.find(g_myid)->second->Transform()->GetLocalPos());

		Vector2 real(g_Object.find(g_myid)->second->Transform()->GetLocalPos().x, g_Object.find(g_myid)->second->Transform()->GetLocalPos().z);
		Vector2 follower(player->GetReckoner()->GetLocalPos().x, player->GetReckoner()->GetLocalPos().z);
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
	//cout <<"op MOVE  :  "<< p->id << endl;
	if (p == nullptr)
		return;
	if (g_Object.count(p->id) == 0)return;

	CPlayerScript* pScript = g_Object.find(g_myid)->second->GetScript<CPlayerScript>();
	CPlayerScript* player = g_Object.find(p->id)->second->GetScript<CPlayerScript>();


	pScript->Search_Origin_Points(p->id, pScript->GetRTT());


	pScript->Compute_Bezier(player->GetOriginPoint(), player->GetInterpolationPoint());

	CTransform* ObjTrans = g_Object.find(p->id)->second->Transform();;
	ObjTrans->SetLocalRot(Vector3(0.f, p->rotateY, 0.f));

	if (player->GetInterpolationCnt() != 4)
	{
		ObjTrans->SetLocalPos(Vector3(player->GetInterpolationPoint()[player->GetInterpolationCnt()].x,
			p->localVec.y,
			player->GetInterpolationPoint()[player->GetInterpolationCnt()].y));
		cout << ObjTrans->GetLocalPos().x << endl;
		cout << ObjTrans->GetLocalPos().z << endl<<endl;
		player->InterpolationCnt_PP();
	}
	else
	{
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->DeleteOherMovePaacket();
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
	sc_packet_move* recvPacket = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetOtherMovePacket();
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
		/*cout << "templocalPos.z : " << tempLocalPos.z << endl;
		cout << "time stamp : " << (float)(rtt / i) << endl;
		cout << "calc       : " << (tempWorldDir.z * tempSpeed * (float)(rtt / i)) << endl;*/

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
	sc_packet_move* recvPacket = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetOtherMovePacket();
	CPlayerScript* player = g_Object.find(recvPacket->id)->second->GetScript<CPlayerScript>();
	for (i = 0; i < 4; i++)
	{
		Vector2 inst = g_Object.find(recvPacket->id)->second->
			GetScript<CPlayerScript>()->Search_Interpolation_Points(points, i * dt);

		player->SetInterpolation_Point(i, inst.x, inst.y);
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
