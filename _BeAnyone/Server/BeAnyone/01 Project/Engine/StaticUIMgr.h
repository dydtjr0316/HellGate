#pragma once
class CGameObject;
class CTexture;
class CButton;

enum class UI_TYPE
{
	PRIVATE_ITEM_UI,
	PUBLIC_SHOP_UI,
};

class CStaticUIMgr
{

private:
	Ptr<CTexture>			m_pFrame;
	UI_TYPE					m_eType;
	
	vector<CGameObject*>	m_vecItemObj;
	vector<CButton*>		m_vecButton;

	bool					m_bActive;

public:
	void update();
	

private:
	CStaticUIMgr();
	virtual ~CStaticUIMgr();
};



