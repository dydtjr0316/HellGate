#include "pch.h"
#include "DummyItemScript.h"

CDummyItemScript::CDummyItemScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
{

}

CDummyItemScript::~CDummyItemScript()
{

}

void CDummyItemScript::update()
{

}

void CDummyItemScript::OnCollisionEnter(CCollider* _pOther)
{
	cout << "¾¾¹ß ¾ÆÀÌÅÛ ´êÀ½" << endl;
}

void CDummyItemScript::OnCollisionExit(CCollider* _pOther)
{
}

void CDummyItemScript::OnCollsion(CCollider* _pOther)
{
}