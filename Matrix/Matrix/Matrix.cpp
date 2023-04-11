#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include "D:\ASD\doctest.h"
#include <list>
#include <stdexcept>

using namespace std;

class Matrix
{
public:
	Matrix(int** arr = nullptr, size_t n = 0, size_t m = 0) :n_(n), m_(m) {
		for (size_t i = 0; i < n_; i++)
		{
			rows_.push_back(pair<size_t, list<pair<size_t, int>>>(i, list<pair<size_t, int>>()));
			for (size_t j = 0; j < m_; j++)
			{
				if (arr[i][j] != 0) {
					rows_.back().second.push_back(pair<size_t, int>(j, arr[i][j]));
				}
			}
			if (rows_.back().second.empty()) {
				rows_.pop_back();
			}
		}
	}
	~Matrix() {}

	void SetElement(size_t i, size_t j, int el) {
		if (!(i < n_ && j < m_))throw out_of_range("Index out of bounds");

		list<pair<size_t, int>>* row = nullptr;
		list<pair<size_t, list<pair<size_t, int>>>>::iterator it_i;
		for (it_i = rows_.begin(); it_i != rows_.end(); it_i++)
		{
			if (it_i->first == i) {
				row = &it_i->second;
				break;
			}
		}
		if (row == nullptr) {
			if (el == 0)return;
			rows_.push_back(pair<size_t, list<pair<size_t, int>>>(i, list<pair<size_t, int>>()));
			row = &rows_.back().second;
		}
		
		int* element = nullptr;
		list<pair<size_t, int>>::iterator it_j;
		for (it_j = row->begin(); it_j != row->end(); it_j++)
		{
			if (it_j->first == j) {
				if (el == 0) {
					row->erase(it_j);
				}
				else 
				{
					element = &it_j->second;
				}
				break;
			}
		}
		if (element == nullptr) {
			if (el == 0) {
				if (row->size() == 0) {
					rows_.erase(it_i);
				}
				return;
			}
			row->push_back(pair<size_t, int>(j, 0));
			element = &row->back().second;
		}

		*element = el;
	}
	
	int GetElement(size_t i, size_t j) {
		if (!(i < n_ && j < m_))throw out_of_range("Index out of bounds");
		
		list<pair<size_t, int>>* row = nullptr;
		list<pair<size_t, list<pair<size_t, int>>>>::iterator it_i;
		for (it_i = rows_.begin(); it_i != rows_.end(); it_i++)
		{
			if (it_i->first == i) {
				row = &it_i->second;
				break;
			}
		}

		if (row == nullptr)return 0;

		int* element = nullptr;
		list<pair<size_t, int>>::iterator it_j;
		for (it_j = row->begin(); it_j != row->end(); it_j++)
		{
			if (it_j->first == j) {
				element = &it_j->second;
				break;
			}
		}
		
		if (element == nullptr)return 0;

		return *element;
	}

	int** ToArr() {
		int** arr = new int* [n_];
		for (size_t i = 0; i < n_; i++)
		{
			arr[i] = new int[m_];
			for (size_t j = 0; j < m_; j++)
			{
				arr[i][j] = 0;
			}
		}
		list<pair<size_t, list<pair<size_t, int>>>>::iterator it_i;
		list<pair<size_t, int>>::iterator it_j;
		for (it_i = rows_.begin(); it_i != rows_.end(); it_i++)
		{
			for (it_j = it_i->second.begin(); it_j != it_i->second.end(); it_j++)
			{
				arr[it_i->first][it_j->first] = it_j->second;
			}
		}
		return arr;
	}

	Matrix* Add(Matrix* m) {
		if (!((n_ == m->n_) && (m_ == m->m_))) throw invalid_argument("Unable to add Matrixes with different size");
		
		int** arr = m->ToArr();

		list<pair<size_t, list<pair<size_t, int>>>>::iterator it_i;
		list<pair<size_t, int>>::iterator it_j;
		for (it_i = rows_.begin(); it_i != rows_.end(); it_i++)
		{
			for (it_j = it_i->second.begin(); it_j != it_i->second.end(); it_j++)
			{
				arr[it_i->first][it_j->first] += it_j->second;
			}
		}
		return new Matrix(arr, n_, m_);
	}

