#include "stdafx.h"
#include "RenderMgr.h"

#include "Device.h"
#include "Camera.h"

#include "SceneMgr.h"
#include "ConstantBuffer.h"

#include "TimeMgr.h"
#include "EventMgr.h"
#include "ResMgr.h"

#include "Light3D.h"
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
	float arrColor[4] = { 0.f,0.f, 0.f, 1.f };
	CDevice::GetInst()->render_start(arrColor);

	// �������� ������ ������Ʈ
	static CConstantBuffer* pGlobalBuffer = CDevice::GetInst()->GetCB(CONST_REGISTER::b5);	
	CDevice::GetInst()->SetConstBufferToRegister(pGlobalBuffer, pGlobalBuffer->AddData(&g_global));

	// ���� ���� ������Ʈ
	UpdateLight2D();
	UpdateLight3D();

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

	// Render Light
	render_lights();
		
	// Merge (Diffuse Target, Diffuse Light Target, Specular Target )		
	merge_light();

	// Forward Render
	m_vecCam[0]->render_forward(); // skybox, grid, ui

	//=================================
	// �߰� ī�޶�� forward render ��
	//=================================
	for (int i = 1; i < m_vecCam.size(); ++i)
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
	UpdateLight2D();
	UpdateLight3D();
}

void CRenderMgr::UpdateLight2D()
{
	static CConstantBuffer* pLight2DBuffer = CDevice::GetInst()->GetCB(CONST_REGISTER::b3);

	UINT iOffsetPos = pLight2DBuffer->AddData(&m_tLight2DInfo);
	CDevice::GetInst()->SetConstBufferToRegister(pLight2DBuffer, iOffsetPos);

	m_tLight2DInfo.iCount = 0;
}

void CRenderMgr::UpdateLight3D()
{
	static CConstantBuffer* pLight3DBuffer = CDevice::GetInst()->GetCB(CONST_REGISTER::b4);
	
	tLight3DInfo tLight3DArray;

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		const tLight3D& info = m_vecLight3D[i]->GetLight3DInfo();
		tLight3DArray.arrLight3D[i] = info;
	}
	tLight3DArray.iCurCount = (UINT)m_vecLight3D.size();
	
	UINT iOffsetPos = pLight3DBuffer->AddData(&tLight3DArray);
	CDevice::GetInst()->SetConstBufferToRegister(pLight3DBuffer, iOffsetPos);
}


void CRenderMgr::render_lights()
{
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	// ������ �׸���.
	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->Light3D()->render();
	}

	m_vecLight3D.clear();
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
