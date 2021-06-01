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
	OnPlayerUpdateCallback();

	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		MeshRender()->SetMaterial(m_pOriginMtrl);
	}


	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vector3 vRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();

		vRot.y += vDrag.x * DT * ROTATE_SPEED * 6;
		
		g_netMgr.Send_Rotate_Packet(Rotate_LBTN, vRot.y);

		g_Object.find(g_myid)->second->Transform()->SetLocalRot(vRot);

	}

	

}


void CPlayerScript::OnPlayerUpdateCallback()
{
	CTerrain* pTerrain = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain();
	// 용석
	// 터레인 객체 CGameObject로 바꾸라고 말하기 // 바꾸면 전체적으로 다 수정들어가야됨

	Vector3 xmf3Scale = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->Transform()->GetLocalScale();

	//	영문서버
	Vector3 localPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();
	

	
	char dir = MV_IDLE;
	
	float speed = 200.f;

	if (KEY_HOLD(KEY_TYPE::KEY_SPACE))
	{
		speed = 600.f;

	}
	if (KEY_HOLD(KEY_TYPE::KEY_R))
	{
		Vector3 ve = Vector3(0.9f, 0.9f, 0.9f);
		Transform()->SetLocalScale(ve);
		
	}

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		localPos += -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * speed * DT;
		int z = (int)(localPos.z / xmf3Scale.z);
		bool bReverseQuad = ((z % 2) != 0);
		float fHeight = pTerrain->GetHeight(localPos.x, localPos.z, bReverseQuad) * 2.f + 100.0f;

		//cout << " x :\t" << localPos.x << " y :\t" << localPos.y << " z :\t" << localPos.z <<  "  DT  :\t" << DT <<"  FPS  :\t"<< CTimeMgr::GetInst()->GetFPS()<< endl;

		if (localPos.y != fHeight)
		{
			localPos.y = fHeight;
		}
		dir = MV_FRONT;
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::WALK_F);


		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
		//g_netMgr.Send_Move_Packet(MV_FRONT, g_Object.find(g_myid)->second->Transform()->GetLocalPos());

		//	충돌체크시 방향벡터 한개만 우선 체크
		m_vecPlayerDir = -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * speed * DT;

	}

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		localPos += g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * speed * DT;

		int z = (int)(localPos.z / xmf3Scale.z);
		bool bReverseQuad = ((z % 2) != 0);
		float fHeight = pTerrain->GetHeight(localPos.x, localPos.z, bReverseQuad) * 2.f + 100.f;

		if (localPos.y != fHeight)
		{
			localPos.y = fHeight;
		}
		dir = MV_BACK;

		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::WALK_D);
		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
		//g_netMgr.Send_Move_Packet(MV_BACK, g_Object.find(g_myid)->second->Transform()->GetLocalPos());
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		//vPos.x -= DT * 200.f;
		localPos += g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * speed * DT;

		int z = (int)(localPos.z / xmf3Scale.z);
		bool bReverseQuad = ((z % 2) != 0);
		float fHeight = pTerrain->GetHeight(localPos.x, localPos.z, bReverseQuad) * 2.f + 100.f;

		if (localPos.y != fHeight)
		{
			localPos.y = fHeight;
		}
		dir = MV_FRONT;
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::WALK_F);
		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
		//g_netMgr.Send_Move_Packet(MV_LEFT, g_Object.find(g_myid)->second->Transform()->GetLocalPos());
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		localPos += -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * speed * DT;

		int z = (int)(localPos.z / xmf3Scale.z);
		bool bReverseQuad = ((z % 2) != 0);
		float fHeight = pTerrain->GetHeight(localPos.x, localPos.z, bReverseQuad) * 2.f + 100.f;

		if (localPos.y != fHeight)
		{
			localPos.y = fHeight;
		}
		dir = MV_FRONT;
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::WALK_F);
		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
		//g_netMgr.Send_Move_Packet(MV_RIGHT, g_Object.find(g_myid)->second->Transform()->GetLocalPos());

		
	}

	if (KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_D) || KEY_HOLD(KEY_TYPE::KEY_S))
	{
		g_netMgr.Send_Move_Packet(dir,localPos);

	}
	else if (KEY_AWAY(KEY_TYPE::KEY_W) || KEY_AWAY(KEY_TYPE::KEY_A) || KEY_AWAY(KEY_TYPE::KEY_D) || KEY_AWAY(KEY_TYPE::KEY_S)) {
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetAnimation(Ani_TYPE::IDLE);
		g_netMgr.Send_Move_Packet(dir, localPos);
	}
}

void CPlayerScript::SetAnimation(const Ani_TYPE& type)
{
	g_Object.find(g_myid)->second->Animator3D()->SetBones(m_pAniData[(int)type]->GetBones());
	g_Object.find(g_myid)->second->Animator3D()->SetAnimClip(m_pAniData[(int)type]->GetAnimClip());
	g_Object.find(g_myid)->second->MeshRender()->SetMesh(m_pAniData[(int)type]);
		g_netMgr.Send_Move_Packet(MV_BACK, g_Object.find(g_myid)->second->Transform()->GetLocalPos());
	
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

	
	Vector3 localPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();
	localPos += -m_vecPlayerDir;
	g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);

}

void CPlayerScript::OnCollisionExit(CCollider* _pOther)
{
}

