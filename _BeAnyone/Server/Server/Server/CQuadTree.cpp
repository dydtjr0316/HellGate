#include "stdafx.h"
#include "CQuadTree.h"
#include "CGameObject.h"
#include "CBoundary.h"


void CQuadTree::PrintQuadTree()
{
	cout << "------------------" << endl;
	cout << "DEPTH : " << m_iDepth << endl;
	cout << "------------------" << endl;
	if (!m_bisDivide)
	{
		for (auto& obj : m_vpPlayers)
		{
			CGameObject* temp = Netmgr.GetMediatorMgr()->Find(obj);
			cout << "ID : " << temp->GetID() << "\tPOS : (" << temp->GetLocalPosVector().x << ", " 
				<< temp->GetLocalPosVector().z << ")" << endl;
			cout << "------------------" << endl;

		}
	}
	else
	{
		cout << "�ڽ��� �ִ� ��� �Դϴ�" << endl << endl;
		for (auto& obj : m_pChild)
		{
			obj->PrintQuadTree();
		}
	}
}

CQuadTree::CQuadTree(const CBoundary& _m_boundary, const int& n)
{
	m_boundary = _m_boundary; m_icapacity = n;
}


bool CQuadTree::Insert(CGameObject* p)
{
	if (!m_boundary.contains(p))
		return false;

	if (!m_bisDivide)
	{
		if (m_vpPlayers.size() < m_icapacity)	// leaf node �϶�
		{
			m_vpPlayers.emplace(p->GetID());
		}
		else
		{
			Sub_Divide();
			for (auto& obj : m_pChild)
				if (obj->Insert(p))return true;
		}
	}
	else
	{
		for (auto& obj : m_pChild)
			if (obj->Insert(p))return true;
	}
	//cout << endl;
	//cout << "Insert**********************" << endl;
	//PrintQuadTree();
	//return true;// ���⼭ true ��ȯ�ϴ°� �������� ����
}

void CQuadTree::Delete(CGameObject* p)
{
	// �ش� �÷��̾� id�� ������ġ�� ��带 ã�´� 
	// // �ڽĳ�尡 ���� ����� �÷��̾� ��ǥ�� ���ϴ°�
	int cnt = 0;
	this;

	if (!m_bisDivide && m_boundary.contains(p))		// player p�� �����ϴ� ���� ����ΰ�?
	{
		//cout << "Test ���" << endl;
		//cout << m_boundary.GetX() << " - " << m_boundary.GetZ() << " - "
		//	<< m_boundary.GetW() << " - " << m_boundary.GetH() << endl;
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
		if (cnt <= 4)
		{
			for (auto& obj : m_pParent->GetChild())
			{
				for (auto& sub : obj->GetPoint())	//0629 �̱�ó �κ� ���׹����� m_vplayer�����Ÿ� �װŸ� ���°ɷ� �����ϱ� ���� �򰥸��� ����
				{
					m_pParent->GetPoint().emplace(sub);
					if (m_pParent->m_bisDivide)
						m_pParent->m_bisDivide = false;
				}
			}

		}
		//m_pParent->m_bisDivide = false;
	}
	else
	{
		//if (m_pChild.size() ==0)return;
		for (auto& childNode : m_pChild)
		{
			childNode->Delete(p);
		}

	}
	//cout << endl;
	//cout << "Delete********************" << endl;
	//PrintQuadTree();
}
bool CQuadTree::IsSameObject(const uShort& p1, const uShort& p2)
{
	if (p1 == p2)return true;
	else return false;
}
void CQuadTree::Sub_Divide()
{
	CBoundary childRect[CHILD_NODE_SIZE];//�뼮
	childRect[0] = CBoundary(m_boundary.GetX() + m_boundary.GetW() / 2, m_boundary.GetZ() - m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[1] = CBoundary(m_boundary.GetX() - m_boundary.GetW() / 2, m_boundary.GetZ() - m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[2] = CBoundary(m_boundary.GetX() + m_boundary.GetW() / 2, m_boundary.GetZ() + m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	childRect[3] = CBoundary(m_boundary.GetX() - m_boundary.GetW() / 2, m_boundary.GetZ() + m_boundary.GetH() / 2,
		m_boundary.GetW() / 2, m_boundary.GetH() / 2);
	for (int i = 0; i < CHILD_NODE_SIZE; i++)
	{
		CQuadTree* temp = new CQuadTree(childRect[i], MAX_PLAYER_IN_NODE);
		temp->SetParent(this);
		m_pChild.push_back(temp);
	}
	for (auto& obj : m_pChild)
	{
		obj->m_iDepth = this->m_iDepth + 1;
	}
	m_bisDivide = true;
	SubDivideToChild();
}
void CQuadTree::SubDivideToChild()
{
	for (auto& player : m_vpPlayers)
	{
		// ���⼭ �θ��ִ� �÷��̾����� ��ǥ�� ���� �ڽ����� �Ѱ��ְ� 
		// �θ����� player������ �����ϴ� �ڵ尡 �־����
		for (auto& childNode : m_pChild)
		{
			if (childNode->m_boundary.contains(Netmgr.GetMediatorMgr()->Find(player)))
			{
				/*m_vpPlayers.erase(player);*/
				childNode->m_vpPlayers.emplace(player);
				break;
			}
		}
	}
	if (m_bisDivide)
		m_vpPlayers.clear();
}




unordered_set<uShort> CQuadTree::search(const CBoundary& range)
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
	CBoundary temp = range;
	if (m_boundary.intersects(temp))
	{
		for (auto& p : m_vpPlayers)
		{
			if (temp.contains(Netmgr.GetMediatorMgr()->Find(p)))
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
	cout << "Search****************************" << endl;
	for (auto& obj : found)
	{
		cout <<"ID : "<< obj << endl;
	}
	cout << "---------------------------" << endl;

	return found;

}

