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

	//	충돌 테스트용 분기문
	if (KEY_HOLD(KEY_TYPE::KEY_NUM0))
	{
		BoundingBox bBox = Collider()->GetBoundingBox();
		cout << "Ceter :   \t" << bBox.Center.x << "\t" << bBox.Center.y << "\t" << bBox.Center.z << endl;
		cout << "Extents : \t" << bBox.Extents.x << "\t" << bBox.Extents.y << "\t" << bBox.Extents.z << endl << endl;
	}

	//Matrix m_matColWorld = Collider()->GetColliderWorldMat();
	/*cout << bBox.Center.x << "\t" << bBox.Center.y << "\t" << bBox.Center.z << endl;
	cout << bBox.Extents.x << "\t" << bBox.Extents.y << "\t" << bBox.Extents.z << endl;*/
	/*Matrix m_matColWorld = Transform()->GetWorldMat();
	cout << m_matColWorld._11 << "\t" << m_matColWorld._12 << "\t" << m_matColWorld._13 << "\t" << m_matColWorld._14 << endl;
	cout << m_matColWorld._21 << "\t" << m_matColWorld._22 << "\t" << m_matColWorld._23 << "\t" << m_matColWorld._24 << endl;
	cout << m_matColWorld._31 << "\t" << m_matColWorld._32 << "\t" << m_matColWorld._33 << "\t" << m_matColWorld._34 << endl;
	cout << m_matColWorld._41 << "\t" << m_matColWorld._42 << "\t" << m_matColWorld._43 << "\t" << m_matColWorld._44 << endl;
	*/

	//system("cls");
}


void CPlayerScript::OnPlayerUpdateCallback()
{
	CTerrain* pTerrain = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->GetTerrain();
	// 용석
	// 터레인 객체 CGameObject로 바꾸라고 말하기 // 바꾸면 전체적으로 다 수정들어가야됨

	Vector3 xmf3Scale = g_Object.find(g_myid)->second->GetScript<CPlayerScript>()->Transform()->GetLocalScale();

	//	영문서버
	Vector3 localPos = g_Object.find(g_myid)->second->Transform()->GetLocalPos();
	//if (isInMap(localPos))
	//{

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

		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
		g_netMgr.Send_Move_Packet(MV_BACK, g_Object.find(g_myid)->second->Transform()->GetLocalPos());
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

		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
		g_netMgr.Send_Move_Packet(MV_BACK, g_Object.find(g_myid)->second->Transform()->GetLocalPos());
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

		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
		g_netMgr.Send_Move_Packet(MV_BACK, g_Object.find(g_myid)->second->Transform()->GetLocalPos());
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

		g_Object.find(g_myid)->second->Transform()->SetLocalPos(localPos);
		g_netMgr.Send_Move_Packet(MV_BACK, g_Object.find(g_myid)->second->Transform()->GetLocalPos());
	}




	//if (KEY_HOLD(KEY_TYPE::KEY_W) || KEY_HOLD(KEY_TYPE::KEY_A) || KEY_HOLD(KEY_TYPE::KEY_S) || KEY_HOLD(KEY_TYPE::KEY_D))
	//{

	//}
}

bool CPlayerScript::isInMap(const Vector3& localPos)
{
	if ((localPos.x > 200 && localPos.x < 6200) && (localPos.z > 200 && localPos.z < 6200))return true;
	else return false;
}
