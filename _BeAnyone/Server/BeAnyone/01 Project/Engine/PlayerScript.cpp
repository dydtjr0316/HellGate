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
	//Vector3 vPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();
	////Vector3 vRot = Transform()->GetLocalRot();

	Vector3 localPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();

	OnPlayerUpdateCallback();

	// 이부분 필요 없다면 삭제 // 용석
	// z 키를 누르면 z 축 회전
	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		//vRot.z += DT * XM_PI;

		// 복사 메테리얼을 MeshRender 에 세팅
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		// z 키를 떼면 원레 메테리얼로 돌아감
		MeshRender()->SetMaterial(m_pOriginMtrl);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
	{
		Vector2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		Vector3 vRot = g_Object.find(g_myid)->second->Transform()->GetLocalRot();

		vRot.y += vDrag.x * DT * ROTATE_SPEED;
		
		g_netMgr.Send_Rotate_Packet(Rotate_LBTN, vRot.y);

		g_Object.find(g_myid)->second->Transform()->SetLocalRot(vRot);

	}
	
	// 용석
	// 이건뭔지 영문이한테 물어보기

	//g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);

	// 용석
	// 이자리에 send packet 한번 하면되남? // 아니지 그럼 매프레임 패킷이가는구나
	// OnPlayerUpdateCallback에서 send 패킷하기 
	// send패킷에는 local좌표만 남기고 삭제 하는 방식으로 수정
}


Vector3 CPlayerScript::OnPlayerUpdateCallback()
{
	CTerrain* pTerrain = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain();
	// 용석
	// 터레인 객체 CGameObject로 바꾸라고 말하기 // 바꾸면 전체적으로 다 수정들어가야됨

	Vector3 xmf3Scale = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->Transform()->GetLocalScale();

	//	영문서버
	Vector3 localPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();

	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		localPos += -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200.f * DT;

		cout << localPos.x << "\t" << localPos.y << "\t" << localPos.z << endl;
		//g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		localPos += g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 200.f * DT;

		//g_netMgr.Send_Move_Packet(MV_BACK, OnPlayerUpdateCallback());


		//OnPlayerUpdateCallback();


		//g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		//vPos.x -= DT * 200.f;
		localPos += g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200.f * DT;
		//g_netMgr.Send_Move_Packet(MV_BACK, OnPlayerUpdateCallback());

		//g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		localPos += -g_Object.find(g_myid)->second->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 200.f * DT;
		//g_netMgr.Send_Move_Packet(MV_BACK, OnPlayerUpdateCallback());


		//g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
	}

	localPos.y -= m_xmf3Velocity.y;

	int z = (int)(localPos.z / xmf3Scale.z);
	bool bReverseQuad = ((z % 2) != 0);
	float fHeight = pTerrain->GetHeight(localPos.x, localPos.z, bReverseQuad) * 1.f + 30.0f;

	if (localPos.y < fHeight)
	{
		XMFLOAT3 xmf3PlayerVelocity = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetVelocity();
		xmf3PlayerVelocity.y = 0.0f;
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetVelocity(xmf3PlayerVelocity);
		localPos.y = fHeight;
		
	}

	if (localPos.y > fHeight + 6.0f)
	{
		m_xmf3Velocity.y = 0.3f;
		g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->SetVelocity(localPos);
	}

	g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);

	g_netMgr.Send_Move_Packet(MV_BACK, g_Object.find(g_myid)->second->Transform()->GetLocalPos());

	
	return g_Object.find(g_myid)->second->Transform()->GetLocalPos();
	// 용석
	// 이자리에 send packet부분 들어가고 이함수를 wasd키에 넣는게 제일 나을듯
}
