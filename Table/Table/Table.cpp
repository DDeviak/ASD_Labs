#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <string>

using namespace std;

template <class Key, class Value>
class Table
{
public:
	Table() {}
	~Table()
	{
		Clear();
		delete first_;
	}

	void Add(Key k, Value v) {
		Row* current = first_;
		while (current->next != nullptr)
		{
			current = current->next;
		}
		current->next = new Row();
		current->key = k;
		current->value = v;
		count_++;
	}
	Value* Find(Key k) {
		Row* current = first_;
		while (current->next != nullptr)
		{
			if (current->key == k)return &(current->value);
			current = current->next;
		}
		return nullptr;
	}
	bool Replace(Key k, Value v) {
		Row* current = first_;
		while (current->next != nullptr)
		{
			if (current->key == k) {
				current->value = v;
				return true;
			}
			current = current->next;
		}
		return false;
	}
	void Remove(Key k) {
		if (first_->key == k)
		{
			Row* temp = first_;
			first_ = first_->next;
			delete temp;
			count_--;
			return;
		}

		Row* previous = first_;
		Row* current = previous->next;
		while (current->next != nullptr)
		{
			if (current->key == k)
			{
				previous->next = current->next;
				delete current;
				count_--;
				return;
			}
			previous = current;
			current = previous->next;
		}
	}
	void Clear() {
		Row* temp;
		while (first_->next != nullptr)
		{
			temp = first_;
			first_ = first_->next;
			delete temp;
		}
		count_ = 0;
	}
	size_t GetCount() {
		return count_;
	}

	void Output(ostream& out) {
		Row* current = first_;
		while (current->next != nullptr)
		{
			out << current->key << " " << current->value << endl;
			current = current->next;
		}
	}

private:
	struct Row
	{
		Key key;
		Value value;

		Row* next = nullptr;
	};

	Row* first_ = new Row();

	size_t count_ = 0;
};


TEST_CASE("Testing Table class") {
	Table<string, string> table;

	REQUIRE(table.GetCount() == 0);

	size_t n = 10;
	for (size_t i = 0; i < n; i++)
	{
		table.Add(to_string(i) + "k", to_string(i) + "v");
	}

	REQUIRE(table.GetCount() == n);

	SUBCASE("Testing Add") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK((*(table.Find(to_string(i) + "k"))) == to_string(i) + "v");
		}
		table.Output(cout);
	}
	SUBCASE("Testing Find") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK(table.Find(to_string(i) + "key that doesnt exist") == nullptr);
		}
	}
	SUBCASE("Testing Replace") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK(table.Replace(to_string(i) + "k", to_string(i) + "nv") == true);
			CHECK(*(table.Find(to_string(i) + "k")) == (to_string(i) + "nv"));
		}
		for (size_t i = 0; i < n; i++)
		{
			CHECK(table.Replace(to_string(i) + "nk", to_string(i) + "nnv") == false);
		}
	}
	SUBCASE("Testing Remove") {
		for (size_t i = 0; i < n; i++)
		{
			table.Remove(to_string(i) + "k");
			CHECK(table.Find(to_string(i) + "k") == nullptr);
			CHECK(table.GetCount() == n - i - 1);
		}
	}
	SUBCASE("Testing Clear") {
		table.Clear();
		CHECK(table.GetCount() == 0);
	}
}