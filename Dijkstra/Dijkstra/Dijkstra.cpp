#include <iostream>

using namespace std;

struct Graph
{
	size_t n = 0;
	size_t** G = nullptr;

	Graph(size_t n_t) : n(n_t) {
		G = new size_t * [n];
		for (size_t i = 0; i < n; i++)
		{
			G[i] = new size_t[n];
			for (size_t j = 0; j < n; j++)
			{
				G[i][j] = 0;
			}
		}
	}
};

int main()
{
	size_t G[8][8] = {
					{0,4,0,0,3,0,0,0},
					{4,0,5,0,2,0,0,0},
					{0,5,0,5,3,1,0,0},
					{0,0,5,0,0,0,2,7},
					{3,2,3,0,0,6,0,0},
					{0,0,1,0,6,0,5,0},
					{0,0,0,2,0,5,0,4},
					{0,0,0,7,0,0,4,0}
	};

	size_t l[8] = { -1,-1,-1,-1,-1,-1,-1,-1 };
	size_t prev[8];

	size_t start = 0;

	l[start] = 0;

	for (size_t i_t = 0; i_t < 8; i_t++)
	{
		size_t i = i_t + start;
		for (size_t j = 0; j < 8; j++)
		{
			if (G[i][j] != 0) 
			{
				if (G[i][j] + l[i] < l[j]) 
				{
					l[j] = G[i][j] + l[i];
					prev[j] = i;
				}
			}
		}
	}

	for (size_t i = 0; i < 8; i++)
	{
		cout << prev[i] << " ";
	}
}