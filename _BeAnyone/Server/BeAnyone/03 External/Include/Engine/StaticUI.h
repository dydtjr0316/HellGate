#pragma once
#include "Component.h"

class CGameObject;
class CTexture;
class CButton;


enum class UI_TYPE
{
	PRIVATE_ITEM_UI,


	PUBLIC_SHOP_UI,
};

class CStaticUI
	: public CComponent
{

public:
	bool					m_bActive;

	Ptr<CTexture>			m_pFrame;
	UI_TYPE					m_eType;

public:
	vector<CGameObject*>	m_vecItemObj;
	vector<CButton*>		m_vecButton;

	CGameObject*			m_pMousePoint;
	CCamera*				m_pCameraProj;

public:
	void CreatePickingObj();
	void CreateButton(CCamera*);


	CCamera* GetCameraProj() { return m_pCameraProj; }
	vector<CButton*> GetButton() { return m_vecButton; }
	UI_TYPE GetType() { return m_eType; }

	void SetCameraProj(CCamera* _cam) { m_pCameraProj = _cam; }
	void SetType(UI_TYPE _ui) { m_eType = _ui; }
	void SetButton(ITEM_ID);


public:
	void init(UI_TYPE );

	virtual void update();
	virtual void finalupdate();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CStaticUI);

public:
	CStaticUI();
	virtual ~CStaticUI();

};



