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


public:
	void init();

	virtual void update();
	virtual void finalupdate();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	CLONE(CStaticUI);

public:
	CStaticUI();
	virtual ~CStaticUI();

};



