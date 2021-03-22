#include "stdafx.h"
#include "CQuadTree.h"
#include "CPlayer.h"
CQuadTree* quadtree;
int main()
{
	quadtree->insert(10, 10, quadtree->GetRoot());
	return 0;
}