#include "stdafx.h"
#include "CQuadTree.h"
#include "CQuad.h"
#include "CPlayer.h"

CQuadTree::CQuadTree()
{
	m_pRoot = new CQuad(0, 0, WINDOW::WIDTH, WINDOW::HEIGHT);
}

void CQuadTree::update(const CQuad* root)
{
	bool flag = true;
	if (root->GetChildern().size() != 0)							// ��忡 �ڽ��� �����Ѵٸ�
	{
		for (int i = 0; i < root->GetChildern().size(); ++i)		// �ڽļ� ��ŭ 
		{
			if (root->GetChildern()[i].GetChildern().size() != 0)	// �ڽ��� �ڽ� ��忡 ��尡 ���� �Ѵٸ�
			{
				flag = false;
				break;
			}
		}
	}

	if (flag)	//������ flag ���� �ڽĳ�尡 �ڽ��� ������ ���� �ʴٸ�
	{
		int sum = 0;
		for (int i = 0; i < root->GetChildern().size(); ++i)
		{
			sum += root->GetChildern()[i].GetInfo().size();
			// �ڽĳ�忡 �÷��̾ ������� ����ؼ� ���ϱ�
		}

		if (sum < 4/*���� 4�� ����ϳ��� �� �÷��̾� ���� ����*/)
		{
			for (int i = 0; i < root->GetChildern().size(); ++i)
			{
				for (auto it : root->GetChildern()[i].GetInfo())
				{
					root->GetInfo()[it.first] = it.second;
					// Player class ���� �ۼ� �� ���� 

				}
			}
		}
	}



}
