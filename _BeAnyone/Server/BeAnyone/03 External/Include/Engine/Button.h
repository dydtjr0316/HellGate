#pragma once
#include "Component.h"
#include "Ptr.h"

#include "Collider.h"
#include "Texture.h"

class CButton :
	public CComponent
{
private:
	bool					m_bActive;

	ITEM_ID					m_eItemId;
	Ptr<CTexture>			m_pItemImage;
	
public:
	void					init();

	virtual void			update();

	Ptr<CTexture> GetImage() { return m_pItemImage; }

	void SaveToScene(FILE* _pFile);
	void LoadFromScene(FILE* _pFile);

	CLONE(CButton);
public:
	CButton();
	virtual ~CButton();


};