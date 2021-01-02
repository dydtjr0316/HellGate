#include <iostream>

using namespace std;

void youngmun();

int main()
{
#ifdef _DEBUG
	cout << "Debug" << endl;
#endif
	cout << "¾È µÇ³×" << endl;
}

void youngmun()
{
	cout << "youngmun" << endl;
}