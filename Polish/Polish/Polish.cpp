#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <stack>

using namespace std;

size_t IsOperation(char c) {
	switch (c)
	{
	case '+':
		return 1;
		break;
	case '-':
		return 2;
		break;
	case '*':
		return 3;
		break;
	case '/':
		return 4;
		break;
	case '^':
		return 5;
		break;
	default:
		return 0;
		break;
	}
}
string ToPolish(string exp) {
	stack<char> stack;
	string output;
	for (size_t i = 0; i < exp.size(); i++)
	{
		if (exp[i] == ' ')continue;
		if (exp[i] == '(')stack.push(exp[i]);
		else if (exp[i] == ')') 
		{
			while (stack.top()!='(')
			{
				output += stack.top();
				output += " ";
				stack.pop();
			}
			stack.pop();
		}
		else if (IsOperation(exp[i])) {
			while (!stack.empty() && (IsOperation(exp[i]) < IsOperation(stack.top())))
			{
				output += stack.top();
				output += " ";
				stack.pop();
			}
			stack.push(exp[i]);
		}
		else
		{
			output += exp[i];
			if (!((exp[i + 1] <= '9' && exp[i + 1] >= '0') || exp[i + 1] == '.'))
				output += " ";
		}
	}
	while (!stack.empty())
	{
		if (stack.top() != '(') {
			output += stack.top();
			output += " ";
		}
		stack.pop();
	}
	output.pop_back();
	return output;
}
double Calculate(string exp) {
	stack<double> stack;
	string t;
	for (size_t i = 0; i < exp.size(); i++)
	{
		if (exp[i] == ' ') {
			if (t != "") {
				stack.push(stod(t));
				t = "";
			}
			continue;
		}
		else if (IsOperation(exp[i]))
		{
			double a, b;
			b = stack.top();
			stack.pop();
			a = stack.top();
			stack.pop();
			switch (exp[i])
			{
			case '+':
				stack.push(a + b);
				break;
			case '-':
				stack.push(a - b);
				break;
			case '*':
				stack.push(a * b);
				break;
			case '/':
				stack.push(a / b);
				break;
			case '^':
				stack.push(pow(a, b));
				break;
			default:
				throw "Invalid operation";
				break;
			}
		}
		else t += exp[i];
	}
	return stack.top();
}

TEST_CASE("Testing Polish notation") {
	
	CHECK(ToPolish("2.2 + 3 * 4") == "2.2 3 4 * +");
	CHECK(Calculate("2.2 3 4 * +") == 14.2);

	CHECK(ToPolish("(2 + 3) * 4") == "2 3 + 4 *");
	CHECK(Calculate("2 3 + 4 *") == 20);

	CHECK(ToPolish("2 ^ 3 + 4") == "2 3 ^ 4 +");
	CHECK(Calculate("2 3 ^ 4 +") == 12);

	CHECK(ToPolish("2 + 3 * 4 - 5") == "2 3 4 * 5 - +");
	CHECK(Calculate("2 3 4 * 5 - +") == 9);

	CHECK(ToPolish("3 * (4 + 5) - (6 - 2) * 4") == "3 4 5 + * 6 2 - 4 * -");
	CHECK(Calculate("3 4 5 + * 6 2 - 4 * -") == 11);
}