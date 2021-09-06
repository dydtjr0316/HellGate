#pragma once
#include "Script.h"


class CPotalScript
	: public CScript
{
private:

public:
	virtual void update();

public:
	CLONE(CPotalScript);

	// collision
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);
	virtual void OnCollsion(CCollider* _pOther);

public:
	CPotalScript();
	virtual ~CPotalScript();
};

