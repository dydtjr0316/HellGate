#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"
#include "Camera.h"

#include "SceneMgr.h"
#include "ConstantBuffer.h"

#include "TimeMgr.h"
#include "EventMgr.h"
#include "ResMgr.h"

#include "Light.h"
#include "MRT.h"

CRenderMgr::CRenderMgr()
	: m_arrMRT{}
	, m_iRTVHeapSize(0)
{
}

CRenderMgr::~CRenderMgr()
{
	Safe_Delete_Array(m_arrMRT);
}

void CRenderMgr::render()
{
	// �ʱ�ȭ
	float arrColor[4] = { 0.6f, 0.6f, 0.6f, 1.f };
	CDevice::GetInst()->render_start(arrColor);

	// ���� ���� ������Ʈ
	UpdateLight();

	// SwapChain MRT �ʱ�ȭ
	UINT iIdx = CDevice::GetInst()->GetSwapchainIdx();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Clear(iIdx);

	// DeferredMRT �ʱ�ȭ
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Clear();

	// LightMRT �ʱ�ȭ
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Clear();

	// ==================================
	// Main Camera �� Deferred ������ ����
	// ==================================
	m_vecCam[0]->SortGameObject();

	// Deferred MRT ����
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
	m_vecCam[0]->render_deferred();
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->TargetToResBarrier();

	// shadowmap �����
	render_shadowmap();

	// Render Light
	render_lights();

	// Merge (Diffuse Target, Diffuse Light Target, Specular Target )		
	merge_light();

	// SwapChain MRT ����
	//m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet(1, iIdx);
	m_vecCam[0]->render_forward();

	for (size_t i = 1; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->SortGameObject();
		m_vecCam[i]->render_forward();
	}

	// ���
	CDevice::GetInst()->render_present();
}

void CRenderMgr::render_tool()
{
	// �ʱ�ȭ
	float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };
	//Clear(arrColor);

	// ���� ���� ������Ʈ
	UpdateLight();
}

void CRenderMgr::UpdateLight()
{
	static CConstantBuffer* pLightBuffer = CDevice::GetInst()->GetCB(CONST_REGISTER::b3);

	tLightInfo tLightArray;

	for (size_t i = 0; i < m_vecLight.size(); ++i)
	{
		const tLight& info = m_vecLight[i]->GetLightInfo();
		tLightArray.arrLight[i] = info;
	}
	tLightArray.iCurCount = (UINT)m_vecLight.size();

	UINT iOffsetPos = pLightBuffer->AddData(&tLightArray);
	CDevice::GetInst()->SetConstBufferToRegister(pLightBuffer, iOffsetPos);

}

void CRenderMgr::render_lights()
{
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	// ������ �׸���.
	for (size_t i = 0; i < m_vecLight.size(); ++i)
	{
		m_vecLight[i]->Light()->render();
	}

	m_vecLight.clear();
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->TargetToResBarrier();

	// SwapChain MRT ����
	UINT iIdx = CDevice::GetInst()->GetSwapchainIdx();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet(1, iIdx);
}

void CRenderMgr::merge_light()
{
	// ========================
	// Merge (Diffuse + Lights)
	// ========================
	static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	static Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");

	pMtrl->UpdateData();
	pRectMesh->render();
}

void CRenderMgr::render_shadowmap()
{
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SHADOWMAP)->Clear();
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SHADOWMAP)->OMSet();

	// ���� �������� ���̸� �׸�
	for (UINT i = 0; i < m_vecLight.size(); ++i)
	{
		if (m_vecLight[i]->GetLightInfo().iLightType != (UINT)LIGHT_TYPE::DIR)
			continue;

		m_vecLight[i]->render_shadowmap();
	}

	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SHADOWMAP)->TargetToResBarrier();
}

