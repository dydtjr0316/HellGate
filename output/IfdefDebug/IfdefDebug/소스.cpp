#include <iostream>

using namespace std;

void hyorim();

int main()
{
#ifdef _DEBUG
	cout << "Debug" << endl;
	hyorim();
#endif
	cout << "�� �ǳ�" << endl;
}

void hyorim()
{
	cout << "hyorim" << endl;
}