#pragma once
#include "Component.h"

class CGameObject;
class CTexture;
class CButton;

enum class STORE_BUTTON
{
	LACK_MONEY,
	EXIT,
	END,
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

	// wallet, money etc ... (only player)
	CGameObject*			m_pWallet;
	vector<CGameObject*>	m_pMoneyUi;
	int						m_iMoney = 9999;

	// store button
	vector<CGameObject*>	m_StoreButton;

public:
	void CreatePickingObj();
	void CreateButton(CCamera*);
	void CreateStoreButton();


	CCamera* GetCameraProj() { return m_pCameraProj; }
	vector<CButton*> GetButton() { return m_vecButton; }
	UI_TYPE GetType() { return m_eType; }

	void SetCameraProj(CCamera* _cam) { m_pCameraProj = _cam; }
	void SetType(UI_TYPE _ui) { m_eType = _ui; }
	void SetButton(ITEM_ID);

	// quest
	int GetQuestItemCount();

	// UI
	void Sell();
	// money
	void SetWalletMoney();

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



