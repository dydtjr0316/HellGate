#include "pch.h"
#include "Camera.h"

#include "Transform.h"
#include "Device.h"
#include "SceneMgr.h"
#include "RenderMgr.h"
#include "Scene.h"
#include "Layer.h"

#include "KeyMgr.h"
#include "TimeMgr.h"

#include "MeshRender.h"
#include "Collider.h"

#define COLLIDER_RENDERER 0

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_frustum(this)
	, m_fFar(1000.f)
	, m_fNear(1.f)
	, m_fFOV(XM_PI / 4.f)
	, m_fScale(1.f)
	, m_eProjType(PROJ_TYPE::PERSPECTIVE)
	, m_iLayerCheck(0)
	, m_bModule(false)
{
}

CCamera::~CCamera()
{
}

void CCamera::finalupdate()
{
	// 뷰행렬
	Vector3 vPos = Transform()->GetWorldPos();
	Matrix matViewTrans = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

	Matrix matViewRot = XMMatrixIdentity();
	Vector3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vector3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vector3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vRight.x; matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y; matViewRot._22 = vUp.y; matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z; matViewRot._32 = vUp.z; matViewRot._33 = vFront.z;

	m_matView = matViewTrans * matViewRot;

	// 투영행렬
	m_matProj = XMMatrixIdentity();

	tResolution res = CRenderMgr::GetInst()->GetResolution();

	if (m_eProjType == PROJ_TYPE::PERSPECTIVE)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, res.fWidth / res.fHeight, m_fNear, m_fFar);
	}
	else
	{
		m_matProj = XMMatrixOrthographicLH(res.fWidth * m_fScale, res.fHeight * m_fScale, m_fNear, m_fFar);
		//m_matProj = XMMatrixOrthographicOffCenterLH(0.f, res.fWidth, res.fHeight, 0.f, m_fNear, m_fFar);		
	}

	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	m_frustum.finalupdate();

	if(!m_bModule)
		CRenderMgr::GetInst()->RegisterCamera(this);

	//	충돌 테스트용 분기문
	if (KEY_HOLD(KEY_TYPE::KEY_NUM0))
		COLLIDER_RENDERER != COLLIDER_RENDERER; 
}

void CCamera::SortGameObject()
{
	//	 여기 들어오기 전에 이미 터져있음
	//	vecDeferred 벡터안에 Terrain 게임 오브젝트 안에 MeshRender 컴포넌트의 vertexData값이 쓰레기값이 됌.
	auto ab = m_vecDeferred;
	m_vecDeferred.clear();
	m_vecForward.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT i = 0; i < vecObj.size(); ++i)
			{
				if (!vecObj[i]->GetFrustumCheck()
					|| m_frustum.CheckFrustumSphere(vecObj[i]->Transform()->GetWorldPos(), vecObj[i]->Transform()->GetMaxScale()))
				{
					if (vecObj[i]->MeshRender()
						&& vecObj[i]->MeshRender()->GetMesh() != nullptr
						&& vecObj[i]->MeshRender()->GetSharedMaterial() != nullptr
						&& vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader() != nullptr)
					{
						if (SHADER_POV::DEFERRED == vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader()->GetShaderPOV())
							m_vecDeferred.push_back(vecObj[i]);
						else if (SHADER_POV::FORWARD == vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader()->GetShaderPOV())
							m_vecForward.push_back(vecObj[i]);
					}
				}
			}
		}
	}
	auto a = m_vecDeferred;
	auto b = m_vecForward;
}

void CCamera::render_deferred()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProjInv = m_matProjInv;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (size_t i = 0; i < m_vecDeferred.size(); ++i)
	{
		if (m_vecDeferred[i]->GetUiRenderCheck() == true) {
			m_vecDeferred[i]->MeshRender()->render();
		}

		if (m_vecDeferred[i]->Collider() && COLLIDER_RENDERER)
			m_vecDeferred[i]->Collider()->render();
	}
}

void CCamera::render_forward()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProjInv = m_matProjInv;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (size_t i = 0; i < m_vecForward.size(); ++i)
	{
		if (m_vecForward[i]->GetUiRenderCheck() == true) {
			m_vecForward[i]->MeshRender()->render();
		}

		if (m_vecForward[i]->Collider() && COLLIDER_RENDERER)
			m_vecForward[i]->Collider()->render();
	}

}

void CCamera::SortShadowObject()
{
	m_vecShadowObj.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = nullptr;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		pLayer = pCurScene->GetLayer(i);
		if (nullptr == pLayer || !(m_iLayerCheck & (1 << i)))
			continue;

		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (!vecObj[j]->GetFrustumCheck()
				|| m_frustum.CheckFrustumSphere(vecObj[j]->Transform()->GetWorldPos(), vecObj[j]->Transform()->GetMaxScale()))
			{
				if (vecObj[j]->MeshRender()
					&& vecObj[j]->MeshRender()->GetMesh() != nullptr
					&& vecObj[j]->MeshRender()->GetSharedMaterial() != nullptr
					&& vecObj[j]->MeshRender()->GetSharedMaterial()->GetShader() != nullptr
					&& vecObj[j]->MeshRender()->GetDynamicShadow())
				{
					m_vecShadowObj.push_back(vecObj[j]);
				}
			}
		}
	}
}

void CCamera::render_shadowmap()
{
	// 뷰행렬과 투영행렬을 광원시점 카메라의 것으로 대체해둠
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;
	g_transform.matViewInv = m_matViewInv;

	for (UINT i = 0; i < m_vecShadowObj.size(); ++i)
	{
		m_vecShadowObj[i]->MeshRender()->render_shadowmap();
	}
}

void CCamera::render()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT i = 0; i < vecObj.size(); ++i)
			{
				if (!vecObj[i]->GetFrustumCheck()
					|| m_frustum.CheckFrustumSphere(vecObj[i]->Transform()->GetWorldPos(), vecObj[i]->Transform()->GetMaxScale()))
				{
					if (vecObj[i]->MeshRender())
					{
						vecObj[i]->MeshRender()->render();
					}
				 }
			}
		}
	}
}

void CCamera::SaveToScene(FILE* _pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_fNear, sizeof(float), 1, _pFile);
	fwrite(&m_fFar, sizeof(float), 1, _pFile);

	fwrite(&m_fFOV, sizeof(float), 1, _pFile);
	fwrite(&m_fScale, sizeof(float), 1, _pFile);

	fwrite(&m_eProjType, sizeof(UINT), 1, _pFile);
	fwrite(&m_iLayerCheck, 4, 1, _pFile);
}

void CCamera::LoadFromScene(FILE* _pFile)
{
	fread(&m_fNear, sizeof(float), 1, _pFile);
	fread(&m_fFar, sizeof(float), 1, _pFile);

	fread(&m_fFOV, sizeof(float), 1, _pFile);
	fread(&m_fScale, sizeof(float), 1, _pFile);

	fread(&m_eProjType, sizeof(UINT), 1, _pFile);
	fread(&m_iLayerCheck, 4, 1, _pFile);
}
