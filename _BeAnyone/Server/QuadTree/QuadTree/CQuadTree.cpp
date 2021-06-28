#include "stdafx.h"
#include "CQuadTree.h"
#include "CPlayer.h"
#include "CRectangle.h"
#include "CMediator.h"


void CQuadTree::PrintQuadTree()
{
	cout << "------------------" << endl;
	cout << "DEPTH : " << m_iDepth << endl;
	cout << "------------------" << endl;
	if (!m_bisDivide)
	{
		for (auto& obj : m_vpPlayers)
		{
			CPlayer* temp = g_Medi.Find(obj);
			cout << "ID : " << temp->GetID() << "\tPOS : (" << temp->GetX() << ", " << temp->GetZ() << ")" << endl;
		}
	}
	else
	{
		cout << "�ڽ��� �ִ� ��� �Դϴ�" << endl << endl;
		for (auto& obj : m_pChild)
		{
			if(obj->GetPoint().size()!=0)
				obj->PrintQuadTree();
		}
	}
	cout << "------------------" << endl;
}

CQuadTree::CQuadTree(const CRectangle& _m_boundary, const int& n)
{
		m_boundary = _m_boundary; m_icapacity = n;
}


bool CQuadTree::insert(CPlayer* p)
{
	if (!m_boundary.contains(p))
		return false;

	if (m_vpPlayers.size() < m_icapacity)
	{
		m_vpPlayers.emplace(p->GetID());
	}
	else
	{
		if (!m_bisDivide)
		{
			sub_Divide();
		}

		{
			////���ڸ��� �θ��� ���ð� �ڷ� �Ѱ��ִ� �κ��� �ʿ���

		//if (m_pParent != nullptr)
		//{

		//}
		}
		for (auto& obj : m_pChild)
		{
			if (obj->insert(p))return true;
		}
	}
}

void CQuadTree::Delete(CPlayer* p)
{
	// �ش� �÷��̾� id�� ������ġ�� ��带 ã�´� 
	// // �ڽĳ�尡 ���� ����� �÷��̾� ��ǥ�� ���ϴ°�
	int cnt = 0;

	if (!m_bisDivide && m_boundary.contains(p))		// player p�� �����ϴ� ������ ����ΰ�?
	{
		// ��忡�� �ش� �÷��̾ ����
		for (auto& playerID : m_vpPlayers)
		{
			if (IsSameObject(playerID, p->GetID()))
			{
				m_vpPlayers.erase(playerID);
				break;
			}
		}
		// if �ش��尡 ���� �θ��忡 �ִ� ��� �ڽĳ���� �÷��̾� ������ 4������ �Ǵ�


		// 4���̻��̸� �н�
		// 4�����϶�� 4���̻��� �θ��带 ã�� �� ���� ���ȣ��
		for (auto& obj : m_pParent->GetChild())
		{
			cnt += obj->GetPoint().size();
		}
		if (cnt < 4)
		{
			for (auto& obj : m_pParent->GetChild())
			{
				for (auto& sub : obj->GetPoint())
					m_pParent->GetPoint().emplace(sub);
				//obj->GetPoint().clear();
				SafeDelete(obj);
			}

		}
	}
	else
	{
		for (auto& childNode : m_pChild)
		{
			childNode->Delete(p);
		}
	}
}
bool CQuadTree::IsSameObject(const uShort& p1, const uShort& p2)
{
	if (p1 == p2)return true;
	else return false;
}
void CQuadTree::sub_Divide()
{
	CQuadTree* temp;
	CRectangle childRect[CHILD_NODE_SIZE];//�뼮
	childRect[0] = CRectangle(m_boundary.GetX() + m_boundary.GetW() / 2, m_boundary.GetZ() - m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[1] = CRectangle(m_boundary.GetX() - m_boundary.GetW() / 2, m_boundary.GetZ() - m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[2] = CRectangle(m_boundary.GetX() + m_boundary.GetW() / 2, m_boundary.GetZ() + m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[3] = CRectangle(m_boundary.GetX() - m_boundary.GetW() / 2, m_boundary.GetZ() + m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	for (int i = 0;i< CHILD_NODE_SIZE;i++)
	{
		temp = new CQuadTree(childRect[i], MAX_PLAYER_IN_NODE);
		m_pChild.push_back(temp);
		temp->setParent(this);
	}
	temp->m_iDepth = m_iDepth++;

	SubDivideToChild();
	m_bisDivide = true;
}
void CQuadTree::SubDivideToChild()
{
	for (auto& player : m_vpPlayers)
	{
		// ���⼭ �θ��ִ� �÷��̾����� ��ǥ�� ���� �ڽ����� �Ѱ��ְ� 
		// �θ����� player������ �����ϴ� �ڵ尡 �־����
		for (auto& childNode : m_pChild)
		{
			if (childNode->m_boundary.contains(g_Medi.Find(player)))
			{
				childNode->m_vpPlayers.emplace(player);
				break;
			}
		}
	}
	m_vpPlayers.clear();
}




unordered_set<uShort> CQuadTree::search(CRectangle& range)
{
	//  ����Ʈ�� �θ� �ڽ� ���� �ٲٸ鼭 �̺κ� �ȹٲ㵵 �Ǵ��� Ȯ���� �� ��
	unordered_set<uShort> found;
	{
		/*if (!m_boundary.intersects(range))
		return found;
	else
	{
		for (auto& p : m_vpPlayers)
		{
			if (range.contains(p))
				found.push_back(p);
		}
	}*/
	}
	if (m_boundary.intersects(range))
	{
		for (auto& p : m_vpPlayers)
		{
			if (range.contains(g_Medi.Find(p)))
				found.emplace(p);
		}
	}
	if (m_bisDivide)
	{
		for (auto& obj : m_pChild)
		{
			for (auto player : obj->search(range))
			{
				found.emplace(player);
			}
		}
	}

	return found;

}

