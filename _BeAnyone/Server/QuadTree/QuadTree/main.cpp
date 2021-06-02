#include"stdafx.h"
#include "CPlayer.h"
#include "CNode.h"
#include "CRect.h"
#include "CQuadTree.h"

int main()
{
	CQuadTree* tree = new CQuadTree;
	CPlayer* p = new CPlayer;
	p->x = 100.f;
	p->z = 100.f;
	tree->insert(p->x, p->z, tree->GetRoot(), false);



	return 0;
}