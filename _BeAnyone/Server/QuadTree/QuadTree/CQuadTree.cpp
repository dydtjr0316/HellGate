#include "stdafx.h"
#include "CQuadTree.h"
#include "CPlayer.h"
#include "CRectangle.h"

CQuadTree::CQuadTree(const CRectangle& _boundary, const int& n)
{
		boundary = _boundary; m_icapacity = n;
}

bool CQuadTree::insert(CPlayer* p)
{
	if (!boundary.contains(p))
		return false;

	if (m_vpPlayers.size() < m_icapacity)
	{
		m_vpPlayers.push_back(p);
	}
	else
	{
		if (!m_bisDivide)
		{
			subm_bisDivide();
		}

		//이자리에 부모노드 세팅과 자료 넘겨주는 부분이 필요함

		if (m_pParent != nullptr)
		{

		}
		for (auto& obj : m_pChild)
		{
			if (obj->insert(p))return true;
		}
	}
}

void CQuadTree::subm_bisDivide()
{
	CRectangle childRect[4];
	childRect[0] = CRectangle(boundary.GetX() + boundary.GetW() / 2, boundary.GetZ() - boundary.GetH() / 2, boundary.GetW() / 2, boundary.GetH() / 2);
	childRect[1] = CRectangle(boundary.GetX() - boundary.GetW() / 2, boundary.GetZ() - boundary.GetH() / 2, boundary.GetW() / 2, boundary.GetH() / 2);
	childRect[2] = CRectangle(boundary.GetX() + boundary.GetW() / 2, boundary.GetZ() + boundary.GetH() / 2, boundary.GetW() / 2, boundary.GetH() / 2);
	childRect[3] = CRectangle(boundary.GetX() - boundary.GetW() / 2, boundary.GetZ() + boundary.GetH() / 2, boundary.GetW() / 2, boundary.GetH() / 2);
	for (int i = 0;i<4;i++)
	{
		CQuadTree* temp = new CQuadTree(childRect[i], 4);
		m_pChild.push_back(temp);
		temp->setParent(this);
	}
	m_bisDivide = true;
}

vector<CPlayer*> CQuadTree::search(CRectangle& range)
{
	vector<CPlayer*> found;
	if (!boundary.intersects(range))
		return found;
	else
	{
		for (auto& p : m_vpPlayers)
		{
			if (range.contains(p))
				found.push_back(p);
		}
	}
	if (m_bisDivide)
	{
		for (auto& obj : m_pChild)
		{
			for (auto player : obj->search(range))
			{
				found.push_back(player);
			}
		}
	}

	return found;

}
