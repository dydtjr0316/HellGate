#include "stdafx.h"
#include "CQuadTree.h"
#include "CQuad.h"
#include "CRect.h"
#include "CPlayer.h"
int main()
{
	//d f
	CQuadTree* quadtree = new CQuadTree();
	quadtree->insert(10, 10, quadtree->GetRoot());
	quadtree->insert(500, 500, quadtree->GetRoot());
	quadtree->insert(600, 30, quadtree->GetRoot());
	quadtree->insert(40, 700, quadtree->GetRoot());
	// 용석 테스트
	for (auto& e : quadtree->GetRoot()->GetInfo())
	{
		cout << e.second->GetRect()->GetX() << endl;
	}

	return 0;
}