#include "stdafx.h"
#include "CQuadTree.h"
#include "CPlayer.h"
#include "CRectangle.h"
uShort id = 0;
void Enter() { cout << endl; }
void Print(CQuadTree* qt);
int main()
{
	CQuadTree* qt = new CQuadTree(CRectangle(3200.f, 3200.f, 3200.f, 3200.f), 4);
	srand((unsigned int)time(NULL));
	cout << "----------insert start-------------" << endl;
	for (int i = 0; i < 10; i++)
	{
		CPlayer *p = new CPlayer(id, (float)(rand() % 6400), (float)(rand() % 6400));
		id++;
		cout << p->GetX() << ", " << p->GetZ() << endl;
		qt->insert(p);
	}
	cout << "----------insert end-------------" << endl;

	Print(qt);
	for (auto& obj : qt->Getne()->GetPoint())
	{
		obj->SetX(obj->GetX() + 500.f);
	}
	


	Enter();
	cout << "----------search-------------" << endl;
	CRectangle look(1000.f, 1000.f, 400.f, 400.f);
	vector<CPlayer*> points = qt->search(look);

	for (auto& obj : points)
	{
		cout << obj->GetX() << ", " << obj->GetZ() << endl;
	}
}

void Print(CQuadTree* qt)
{
	cout << "----------Root의 데이터-------------" << endl;
	if (qt->GetPoint().size() != 0)
		for (auto& obj : qt->GetPoint())
		{
			cout << obj->GetX() << ", " << obj->GetZ() << endl;
		}
	cout << "-----------------------------------" << endl;

	Enter();
	if (qt->Getnw() != nullptr)
		if (qt->Getnw()->GetPoint().size() != 0)
			for (auto& obj : qt->Getnw()->GetPoint())
			{
				cout << "nw  ->" << obj->GetX() << ", " << obj->GetZ() << endl;
			}

	Enter();
	if (qt->Getne() != nullptr)
		if (qt->Getne()->GetPoint().size() != 0)
			for (auto& obj : qt->Getne()->GetPoint())
			{
				cout << "ne  ->" << obj->GetX() << ", " << obj->GetZ() << endl;
			}

	Enter();
	if (qt->Getsw() != nullptr)
		if (qt->Getsw()->GetPoint().size() != 0)
			for (auto& obj : qt->Getsw()->GetPoint())
			{
				cout << "sw  ->" << obj->GetX() << ", " << obj->GetZ() << endl;
			}

	Enter();
	if (qt->Getse() != nullptr)
		if (qt->Getse()->GetPoint().size() != 0)
			for (auto& obj : qt->Getse()->GetPoint())
			{
				cout << "se  ->" << obj->GetX() << ", " << obj->GetZ() << endl;
			}

}
