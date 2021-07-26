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
	SINGLE(CStaticUIMgr);

private:
	Ptr<CTexture>			m_pFrame;
	UI_TYPE					m_eType;
	
	vector<CGameObject*>	m_vecItemObj;
	vector<CButton*>		m_vecButton;

	bool					m_bActive;

public:
	void update();
	void clear() { m_vecEvent.clear(); }
	void AddEvent(tEvent& _event) { m_vecEvent.push_back(_event); }

	const vector<tEvent>& GetEventList() { return m_vecEvent; }

private:
	CStaticUIMgr();
	virtual ~CStaticUIMgr();
};



