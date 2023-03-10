#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <stack>

using namespace std;

template <class T>
class Stack
{
public:
	Stack() {}
	~Stack() {
		clear();
	}

	void push(T val) {
		Item* temp = new Item();
		temp->value = val;
		temp->previous = top_;
		top_ = temp;
		size_++;
	}

	bool pop() {
		if (is_empty())return false;

		Item* temp = top_;
		top_ = top_->previous;
		delete temp;
		size_--;
		return true;
	}

	T top() {
		if (is_empty()) {
			throw underflow_error("Unable to get top of the empty stack");
		}
		return top_->value;
	}

	size_t size() {
		return size_;
	}

	bool is_empty() {
		return size_ == 0;
	}

	void clear() {
		while (pop()) {}
	}

private:
	struct Item
	{
		T value;
		Item* previous;
	};
	Item* top_ = nullptr;
	size_t size_ = 0;
};

TEST_CASE("Testing my Stack class") {
	Stack<int> my_stack;

	REQUIRE(my_stack.size() == 0);
	REQUIRE(my_stack.is_empty() == true);
	
	for (size_t i = 1; i < 11; i++)
	{
		my_stack.push(i);
	}

	SUBCASE("Testing push") {
		CHECK(my_stack.size() == 10);
		CHECK(my_stack.is_empty() == false);
	}
	
	SUBCASE("Testing pop") {
		for (size_t i = 10; i > 0; i--)
		{
			CHECK(my_stack.top() == i);
			CHECK(my_stack.pop() == true);
		}

		CHECK(my_stack.size() == 0);
		CHECK(my_stack.is_empty() == true);
	}

	SUBCASE("Testing clear") {
		my_stack.clear();

		CHECK(my_stack.size() == 0);
		CHECK(my_stack.is_empty() == true);
	}

	SUBCASE("Testing top") {
		my_stack.clear();

		CHECK_THROWS_AS(my_stack.top(), underflow_error);
		CHECK(my_stack.pop() == false);
		CHECK_THROWS_AS(my_stack.top(), underflow_error);
	}
}

TEST_CASE("Testing STL Stack class") {
	stack<int> STL_stack;

	REQUIRE(STL_stack.size() == 0);
	REQUIRE(STL_stack.empty() == true);

	for (size_t i = 1; i < 11; i++)
	{
		STL_stack.push(i);
	}

	SUBCASE("Testing push") {
		CHECK(STL_stack.size() == 10);
		CHECK(STL_stack.empty() == false);
	}

	SUBCASE("Testing pop and top") {
		for (size_t i = 10; i > 0; i--)
		{
			CHECK(STL_stack.top() == i);
			CHECK_NOTHROW(STL_stack.pop());
		}

		CHECK(STL_stack.size() == 0);
		CHECK(STL_stack.empty() == true);
	}
}