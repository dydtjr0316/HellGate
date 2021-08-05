#include "pch.h"
#include "ItemScript.h"

CItemScript::CItemScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
{

}

CItemScript::~CItemScript()
{

}

void CItemScript::update()
{

}

void CItemScript::OnCollisionEnter(CCollider* _pOther)
{
	cout << "¾¾¹ß ¾ÆÀÌÅÛ ´êÀ½" << endl;
}

void CItemScript::OnCollisionExit(CCollider* _pOther)
{
}

void CItemScript::OnCollsion(CCollider* _pOther)
{
}