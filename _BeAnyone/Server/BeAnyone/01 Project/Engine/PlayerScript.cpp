#include "pch.h"
#include "PlayerScript.h"
#include <iostream>

using namespace std;

bool checkOnce = true;

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

	char dir = MV_IDLE;
	bool moveKeyInput = false;

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
		localPos += -playerTrans->GetWorldDir(DIR_TYPE::FRONT) * player->GetSpeed() * DT;

		dir = MV_FRONT;
		player->SetAnimation(Ani_TYPE::WALK_F);
		moveKeyInput = true;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		localPos += playerTrans->GetWorldDir(DIR_TYPE::FRONT) * player->GetSpeed() * DT;

		dir = MV_BACK;
		player->SetAnimation(Ani_TYPE::WALK_D);
		moveKeyInput = true;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		localPos += playerTrans->GetWorldDir(DIR_TYPE::RIGHT) * player->GetSpeed() * DT;

		dir = MV_LEFT;
		player->SetAnimation(Ani_TYPE::WALK_F);
		moveKeyInput = true;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		localPos += -playerTrans->GetWorldDir(DIR_TYPE::RIGHT) * player->GetSpeed() * DT;

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
		g_netMgr.Send_Move_Packet(dir, localPos, vRot.y);
	}
	else
	{
		player->SetAnimation(Ani_TYPE::IDLE);
		g_netMgr.Send_Move_Packet(dir, localPos, vRot.y);
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

bool CPlayerScript::isInMap(const Vector3& localPos)
{
	if ((localPos.x > 200 && localPos.x < 6200) && (localPos.z > 200 && localPos.z < 6200))return true;
	else return false;
}