	void Output(ostream& os) {
		int** arr = ToArr();
		for (size_t i = 0; i < n_; i++)
		{
			for (size_t j = 0; j < m_; j++)
			{
				os << arr[i][j] << " ";
			}
			os << endl;
			delete[] arr[i];
		}
		delete[] arr;
	}
	
	size_t GetRowsCount() { return n_; }
	size_t GetColumnsCount() { return m_; }

	void SetRowsCount(size_t n) { 
		if (n < n_) {
			rows_.remove_if([&](pair<size_t, list<pair<size_t, int>>> t) { return t.first >= n; });
		}
		n_ = n;
	}	
	void SetColumnsCount(size_t m) {
		if (m < m_) {
			list<pair<size_t, list<pair<size_t, int>>>>::iterator it_i;
			for (it_i = rows_.begin(); it_i != rows_.end(); it_i++)
			{
				it_i->second.remove_if([&](pair<size_t, int> t) { return t.first >= m; });
			}
		}
		m_ = m;
	}

	size_t ActualSize() {
		size_t size = 0;
		list<pair<size_t, list<pair<size_t, int>>>>::iterator it_i;
		for (it_i = rows_.begin(); it_i != rows_.end(); it_i++)
		{
			size += it_i->second.size();
		}
		return size;
	}
private:
	size_t n_, m_;
	list<pair<size_t, list<pair<size_t, int>>>> rows_;
};


TEST_CASE("Testing Matrix") {
	int arr[4][5] = {
		{0,2,0,6,5},
		{0,0,0,0,0},
		{0,9,5,4,0},
		{1,0,0,0,0}
	};
	//Testing default constructor
	Matrix m;
	CHECK(m.GetRowsCount() == 0);
	CHECK(m.GetColumnsCount() == 0);

	//Testing size setting
	m.SetRowsCount(4);
	m.SetColumnsCount(5);

	CHECK(m.GetRowsCount() == 4);
	CHECK(m.GetColumnsCount() == 5);

	m.Output(cout);
	cout << endl;

	//Filling matrix
	//Testing get/set methods
	size_t n = 0;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			CHECK(m.GetElement(i, j) == 0);
			m.SetElement(i, j, arr[i][j]);
			CHECK(m.GetElement(i, j) == arr[i][j]);
			n += (size_t)(arr[i][j] != 0);
		}
	}

	//Testing memory usage
	CHECK(m.ActualSize() == n);
	
	m.Output(cout);
	cout << endl;

	//Testing Index out of range behaviour
	CHECK_THROWS_AS(m.GetElement(6, 6), out_of_range);
	CHECK_THROWS_AS(m.SetElement(6, 6, 6), out_of_range);
	
	//Testing dynamic size increase
	m.SetRowsCount(7);
	m.SetColumnsCount(7);
	CHECK_NOTHROW(m.GetElement(6, 6));
	CHECK_NOTHROW(m.SetElement(6, 6, 6));
	
	m.Output(cout);
	cout << endl;

	//Testing memory usage
	CHECK(m.ActualSize() == ++n);

	//Testing dynamic size decrease
	m.SetRowsCount(3);
	m.SetColumnsCount(3);
	CHECK_THROWS_AS(m.GetElement(3, 3), out_of_range);
	CHECK_THROWS_AS(m.SetElement(3, 3, 6), out_of_range);
	
	//Testing memory usage
	CHECK(m.ActualSize() < n);

	m.Output(cout);
	cout << endl;

	int** arr1 = new int*[4];
	for (size_t i = 0; i < 4; i++)
	{
		arr1[i] = new int[4];
		for (size_t j = 0; j < 4; j++)
		{
			arr1[i][j] = -arr[i][j];
		}
	}

	//Testing constructor with parameters
	Matrix m1(arr1, 4, 4);
	m1.Output(cout);
	cout << endl;

	//Testing Addition method with diferent matrixes sizes
	CHECK_THROWS_AS(m.Add(&m1), invalid_argument);

	m1.SetRowsCount(3);
	m1.SetColumnsCount(3);

	//Testing Addition method with proper arguments
	Matrix* t = nullptr;
	CHECK_NOTHROW(t = m.Add(&m1));
	t->Output(cout);
	cout << endl;

	//Testing memory usage
	CHECK(t->ActualSize() == 0);

	for (size_t i = 0; i < 4; i++)
	{
		delete[] arr1[i];
	}
	delete[] arr1;

	delete t;
}