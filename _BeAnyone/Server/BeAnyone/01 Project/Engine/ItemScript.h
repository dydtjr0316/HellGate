#pragma once
#include "Script.h"

class CItemScript :
	public CScript
{
private:

public:
	virtual void update();

public:
	CLONE(CItemScript);

	// collision
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	virtual void OnCollsion(CCollider* _pOther);

public:
	CItemScript();
	virtual ~CItemScript();
};
