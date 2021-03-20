#include "stdafx.h"
#include "CQuadTree.h"
#include "CQuad.h"
#include "CPlayer.h"
#include"CRect.h"

CQuadTree::CQuadTree()
{
	m_pRoot = new CQuad(0, 0, WINDOW::WIDTH, WINDOW::HEIGHT);
}

void CQuadTree::update(CQuad* root)
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
					root->GetInfo()[it.first] = it.second; // Player class ���� �ۼ� �� ���� 
					root->GetInfo()[it.first]->SetParent(root);
				}
			}
			root->GetChildern().clear();
		}
	}

	// �̺κ� ���� �Ҷ� ��� ���� �ٽ� �����غ���
	{
		// ERASING CODE HERE

		/*for (auto it = root->people.begin(); it != root->people.end(); it++) {
			if (it.value().update()) {
				queue.push_back(&it.value());
			}
		}*/
	}	

	for (int i = 0; i < root->GetChildern().size(); ++i) { update(&root->GetChildern()[i]); }
}

void CQuadTree::insert(int& x,int& y, CQuad* root, const bool dead)
{
	CQuad* quad = nullptr;
	try
	{
		quad = search(x, y, root);
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		system("pause");
		return;
	}

	// ��� rect ������ ����ü�� �������� �ѹ� �غ���
	// ����� �� �����ִ� ����
	
	// �̰� �ΰ������� �ؾߵ� �ϴ� �հ��´��
	CRect& temp = quad->GetRect();
	tRect dest;
	dest.set(temp.GetX(), temp.GetY(), temp.GetW(), temp.GetH());

	if (quad->GetIteration() > MAX_ITER)
	{
		quad = nullptr;
	}

	if (quad->GetInfo().size() >= 4/*�ѱ����� ���� ĳ���� ���� �ٽü����ؾ���*/)
	{
		if (quad->GetChildern().size() == 0)
		{

		}
	}

}

CQuad* CQuadTree::search(int& x, int& y, CQuad* root)
{
	
	if (!root->GetRect().CollidePoint(x, y))
	{
		// ���ϴ� �ڵ��ΰ�,,,?
		std::string tmp = "Out of range: ";
		std::stringstream buffer; buffer << x;
		tmp += buffer.str() + " ";
		buffer.str("");
		buffer << y;
		tmp += buffer.str() + "  SDL_Rect {";
		buffer.str("");
		buffer << root->GetRect().GetX();
		tmp += buffer.str() + ", ";
		buffer.str("");
		buffer << root->GetRect().GetY();
		tmp += buffer.str() + ", ";
		buffer.str("");
		buffer << root->GetRect().GetW();
		tmp += buffer.str() + ", ";
		buffer.str("");
		buffer << root->GetRect().GetH();
		tmp += buffer.str() + "}";
		buffer.str("");
		throw std::exception(tmp.c_str());
		return root;
	}

	
	for (int i = 0; i < root->GetChildern().size(); i++)
	{
		if (root->GetChildern()[i].GetRect().CollidePoint(x, y))
			return search(x, y, &root->GetChildern()[i]);
	}
	return nullptr;
}
