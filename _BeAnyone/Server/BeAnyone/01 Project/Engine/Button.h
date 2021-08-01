#pragma once
//#include "Component.h"
#include "Ptr.h"
#include "Script.h"
#include "Collider.h"
#include "Texture.h"

class CButton :
	public CScript
{
private:
	bool					m_bActive;

	ITEM_ID					m_eItemId;
	Ptr<CTexture>			m_pItemImage;
	CComponent*				m_pComParent;

public:
	void					init();

	virtual void			update();
	virtual void			finalupdate();


	void SaveToScene(FILE* _pFile);
	void LoadFromScene(FILE* _pFile);

	CLONE(CButton);

public:
	Ptr<CTexture> GetImage() { return m_pItemImage; }
	void SetParent(CComponent* _com) { m_pComParent = _com; }
	void SetActive(bool _b) { m_bActive = _b; }

public:
	CButton();
	virtual ~CButton();


};