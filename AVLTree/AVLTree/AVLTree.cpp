#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "/ASD/doctest.h"
#include <iostream>
#include <string>

using namespace std;

template <class Key, class Value>
class AVLTree
{
public:
	AVLTree() {}
	~AVLTree()
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
			throw "Item already exists in AVLTree";
		}
		root_->CalculateHeight();
		Balance(&root_);
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
			if (root_ != nullptr) {
				root_->CalculateHeight();
				Balance(&root_);
			}
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
	size_t GetHeight() {
		if (root_ == nullptr) return 0;
		root_->CalculateHeight();
		return max(root_->left_height, root_->right_height) + 1;
	}

	void Output(ostream& out) {
		out << count_ << endl;
		root_->Output(out);
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

		int left_height = 0;
		int right_height = 0;

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
		void CalculateHeight() {
			left_height = 0;
			right_height = 0;

			if (left != nullptr) {
				left->CalculateHeight();
				left_height = max(left->left_height, left->right_height) + 1;
			}
			if (right != nullptr) {
				right->CalculateHeight();
				right_height = max(right->left_height, right->right_height) + 1;
			}
		}
	};
	Node* root_ = nullptr;
	size_t count_ = 0;

	void Balance(Node** root) {
		if ((*root)->left != nullptr) {
			Balance(&(*root)->left);
		}
		if ((*root)->right != nullptr) {
			Balance(&(*root)->right);
		}

		if (((*root)->right_height - (*root)->left_height) > 1) 
		{
			if ((*root)->right->left_height <= (*root)->right->right_height)
			{
				Node* temp = (*root);
				(*root) = (*root)->right;
				temp->right = (*root)->left;
				(*root)->left = temp;
			}
			else 
			{
				Node* temp = (*root);
				(*root) = (*root)->right->left;
				temp->right->left = (*root)->right;
				(*root)->right = temp->right;
				temp->right = (*root)->left;
				(*root)->left = temp;
			}
		}
		else if (((*root)->left_height - (*root)->right_height) > 1)
		{
			if ((*root)->left->right_height <= (*root)->left->left_height)
			{
				Node* temp = (*root);
				(*root) = (*root)->left;
				temp->left = (*root)->right;
				(*root)->right = temp;
			}
			else
			{
				Node* temp = (*root);
				(*root) = (*root)->left->right;
				temp->left->right = (*root)->left;
				(*root)->left = temp->left;
				temp->left = (*root)->right;
				(*root)->right = temp;
			}
		}
	}

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

TEST_CASE("Testing AVLTree class") {
	AVLTree<string, string> AVLTree;

	REQUIRE(AVLTree.GetCount() == 0);

	size_t n = 10;
	for (size_t i = 0; i < n; i++)
	{
		AVLTree.Add(to_string(i) + "k", to_string(i) + "v");
	}

	REQUIRE(AVLTree.GetCount() == n);

	SUBCASE("Testing Add") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK((*(AVLTree.Find(to_string(i) + "k"))) == to_string(i) + "v");
			CHECK(AVLTree.GetHeight() <= (log2(AVLTree.GetCount() + 2) * 1.4404 - 0.328));
		}
		AVLTree.Output(cout);
	}
	SUBCASE("Testing Find") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK(AVLTree.Find(to_string(i) + "key that doesnt exist") == nullptr);
		}
	}
	SUBCASE("Testing Replace") {
		for (size_t i = 0; i < n; i++)
		{
			CHECK(AVLTree.Replace(to_string(i) + "k", to_string(i) + "nv") == true);
			CHECK(*(AVLTree.Find(to_string(i) + "k")) == (to_string(i) + "nv"));
		}
		for (size_t i = 0; i < n; i++)
		{
			CHECK(AVLTree.Replace(to_string(i) + "nk", to_string(i) + "nnv") == false);
		}
	}
	SUBCASE("Testing Remove") {
		for (size_t i = 0; i < n; i++)
		{
			AVLTree.Remove(to_string(i) + "k");
			CHECK(AVLTree.Find(to_string(i) + "k") == nullptr);
			CHECK(AVLTree.GetCount() == n - i - 1);
			CHECK(AVLTree.GetHeight() <= (log2(AVLTree.GetCount() + 2) * 1.4404 - 0.328));
		}
	}
	SUBCASE("Testing Clear") {
		AVLTree.Clear();
		CHECK(AVLTree.GetCount() == 0);
	}
}