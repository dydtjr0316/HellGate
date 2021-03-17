#include "pch.h"
#include "RenderMgr.h"

#include "Device.h"
#include "Camera.h"

#include "SceneMgr.h"
#include "ConstantBuffer.h"

#include "TimeMgr.h"
#include "EventMgr.h"

#include "Light.h"

CRenderMgr::CRenderMgr()
	: m_arrRT{}
	, m_arrMRT{}
{
}

CRenderMgr::~CRenderMgr()
{
}

void CRenderMgr::render()
{
	// �ʱ�ȭ
	float arrColor[4] = { 0.6f,0.6f, 0.6f, 1.f };
	CDevice::GetInst()->render_start(arrColor);

	// ���� ���� ������Ʈ
	//UpdateLight2D();
	UpdateLight();

	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render();
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
	//UpdateLight2D();
	//UpdateLight3D();
}
//
//void CRenderMgr::UpdateLight2D()
//{
//	static CConstantBuffer* pLight2DBuffer = CDevice::GetInst()->GetCB(CONST_REGISTER::b3);
//
//	UINT iOffsetPos = pLight2DBuffer->AddData(&m_tLight2DInfo);
//	CDevice::GetInst()->SetConstBufferToRegister(pLight2DBuffer, iOffsetPos);
//
//	m_tLight2DInfo.iCount = 0;
//}
//
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

	m_vecLight.clear();
}
