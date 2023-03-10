#include <iostream>
#include <ctime>
using namespace std;

class List
{
public:
	List() 
	{
		first_ = new Item();
	}
	~List() 
	{
		Item* current = first_;
		Item* previous;

		while (current->next != NULL)
		{
			previous = current;
			current = current->next;
			delete previous;
		}

		delete current;
	}

	void Add(int element) {
		Item* current = first_;
		while (current->next!=NULL)
		{
			current = current->next;
		}
		current->next = new Item();
		current->value = element;
		length_++;
	}

	void Sort()
	{
		Item* current;
		bool swaped = false;
		for (size_t i = 0; i < length_; i++)
		{
			swaped = false;
			current = first_;
			for (size_t j = 0; j < length_ - i - 1; j++)
			{
				if (current->value > current->next->value)
				{
					swap(current->value, current->next->value);
					swaped = true;
				}
				current = current->next;
			}
			if (!swaped) break;
		}
	}


	int* ToArray() {
		int* arr = new int[length_];
		Item* current = first_;
		for (size_t i = 0; i < length_; i++)
		{
			arr[i] = current->value;
			current = current->next;
		}
		return arr;
	}

	size_t get_length_() {
		return length_;
	}

	int& operator[] (const int index)
	{
		Item* current = first_;
		for (size_t i = 0; i < index; i++)
		{
			current = current->next;
		}
		return current->value;
	}

private:
	struct Item
	{
		int value;
		Item* next;
	};
	
	Item* first_ = new Item();

	size_t length_ = 0;
};

void RandFill(int* arr, size_t n) 
{
	for (size_t i = 0; i < n; i++)
	{
		arr[i] = rand() % 100;
	}
}

int Max(int* arr, size_t n) {
	int res = arr[0];
	for (size_t i = 1; i < n; i++)
	{
		res = arr[i] > res ? arr[i] : res;
	}
	return res;
}
int Min(int* arr, size_t n) {
	int res = arr[0];
	for (size_t i = 1; i < n; i++)
	{
		res = arr[i] < res ? arr[i] : res;
	}
	return res;
}

void PrintArr(int* arr, size_t n) 
{
	for (size_t i = 0; i < n; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

///////////////////////////////////////////////////////////

void BubbleSort(int* arr, size_t n) 
{
    bool swaped = false;
	for (size_t i = 0; i < n; i++)
	{
		swaped = false;
		for (size_t j = 0; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				swap(arr[j], arr[j + 1]);
				swaped = true;
			}
		}
		if (!swaped) break;
	}
}

void BucketSort(int* arr, size_t n) 
{
	List buckets[5];

	int min = Min(arr, n);

	float range = (Max(arr, n) - min + 1) / 5.0f;

	for (size_t i = 0; i < n; i++)
	{
		size_t bucket_ID = floor((float)(arr[i] - min) / range);
		buckets[bucket_ID].Add(arr[i]);
	}

	size_t index = 0;
	for (size_t i = 0; i < 5; i++)
	{
		buckets[i].Sort();
		for (size_t j = 0; j < buckets[i].get_length_(); j++)
		{
			arr[index] = buckets[i][j];
			index++;
		}
	}
}


int main()
{
	srand(time(nullptr));

	const size_t n = 10;
	int arr[n];
	
	cout << "Bubble sort" << endl;
	RandFill(arr, n);
	PrintArr(arr, n);
	BubbleSort(arr, n);
	PrintArr(arr, n);
	
	cout << endl << "Bucket sort" << endl;
	RandFill(arr, n);
	PrintArr(arr, n);
	BucketSort(arr, n);
	PrintArr(arr, n);

}