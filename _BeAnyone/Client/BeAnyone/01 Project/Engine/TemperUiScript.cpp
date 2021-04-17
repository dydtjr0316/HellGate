#include "pch.h"
#include "TemperUiScript.h"


CTemperUiScript::CTemperUiScript()
	: CScript(0)
	, m_bUiCheck(true)
{

}

CTemperUiScript::~CTemperUiScript()
{

}

void CTemperUiScript::update()
{
	Vector3 vUiPos = m_pObject->Transform()->GetLocalPos();
	Vector3 vUiScale = m_pObject->Transform()->GetLocalScale();

	vUiPos.x += 640.f;
	vUiPos.y += -384.f;


	if (KEY_TAB(KEY_TYPE::KEY_LBTN))
	{
		POINT pMousePos = CKeyMgr::GetInst()->GetMousePos();

		if(pMousePos.x >= vUiPos.x - (vUiScale.x / 2) && pMousePos.x <= vUiPos.x + (vUiScale.x / 2)
			&& pMousePos.y <= -vUiPos.y + (vUiScale.x / 2) && pMousePos.y >= -vUiPos.y - (vUiScale.x / 2)) {

			if (m_bUiCheck == true) {
				// m_pObject->Transform()->SetLocalScale(Vector3(90.f, 90.f, 1.f));
				m_pTempBar->SetUiRenderCheck(true);
				m_pUnderTempBar->SetUiRenderCheck(true);
				m_bUiCheck = false;
			}
			else if (m_bUiCheck == false) {
				m_pTempBar->SetUiRenderCheck(false);
			 	m_pUnderTempBar->SetUiRenderCheck(false);
				m_bUiCheck = true;
			}
		}
	}
}