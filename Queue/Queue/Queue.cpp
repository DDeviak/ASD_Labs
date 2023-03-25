#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
using namespace std;

template <class T>
class Queue
{
public:
	Queue() {
		head_ = new Item();
		tail_ = head_;
	}
	~Queue() {
		Clear();
		delete tail_;
	}

	void Enqueue(T val) {
		tail_->value = val;
		tail_->next = new Item();
		tail_ = tail_->next;
		size_++;
	}

	bool Dequeue() {
		if (is_empty())return false;

		Item* temp = head_;
		head_ = head_->next;
		delete temp;
		size_--;
		return true;
	}

	T Head() {
		if (IsEmpty()) {
			throw underflow_error("Unable to get head of the empty queue");
		}
		return head_->value;
	}

	size_t GetSize() {
		return size_;
	}

	bool IsEmpty() {
		return size_ == 0;
	}

	void Clear() {
		while (Dequeue()) {}
	}

private:
	struct Item
	{
		T value;
		Item* next = nullptr;
	};

	Item* head_ = nullptr;
	Item* tail_ = nullptr;
	size_t size_ = 0;
};

template <class Value>
class PriorityQueue
{
public:
	PriorityQueue() {}
	~PriorityQueue() {
		Clear();
	}
	void Add(const Value& v) {
		Node** temp = FindNode(v, &root_);
		if ((*temp) == nullptr)
		{
			(*temp) = new Node(v);
			count_++;
		}
		else
		{
			throw "Item already exists in Table";
		}
	}
	void Remove(const Value& v) {
		Node** node_to_remove = FindNode(v, &root_);
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

TEST_CASE("Testing Queue") {

}