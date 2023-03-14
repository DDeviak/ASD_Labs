#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

class MathTree
{
public:
	MathTree() {}
	MathTree(string exp) 
	{
		root_ = new Node(exp);
	}
	~MathTree() {}

	void Output(ostream &out) {
		root_->Output(out);
	}
	double Calculate() {
		root_->Calculate();
		return root_->value;
	}

private:
	struct Node
	{
		char operation = 0;
		double value;
		Node* left = nullptr;
		Node* right = nullptr;

		Node() :operation(0) {}
		Node(string exp) {
			size_t i, b = 0;
			size_t l = exp.length();
			if (exp[0] == '(')
			{
				for (i = 0; i < l; i++)
				{
					if (exp[i] == '(') b++;
					if (exp[i] == ')') b--;
					if (b == 0)
					{
						i++;
						break;
					}
				}
			}
			else 
			{
				i = 0;
				while ((exp[i] <= '9' && exp[i] >= '0') || exp[i] == '.')
				{
					i++;
					if (i == l) break;
				}
			}

			if (i == l) 
			{
				stringstream ss;
				ss << exp;
				ss >> value;
			}
			else {
				operation = exp[i];
				left = new Node(exp.substr(exp[0] == '(', i - (exp[0] == '(')*2));
				right = new Node(exp.substr(i + 1 + (exp[i + 1] == '('), l - i - 1 - (exp[l - 1] == ')')));
			}
		}

		void Output(ostream& out) {
			if (left != nullptr && right != nullptr) {
				left->Output(out);
				right->Output(out);
				out << operation;
			}
			else
			{
				out << value;
			}
			out << " ";
		}
		void Calculate() {
			if (left != nullptr && right != nullptr) {
				left->Calculate();
				right->Calculate();
			}
			switch (operation)
			{
			case '+':
				value = left->value + right->value;
				break;
			case '-':
				value = left->value - right->value;
				break;
			case '*':
				value = left->value * right->value;
				break;
			case '/':
				value = left->value / right->value;
				break;
			case '^':
				value = pow(left->value, right->value);
				break;
			default:
				break;
			}
		}
	};

	Node* root_ = nullptr;

};

int main()
{
	string s;
	cin >> s;
	MathTree mt(s);
	mt.Output(cout);
	cout << " = " << mt.Calculate();
}