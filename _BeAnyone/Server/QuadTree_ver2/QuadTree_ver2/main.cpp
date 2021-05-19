#include "stdafx.h"
#include <vector>

class Point {
public:
	Point() {}
	Point(const float& x, const float& y)
	{
		this->x = x;
		this->y = y;
	}
	void setX(const float& x) { this->x = x; }
	void setY(const float& y) { this->y = y; }
	float getX() { return x; }
	float getY() { return y; }
private:
	float x, y;
};

class Rectanlge
{
public:
	Rectanlge() {}
	Rectanlge(const float& x, const float& y, const float& w, const float& h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
	bool contains(Point& p)
	{
		return (p.getX() >= x - w &&
			p.getX() <= x + w &&
			p.getY() >= y - h &&
			p.getY() <= y + h);
	}
	bool intersects(Rectanlge& range)
	{
		return !(range.getX() - range.getW() > x + w ||
			range.getX() + range.getW() < x - w ||
			range.getY() - range.getH() > y + h ||
			range.getY() + range.getH() < y - h);
	}
public:
	float getX() { return x; }
	float getY() { return y; }
	float getW() { return w; }
	float getH() { return h; }
private:
	float x, y, w, h;
};

class QuadTRee {
public:
	QuadTRee(const Rectanlge& _boundary, const int& n) {
		boundary = _boundary; capacity = n;
	}
	bool insert(Point& p)
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

			if(nw->insert(p))return true;
			else if(ne->insert(p))return true;
			else if(sw->insert(p))return true;
			else if(se->insert(p))return true;

		}
	}
	void subdivide()
	{
		Rectanlge nwr (boundary.getX() + boundary.getW() / 2, boundary.getY() - boundary.getH() / 2, boundary.getW()/2, boundary.getH()/2);
		Rectanlge ner (boundary.getX() - boundary.getW() / 2, boundary.getY() - boundary.getH() / 2, boundary.getW()/2, boundary.getH()/2);
		Rectanlge swr (boundary.getX() + boundary.getW() / 2, boundary.getY() + boundary.getH() / 2, boundary.getW()/2, boundary.getH()/2);
		Rectanlge ser (boundary.getX() - boundary.getW() / 2, boundary.getY() + boundary.getH() / 2, boundary.getW()/2, boundary.getH()/2);
		nw = new QuadTRee(nwr, 4);
		ne = new QuadTRee(ner, 4);
		sw = new QuadTRee(swr, 4);
		se = new QuadTRee(ser, 4);
		divide = true;
	}
	vector<Point> search(Rectanlge& range)	// 인자로 vector<Point> found 넣는 방식 생각해보기
	{
		vector<Point> found;
		if (!boundary.intersects(range))
			return ;
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
	QuadTRee* getnw() { return nw; }
	QuadTRee* getne() { return ne; }
	QuadTRee* getsw() { return sw; }
	QuadTRee* getse() { return se; }
	vector<Point> getPoint() { return points; }
private:
	Rectanlge boundary;
	int capacity;
	vector<Point> points;
	QuadTRee* nw;
	QuadTRee* ne;
	QuadTRee* sw;
	QuadTRee* se;
	bool divide = false;
};

int main()
{
	QuadTRee* qt = new QuadTRee(Rectanlge(3200.f, 3200.f, 3200.f, 3200.f), 4);
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 1000; i++)
	{
		Point p((float)(rand() % 6400), (float)(rand() % 6400));
		qt->insert(p);
	}


	cout << "----------insert-------------" << endl;
	cout << "qt" << endl << endl;
	if (qt->getPoint().size() != 0)
		for (auto& obj : qt->getPoint())
		{
			cout << obj.getX() << ", " << obj.getY() << endl;
		}


	cout << "nw" << endl << endl;
	if (qt->getnw() != nullptr)
		if (qt->getnw()->getPoint().size() != 0)
			for (auto& obj : qt->getnw()->getPoint())
			{
				cout << obj.getX() << ", " << obj.getY() << endl;
			}

	cout << "ne" << endl << endl;
	if (qt->getne() != nullptr)
		if (qt->getne()->getPoint().size() != 0)
			for (auto& obj : qt->getne()->getPoint())
			{
				cout << obj.getX() << ", " << obj.getY() << endl;
			}

	cout << "sw" << endl << endl;
	if (qt->getsw() != nullptr)
		if (qt->getsw()->getPoint().size() != 0)
			for (auto& obj : qt->getsw()->getPoint())
			{
				cout << obj.getX() << ", " << obj.getY() << endl;
			}

	cout << "se" << endl << endl;
	if (qt->getse() != nullptr)
		if (qt->getse()->getPoint().size() != 0)
			for (auto& obj : qt->getse()->getPoint())
			{
				cout << obj.getX() << ", " << obj.getY() << endl;
			}


	cout << "----------search-------------" << endl;
	Rectanlge look(1000.f, 1000.f, 400.f, 400.f);
	vector<Point> points = qt->search(look);

	for (auto& obj : points)
	{
		cout << obj.getX() << ", " << obj.getY() << endl;
	}
}