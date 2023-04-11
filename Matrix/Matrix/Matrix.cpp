#include <iostream>
#include <list>

using namespace std;

class Matrix
{
public:
	Matrix(int** arr, size_t n = 0, size_t m = 0) :n_(n), m_(m) {
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
		if (!(i < n_ && j < m_))throw "Index out of bounds";

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
		if (!(i < n_ && j < m_))throw "Index out of bounds";
		
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
		if ((n_ == m->n_) && (m_ == m->m_)) throw "Unable to add Matrixes with different size";
		
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
		n_ = n;
		list<pair<size_t, list<pair<size_t, int>>>>::iterator it_i;
		for (it_i = rows_.begin(); it_i != rows_.end(); it_i++)
		{
			if (it_i->first >= n) {
				rows_.erase(it_i);
			}
		}
	}	
	void SetColumnsCount(size_t m) { 
		m_ = m;
		list<pair<size_t, list<pair<size_t, int>>>>::iterator it_i;
		list<pair<size_t, int>>::iterator it_j;
		for (it_i = rows_.begin(); it_i != rows_.end(); it_i++)
		{
			for (it_j = it_i->second.begin(); it_j != it_i->second.end(); it_j++)
			{
				if (it_i->first >= m) {
					it_i->second.erase(it_j);
				}
			}
		}
	}

private:
	size_t n_, m_;
	list<pair<size_t, list<pair<size_t, int>>>> rows_;
};


int main()
{
    
}