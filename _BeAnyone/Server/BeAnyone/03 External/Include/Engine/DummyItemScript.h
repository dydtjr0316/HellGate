#pragma once
#include "Script.h"

class CDummyItemScript :
	public CScript
{
private:

public:
	virtual void update();

public:
	CLONE(CDummyItemScript);

	// collision
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	virtual void OnCollsion(CCollider* _pOther);

public:
	CDummyItemScript();
	virtual ~CDummyItemScript();
};

