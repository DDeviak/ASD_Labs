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
			rows_.push_back(pair<size_t, list<pair<size_t, int>>>(i, list<pair<size_t, int>>()));
			row = &rows_.back().second;
		}

		list<pair<size_t, int>>::iterator it_j;
		for (it_j = row->begin(); it_j != row->end(); it_j++)
		{

		}
	}



private:
	size_t n_, m_;
	list<pair<size_t, list<pair<size_t, int>>>> rows_;
};


int main()
{
    
}