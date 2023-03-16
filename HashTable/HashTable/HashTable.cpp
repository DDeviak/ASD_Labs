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
	}

	void Add(const Key& k, const Value& v) {
		Node** temp = FindNode(k, &root_);
		if ((*temp) == nullptr)
		{
			(*temp) = new Node(k, v);
			count_++;
		}
		else
		{
			throw "Item already exists in Table";
		}
	}
	Value* Find(const Key& k) {
		Node** temp = FindNode(k, &root_);
		if (*temp != nullptr) return (*temp)->value;
		else return nullptr;
	}
	bool Replace(const Key& k, const Value& v) {
		Node** temp = FindNode(k, &root_);
		if ((*temp) != nullptr)
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
	void Remove(const Key& k) {
		Node** node_to_remove = FindNode(k, &root_);
		if ((*node_to_remove) != nullptr)
		{
			if ((*node_to_remove)->left != nullptr) {
				Node** current = &(*node_to_remove)->left;
				while ((*current)->right != nullptr)
				{
					current = &(*current)->right;
				}
				Node* temp = *current;

				*current = (*current)->left;

				temp->left = (*node_to_remove)->left;
				temp->right = (*node_to_remove)->right;

				delete (*node_to_remove);

				*node_to_remove = temp;
			}
			else if ((*node_to_remove)->right != nullptr)
			{
				Node** current = &(*node_to_remove)->right;
				while ((*current)->left != nullptr)
				{
					current = &(*current)->left;
				}
				Node* temp = *current;

				*current = (*current)->right;

				temp->left = (*node_to_remove)->left;
				temp->right = (*node_to_remove)->right;

				delete (*node_to_remove);

				*node_to_remove = temp;
			}
			else
			{
				delete (*node_to_remove);
				(*node_to_remove) = nullptr;
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
		//out << count_ << endl;
		if (root_ != nullptr) root_->Output(out);
	}

private:
	struct Node
	{
		Node(Key k, Value v) : key(k) {
			value = new Value(v);
		}
		~Node() {
			if (value != nullptr) {
				delete value;
				value = nullptr;
			}
		}
		Key key;
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
		void Output(ostream& out) {
			if (left != nullptr) {
				left->Output(out);
			}
			out << key << " " << *value << endl;
			if (right != nullptr) {
				right->Output(out);
			}
		}
	};
	Node* root_ = nullptr;
	size_t count_ = 0;

	Node** FindNode(const Key& key, Node** root) {
		if (*root == nullptr || (*root)->key == key) {
			return root;
		}

		if ((*root)->key > key)
		{
			return FindNode(key, &(*root)->left);
		}
		else if ((*root)->key < key)
		{
			return FindNode(key, &(*root)->right);
		}
	}
};

template <class Key, class Value>
class HashTable
{
public:
	HashTable(size_t possible_size = 10) : buckets_count_(possible_size)
	{
		buckets = new Table<Key, Value>*[buckets_count_]();
		for (size_t i = 0; i < buckets_count_; i++)
		{
			buckets[i] = new Table<Key, Value>();
		}
	}
	~HashTable()
	{
		for (size_t i = 0; i < buckets_count_; i++)
		{
			buckets[i]->Clear();
			delete buckets[i];
		}
		delete buckets;
		count_ = 0;
	}

	void Add(const Key &k, const Value &v) {
		buckets[Hash(k)]->Add(k, v);
		count_++;
	}
	Value* Find(const Key& k) {
		return buckets[Hash(k)]->Find(k);
	}
	bool Replace(const Key& k, const Value& v) {
		return buckets[Hash(k)]->Replace(k, v);
	}
	void Remove(const Key &k) {
		buckets[Hash(k)]->Remove(k);
		count_--;
	}
	void Clear() {
		for (size_t i = 0; i < buckets_count_; i++)
		{
			buckets[i]->Clear();
		}
		count_ = 0;
	}
	size_t GetCount() {
		return count_;
	}

	void Output(ostream& out) {
		out << count_ << endl;
		for (size_t i = 0; i < buckets_count_; i++)
		{
			buckets[i]->Output(out);
		}
	}

private:
	size_t buckets_count_ = 0;
	size_t count_ = 0;
	Table<Key, Value>** buckets = nullptr;

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
		return (rand() - rand()) % buckets_count_;
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