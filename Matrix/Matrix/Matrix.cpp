#include <iostream>
#include <list>

using namespace std;

class Matrix
{
public:
	Matrix() {}
	~Matrix() {}

private:
	size_t n_, m_;
	list<list<pair<size_t, int>>> rows_;
};


int main()
{
    
}