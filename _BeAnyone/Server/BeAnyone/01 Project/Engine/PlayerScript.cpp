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

	// �̺κ� �ʿ� ���ٸ� ���� // �뼮
	// z Ű�� ������ z �� ȸ��
	if (KEY_HOLD(KEY_TYPE::KEY_Z))
	{
		//vRot.z += DT * XM_PI;

		// ���� ���׸����� MeshRender �� ����
		MeshRender()->SetMaterial(m_pCloneMtrl);
	}
	else if (KEY_AWAY(KEY_TYPE::KEY_Z))
	{
		// z Ű�� ���� ���� ���׸���� ���ư�
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
	
	// �뼮
	// �̰ǹ��� ���������� �����

	//g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);

	// �뼮
	// ���ڸ��� send packet �ѹ� �ϸ�ǳ�? // �ƴ��� �׷� �������� ��Ŷ�̰��±���
	// OnPlayerUpdateCallback���� send ��Ŷ�ϱ� 
	// send��Ŷ���� local��ǥ�� ����� ���� �ϴ� ������� ����
}


Vector3 CPlayerScript::OnPlayerUpdateCallback()
{
	CTerrain* pTerrain = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain();
	// �뼮
	// �ͷ��� ��ü CGameObject�� �ٲٶ�� ���ϱ� // �ٲٸ� ��ü������ �� �������ߵ�

	Vector3 xmf3Scale = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->Transform()->GetLocalScale();

	//	��������
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
	// �뼮
	// ���ڸ��� send packet�κ� ���� ���Լ��� wasdŰ�� �ִ°� ���� ������
}
