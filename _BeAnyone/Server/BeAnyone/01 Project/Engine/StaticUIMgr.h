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

class CStaticUIMgr
	: public CComponent
{

public:
	Ptr<CTexture>			m_pFrame;
	UI_TYPE					m_eType;
	
	vector<CGameObject*>	m_vecItemObj;
	vector<CButton*>		m_vecButton;

	bool					m_bActive;

public:
	void init();

	virtual void update();
	virtual void finalupdate();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CStaticUIMgr);

public:
	CStaticUIMgr();
	virtual ~CStaticUIMgr();

};



