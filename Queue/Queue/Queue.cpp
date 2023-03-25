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
		if (IsEmpty())return false;

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

template <class T>
class PriorityQueue
{
public:
	PriorityQueue() {}
	~PriorityQueue() {
		Clear();
	}

	T PeekMin() {
		if (IsEmpty()) {
			throw underflow_error("Unable to get min of the empty queue");
		}
		Node* current = root_;
		while (current->left != nullptr)
		{
			current = current->left;
		}
		return current->value;
	}
	bool PopMin() {
		if (IsEmpty()) {
			return false;
		}
		Node** current = &root_;
		while ((*current)->left != nullptr)
		{
			current = &(*current)->left;
		}
		Remove(current);
		return true;
	}

	T PeekMax() {
		if (IsEmpty()) {
			throw underflow_error("Unable to get min of the empty queue");
		}
		Node* current = root_;
		while (current->right != nullptr)
		{
			current = current->right;
		}
		return current->value;
	}
	bool PopMax() {
		if (IsEmpty()) {
			return false;
		}
		Node** current = &root_;
		while ((*current)->right != nullptr)
		{
			current = &(*current)->right;
		}
		Remove(current);
		return true;
	}

	void Enqueue(const T& v) {
		Node** temp = FindNode(v, &root_);
		
		(*temp) = new Node(v);
		count_++;
	}
	
	void Clear() {
		if (root_ != nullptr) root_->Clear();
		root_ = nullptr;
		count_ = 0;
	}

	size_t GetCount() {
		return count_;
	}

	bool IsEmpty() {
		return count_ == 0;
	}

private:
	struct Node
	{
		Node(T v) : value(v) {}
		~Node() {}
		T value;

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
			out << *value << endl;
			if (right != nullptr) {
				right->Output(out);
			}
		}
	};
	Node* root_ = nullptr;
	size_t count_ = 0;

	void Remove(Node** node_to_remove) {
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

	Node** FindNode(const T& value, Node** root) {
		if (*root == nullptr) {
			return root;
		}

		if ((*root)->value > value)
		{
			return FindNode(value, &(*root)->left);
		}
		else if ((*root)->value < value)
		{
			return FindNode(value, &(*root)->right);
		}
	}
};

TEST_CASE("Testing Queue class") {
	Queue<int> queue;

	REQUIRE(queue.GetSize() == 0);
	REQUIRE(queue.IsEmpty() == true);

	for (size_t i = 1; i < 11; i++)
	{
		queue.Enqueue(i);
	}

	SUBCASE("Testing Enqueue") {
		CHECK(queue.GetSize() == 10);
		CHECK(queue.IsEmpty() == false);
	}

	SUBCASE("Testing Dequeue") {
		for (size_t i = 1; i < 11; i++)
		{
			CHECK(queue.Head() == i);
			CHECK(queue.Dequeue() == true);
		}

		CHECK(queue.GetSize() == 0);
		CHECK(queue.IsEmpty() == true);
	}

	SUBCASE("Testing Clear") {
		queue.Clear();

		CHECK(queue.GetSize() == 0);
		CHECK(queue.IsEmpty() == true);
	}

	SUBCASE("Testing Head") {
		queue.Clear();

		CHECK_THROWS_AS(queue.Head(), underflow_error);
		CHECK(queue.Dequeue() == false);
		CHECK_THROWS_AS(queue.Head(), underflow_error);
	}
}

TEST_CASE("Testing PriorityQueue class") {
	PriorityQueue<int> queue;

	REQUIRE(queue.GetCount() == 0);
	REQUIRE(queue.IsEmpty() == true);

	for (size_t i = 1; i < 11; i++)
	{
		queue.Enqueue(i);
	}

	SUBCASE("Testing Enqueue") {
		CHECK(queue.GetCount() == 10);
		CHECK(queue.IsEmpty() == false);
	}

	SUBCASE("Testing Dequeue Min") {
		for (size_t i = 1; i < 11; i++)
		{
			CHECK(queue.PeekMin() == i);
			CHECK(queue.PopMin() == true);
		}

		CHECK(queue.GetCount() == 0);
		CHECK(queue.IsEmpty() == true);
	}

	SUBCASE("Testing Dequeue Max") {
		for (size_t i = 10; i > 0; i--)
		{
			CHECK(queue.PeekMax() == i);
			CHECK(queue.PopMax() == true);
		}

		CHECK(queue.GetCount() == 0);
		CHECK(queue.IsEmpty() == true);
	}

	SUBCASE("Testing Clear") {
		queue.Clear();

		CHECK(queue.GetCount() == 0);
		CHECK(queue.IsEmpty() == true);
	}

	SUBCASE("Testing Peek") {
		queue.Clear();

		CHECK_THROWS_AS(queue.PeekMax(), underflow_error);
		CHECK(queue.PopMax() == false);
		CHECK_THROWS_AS(queue.PeekMin(), underflow_error);
		CHECK(queue.PopMin() == false);
	}
}