#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <string>

using namespace std;

template <class Key, class Value>
class HashTable
{
public:
	HashTable() {}
	~HashTable()
	{
		Clear();
	}

	void Add(const Key &k, const Value &v) {
		int h = Hash(k);
		Node** temp = FindNode(h, &root_);
		if ((*temp) == nullptr) 
		{
			(*temp) = new Node(h, v);
			count_++;
		}
		else
		{
			if ((*temp)->value == nullptr) {
				(*temp)->value = new Value(v);
				count_++;
			}
			else 
			{
				throw "Item already exists in Table";
			}
		}
	}
	Value* Find(const Key& k) {
		Node** temp = FindNode(Hash(k), &root_);
		if (*temp != nullptr) return (*temp)->value;
		else return nullptr;
	}
	bool Replace(const Key& k, const Value& v) {
		Node** temp = FindNode(Hash(k), &root_);
		if ((*temp) != nullptr && (*temp)->value != nullptr)
		{
			delete (*temp)->value;
			(*temp)->value = new Value(v);
			return true;
		}
		else
		{
			return false;
		}
	}
	void Remove(const Key &k) {
		Node** temp = FindNode(Hash(k), &root_);
		if ((*temp) != nullptr)
		{
			if ((*temp)->left != nullptr || (*temp)->right != nullptr) {
				delete (*temp)->value;
				(*temp)->value = nullptr;
			}
			else
			{
				delete (*temp);
				(*temp) = nullptr;
			}
			count_--;
		}
	}
	void Clear() {
		if (root_ != nullptr) root_->Clear();
		root_ = nullptr;
		count_ = 0;
	}
	size_t GetCount() {
		return count_;
	}

	void Output(ostream& out) {
		out << count_ << endl;
		root_->Output(out);
	}

private:
	struct Node
	{
		Node(int h,Value v) : hash(h) {
			value = new Value(v);
		}
		~Node() {
			if (value != nullptr) {
				delete value;
				value = nullptr;
			}
		}
		int hash;
		Value* value = nullptr;

		Node* left = nullptr;
		Node* right = nullptr;

		void Clear() {
			if (left != nullptr) {
				left->Clear();
			}
			if (right != nullptr) {
				right->Clear();
			}
			delete this;
		}
		void Output(ostream &out) {
			if (left != nullptr) {
				left->Output(out);
			}
			out << hash << " " << *value << endl;
			if (right != nullptr) {
				right->Output(out);
			}
		}
	};
	Node* root_ = nullptr;
	size_t count_ = 0;

	int Hash(const Key& key) {
		stringstream ss;
		ss << key;
		string s = ss.str();
		int t = 0;
		for (size_t i = 0; i < s.size(); i++)
		{
			t += s[i] * (i + 1);
		}
		srand(t);
		return rand()-rand();
	}

	Node** FindNode(const int &hash, Node** root) {
		if (*root == nullptr || (*root)->hash == hash) {
			return root;
		}
		
		if ((*root)->hash > hash) 
		{
			return FindNode(hash, &(*root)->left);
		}
		else if ((*root)->hash < hash)
		{
			return FindNode(hash, &(*root)->right);
		}
	}
};


TEST_CASE("Testing HashTable class") {
	HashTable<string, string> HashTable;

	REQUIRE(HashTable.GetCount() == 0);

	size_t n = 10;
	for (size_t i = 0; i < n; i++)
	{
		HashTable.Add(to_string(i) + "k", to_string(i) + "v");
	}

	REQUIRE(HashTable.GetCount() == n);

	SUBCASE("Testing Add") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK((*(HashTable.Find(to_string(i) + "k"))) == to_string(i) + "v");
		}
		HashTable.Output(cout);
	}
	SUBCASE("Testing Find") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK(HashTable.Find(to_string(i) + "key that doesnt exist") == nullptr);
		}
	}
	SUBCASE("Testing Replace") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK(HashTable.Replace(to_string(i) + "k", to_string(i) + "nv") == true);
			CHECK(*(HashTable.Find(to_string(i) + "k")) == (to_string(i) + "nv"));
		}
		for (size_t i = 0; i < n; i++)
		{
			CHECK(HashTable.Replace(to_string(i) + "nk", to_string(i) + "nnv") == false);
		}
	}
	SUBCASE("Testing Remove") {
		for (size_t i = 0; i < n; i++)
		{
			HashTable.Remove(to_string(i) + "k");
			CHECK(HashTable.Find(to_string(i) + "k") == nullptr);
			CHECK(HashTable.GetCount() == n - i - 1);
		}
	}
	SUBCASE("Testing Clear") {
		HashTable.Clear();
		CHECK(HashTable.GetCount() == 0);
	}
}