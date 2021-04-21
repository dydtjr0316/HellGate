#pragma once

class CBlendState;
class CDepthStencilState;
class CCamera;
class CLight3D;

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

	//CRenderTarget24*		m_arrRT[(UINT)RT_TYPE::END];
	CMRT*					m_arrMRT[(UINT)MRT_TYPE::END];
 
	vector<CLight3D*>		m_vecLight3D;
	vector<CCamera*>		m_vecCam;

	UINT					m_iRTVHeapSize;

	HWND					m_hWnd;
	bool					m_bWindowed;

public:
	void init(HWND _hWnd, const tResolution& _res, bool _bWindow);
	void render();
	void render_tool();


	void render_shadowmap();
	void render_lights();
	void merge_light();

private:
	void CreateMRT();

	void UpdateLight3D();


public:
	int RegisterLight3D(CLight3D* _pLight3D) {
		if (m_vecLight3D.size() >= 100)
			return -1;
		m_vecLight3D.push_back(_pLight3D);
		return (int)m_vecLight3D.size() - 1;
	}

	CCamera* GetCamera(int _iIdx) { return m_vecCam[_iIdx]; }
	CCamera* GetMainCam();
	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void ClearCamera() { m_vecCam.clear(); }

	tResolution GetResolution() { return m_tResolution; }
	HWND GetHwnd() { return m_hWnd; }
	UINT GetRTVHeapSize() { return m_iRTVHeapSize; }

	CMRT* GetMRT(MRT_TYPE _eType) { return m_arrMRT[(UINT)_eType]; }

	void CopySwapToPosteffect();


	friend class CSceneMgr;
};


