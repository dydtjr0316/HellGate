#pragma once

class CBlendState;
class CDepthStencilState;
class CCamera;
class CLight;

class CRenderTarget24;
class CMRT;

#include "Ptr.h"
#include "Texture.h"

class CRenderMgr
{
	SINGLE(CRenderMgr);

private:
	Ptr<CTexture>			m_pRenderTargetTex;
	Ptr<CTexture>			m_pDepthStencilTex;

	tResolution				m_tResolution;

	// CRenderTarget24*		m_arrRT[(UINT)RT_TYPE::END];
	CMRT*					m_arrMRT[(UINT)MRT_TYPE::END];

	//tLight2DInfo			m_tLight2DInfo;

	vector<CLight*>			m_vecLight;
	vector<CCamera*>		m_vecCam;

	UINT					m_iRTVHeapSize;

	HWND					m_hWnd;
	bool					m_bWindowed;

public:
	void init(HWND _hWnd, const tResolution& _res, bool _bWindow);
	void render();
	void render_tool();

private:
	void CreateMRT();

	//void UpdateLight2D();
	void UpdateLight();

public:
	
	void RegisterLight(CLight* _pLight) { if (m_vecLight.size() >= 100) return; m_vecLight.push_back(_pLight); }

	CCamera* GetCamera(int _iIdx) { return m_vecCam[_iIdx]; }
	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void ClearCamera() { m_vecCam.clear(); }

	tResolution GetResolution() { return m_tResolution; }
	HWND GetHwnd() { return m_hWnd; }
	UINT GetRTVHeapSize() { return m_iRTVHeapSize; }

	CMRT* GetMRT(MRT_TYPE _eType) { return m_arrMRT[(UINT)_eType]; }

	friend class CSceneMgr;
};


