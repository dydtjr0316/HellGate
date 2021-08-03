#pragma once
#include "Ptr.h"
#include "Script.h"
#include "Collider.h"
#include "Texture.h"
#include "Camera.h"

enum class BT_STATE
{
	NONE,
	CLICKED,

	SELL,

};

class CButton
	: public CScript
{
private:
	bool					m_bActive;

	ITEM_ID					m_eItemId;
	Ptr<CTexture>			m_pItemImage;
	CComponent* m_pComParent;

private:
	Vector3					m_vecOldPos;
	CGameObject* m_pCamera;
	BT_STATE				m_bState;

public:
	void					init();

	virtual void			update();
	virtual void			finalupdate();


	void SaveToScene(FILE* _pFile);
	void LoadFromScene(FILE* _pFile);

	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

	CLONE(CButton);

public:
	Ptr<CTexture> GetImage() { return m_pItemImage; }
	BT_STATE GetState() { return m_bState; }

	void SetCameraObj(CGameObject* _cam) { m_pCamera = _cam; }
	void SetParent(CComponent* _com) { m_pComParent = _com; }
	void SetActive(bool _b) { m_bActive = _b; }

public:
	CButton();
	virtual ~CButton();


};