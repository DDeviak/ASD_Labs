#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "D:\ASD\doctest.h"
#include <iostream>
#include <queue>
#include <sstream>

using namespace std;

template<typename First,typename Second>
class PairComparer
{
public:
	bool operator()(pair<First,Second> p1, pair<First, Second> p2)
	{
		return p1.second > p2.second;
	}
};

template <size_t n>
struct Graph
{
	double** matrix = nullptr;

	Graph() {
		matrix = new double * [n];
		for (size_t i = 0; i < n; i++)
		{
			matrix[i] = new double[n];
			for (size_t j = 0; j < n; j++)
			{
				matrix[i][j] = 0;
			}
		}
	}
	Graph(double m[n][n]) {
		matrix = new double * [n];
		for (size_t i = 0; i < n; i++)
		{
			matrix[i] = new double[n];
			for (size_t j = 0; j < n; j++)
			{
				matrix[i][j] = m[i][j];
			}
		}
	}
	
	pair<string,double> Path(size_t start, size_t end) {
		double l[n];
		size_t prev[n];
		bool flags[n];

		//swap(start, end);

		for (size_t i = 0; i < n; i++)
		{
			l[i] = numeric_limits<double>().infinity();
			prev[i] = -1;
			flags[i] = false;
		}
		l[start] = 0;
		
		priority_queue<pair<size_t, double>, vector<pair<size_t, double>>, PairComparer<size_t, double>> pq;
		
		pq.push({ start,0 });
		size_t i;
		while (!pq.empty())
		{
			i = pq.top().first;
			pq.pop();
			if (flags[i]) continue;
			for (size_t j = 0; j < n; j++)
			{
				if ((matrix[i][j] > 0) && (matrix[i][j] + l[i] < l[j])) {
					l[j] = matrix[i][j] + l[i];
					prev[j] = i;
					pq.push({ j,l[j] });
				}
			}
			flags[i] = true;
		}
		
		i = end;
		stringstream path;
		while (i < n)
		{
			path << i << " ";
			i = prev[i];
		}
		
		return { path.str(), l[end]};
	}
};

TEST_CASE("Testing Dijkstra algorithm on simple graph")
{
	double G[8][8] = {
					{0,4,0,0,3,0,0,0},
					{4,0,5,0,2,0,0,0},
					{0,5,6,5,3,1,0,0},
					{0,0,5,0,0,0,2,7},
					{3,2,3,0,0,6,0,0},
					{0,0,1,0,6,8,5,0},
					{0,0,0,2,0,5,0,4},
					{0,0,0,7,0,0,4,2}
	};

	Graph<8> g(G);

	double reachability_matrix[8][8] = {
					{0,4,6,11,3,7,12,16},
					{4,0,5,10,2,6,11,15},
					{6,5,0,5,3,1,6,10},
					{11,10,5,0,8,6,2,6},
					{3,2,3,8,0,4,9,13},
					{7,6,1,6,4,0,5,9},
					{12,11,6,2,9,5,0,4},
					{16,15,10,6,13,9,4,0}
	};
	pair<string, double> path;

	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			path = g.Path(i, j);
			CHECK(path.second == reachability_matrix[i][j]);
			if (i == j)
				CHECK(path.first == (to_string(i)+" "));
		}
	}
}

TEST_CASE("Testing Dijkstra algorithm on oriented graph")
{
	double G[6][6] = {
					{0,10,20,0,0,0},
					{0,0,0,50,10,0},
					{0,0,0,20,33,0},
					{0,0,0,0,20,2},
					{0,0,0,0,0,1},
					{0,0,0,0,0,0}
	};

	Graph<6> g(G);

	double inf = numeric_limits<double>().infinity();

	double reachability_matrix[6][6] = {
					{0,10,20,40,20,21},
					{inf,0,inf,50,10,11},
					{inf,inf,0,20,33,22},
					{inf,inf,inf,0,20,2},
					{inf,inf,inf,inf,0,1},
					{inf,inf,inf,inf,inf,0},
	};
	pair<string, double> path;

	for (size_t i = 0; i < 6; i++)
	{
		for (size_t j = 0; j < 6; j++)
		{
			path = g.Path(i, j);
			CHECK(path.second == reachability_matrix[i][j]);
			if (i == j)
				CHECK(path.first == (to_string(i) + " "));
		}
	}
}