#include "stdafx.h"
#include "CQuadTree.h"
#include "CPlayer.h"
#include "CRectangle.h"

CQuadTree::CQuadTree(const CRectangle& _boundary, const int& n)
{
		boundary = _boundary; capacity = n;
}

bool CQuadTree::insert(CPlayer* p)
{
	if (!boundary.contains(p))
		return false;

	if (points.size() < capacity)
	{
		points.push_back(p);
	}
	else
	{
		if (!divide)
		{
			subdivide();
		}

		//이자리에 부모노드 세팅과 자료 넘겨주는 부분이 필요함

		if (parent != nullptr)
		{

		}

		if (nw->insert(p))return true;
		else if (ne->insert(p))return true;
		else if (sw->insert(p))return true;
		else if (se->insert(p))return true;
	}
}

void CQuadTree::subdivide()
{
	CRectangle nwr(boundary.GetX() + boundary.GetW() / 2, boundary.GetZ() - boundary.GetH() / 2, boundary.GetW() / 2, boundary.GetH() / 2);
	CRectangle ner(boundary.GetX() - boundary.GetW() / 2, boundary.GetZ() - boundary.GetH() / 2, boundary.GetW() / 2, boundary.GetH() / 2);
	CRectangle swr(boundary.GetX() + boundary.GetW() / 2, boundary.GetZ() + boundary.GetH() / 2, boundary.GetW() / 2, boundary.GetH() / 2);
	CRectangle ser(boundary.GetX() - boundary.GetW() / 2, boundary.GetZ() + boundary.GetH() / 2, boundary.GetW() / 2, boundary.GetH() / 2);
	nw = new CQuadTree(nwr, 4);
	ne = new CQuadTree(ner, 4);
	sw = new CQuadTree(swr, 4);
	se = new CQuadTree(ser, 4);
	nw->setParent(this);
	ne->setParent(this);
	sw->setParent(this);
	se->setParent(this);
	divide = true;
}

vector<CPlayer*> CQuadTree::search(CRectangle& range)
{
	vector<CPlayer*> found;
	if (!boundary.intersects(range))
		return found;
	else
	{
		for (auto& p : points)
		{
			if (range.contains(p))
				found.push_back(p);
		}
	}
	if (divide)
	{
		for (auto& obj : nw->search(range))
		{
			found.push_back(obj);
		}
		for (auto& obj : ne->search(range))
		{
			found.push_back(obj);
		}
		for (auto& obj : sw->search(range))
		{
			found.push_back(obj);
		}
		for (auto& obj : se->search(range))
		{
			found.push_back(obj);
		}
		/*found.insert(found.begin(), nw->search(range).begin(), nw->search(range).end());
		found.insert(found.begin(), ne->search(range).begin(), ne->search(range).end());
		found.insert(found.begin(), sw->search(range).begin(), sw->search(range).end());
		found.insert(found.begin(), se->search(range).begin(), se->search(range).end());*/
	}

	return found;

}
