#include <iostream>

using namespace std;

class MathTree
{
public:
	MathTree() {}
	~MathTree() {}

private:
	enum Operation
	{
		Sum,
		Sub,
		Mult,
		Div,
		Pow
	};
	struct Node
	{
		Operation operation;
		double value;
		Node* left = nullptr;
		Node* right = nullptr;
	};
};

int main()
{
    std::cout << "Hello World!\n";
}