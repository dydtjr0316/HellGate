#pragma once
#include "Script.h"
#include "Collider.h"
#include "Texture.h"


class CButton :
	public CGameObject
{
private:
	bool					m_bActive;

	Ptr<CTexture>			m_pItemImage;
	vector<CGameObject*>	m_vecItemObj;
	
public:
	virtual void	update();

	void			ShowUI();

public:
	CButton();
	virtual ~CButton();
};