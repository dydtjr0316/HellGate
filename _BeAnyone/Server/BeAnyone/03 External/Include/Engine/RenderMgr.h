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

	vector<CLight*>			m_vecLight;
	vector<CCamera*>		m_vecCam;

	UINT					m_iRTVHeapSize;

	HWND					m_hWnd;
	bool					m_bWindowed;

	// npcCam 때문에 vecCam의 순서가 바뀌어서 쓰는 거
	int						m_MainCamNum;

public:
	void init(HWND _hWnd, const tResolution& _res, bool _bWindow);
	void render();
	void render_tool();

	void render_lights();
	void merge_light();
	void render_shadowmap();

private:
	void CreateMRT();

	//void UpdateLight2D();
	void UpdateLight();

public:
	
	int RegisterLight(CLight* _pLight3D) {
		if (m_vecLight.size() >= 100)
			return -1;
		m_vecLight.push_back(_pLight3D);
		return (int)m_vecLight.size() - 1;
	}

	CCamera* GetCamera(int _iIdx) { return m_vecCam[_iIdx]; }
	CCamera* GetMainCam() {	return  m_vecCam[0]; }
	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void ClearCamera() { m_vecCam.clear(); }

	tResolution GetResolution() { return m_tResolution; }
	HWND GetHwnd() { return m_hWnd; }
	UINT GetRTVHeapSize() { return m_iRTVHeapSize; }

	CMRT* GetMRT(MRT_TYPE _eType) { return m_arrMRT[(UINT)_eType]; }

	friend class CSceneMgr;
};


