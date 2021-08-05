#pragma once
#include "Script.h"

class CTreeScript :
	public CScript
{
private:


public:
	virtual void update();


public:
	CLONE(CTreeScript);

	// collision
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

public:
	CTreeScript();
	virtual ~CTreeScript();
};

