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
	size_t** matrix = nullptr;

	Graph() {
		matrix = new size_t * [n];
		for (size_t i = 0; i < n; i++)
		{
			matrix[i] = new size_t[n];
			for (size_t j = 0; j < n; j++)
			{
				matrix[i][j] = 0;
			}
		}
	}
	Graph(size_t m[n][n]) {
		matrix = new size_t * [n];
		for (size_t i = 0; i < n; i++)
		{
			matrix[i] = new size_t[n];
			for (size_t j = 0; j < n; j++)
			{
				matrix[i][j] = m[i][j];
			}
		}
	}
	
	pair<string,size_t> Path(size_t start, size_t end) {
		size_t l[n];
		size_t prev[n];
		bool flags[n];

		swap(start, end);

		for (size_t i = 0; i < n; i++)
		{
			l[i] = -1;
			prev[i] = -1;
			flags[i] = false;
		}
		l[start] = 0;
		
		priority_queue<pair<size_t, size_t>, vector<pair<size_t, size_t>>, PairComparer<size_t, size_t>> pq;
		
		pq.push({ start,0 });
		size_t i;
		while (!pq.empty())
		{
			i = pq.top().first;
			pq.pop();
			if (flags[i]) continue;
			for (size_t j = 0; j < n; j++)
			{
				if ((matrix[i][j] != 0) && (matrix[i][j] + l[i] < l[j])) {
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
		
		return { path.str(), l[end] };
	}
};

TEST_CASE("Path should return the shortest path between two nodes in a weighted graph") {

	// Example graph from https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
	size_t example_graph[9][9] = {
		{0, 4, 0, 0, 0, 0, 0, 8, 0},
		{4, 0, 8, 0, 0, 0, 0, 11, 0},
		{0, 8, 0, 7, 0, 4, 0, 0, 2},
		{0, 0, 7, 0, 9, 14, 0, 0, 0},
		{0, 0, 0, 9, 0, 10, 0, 0, 0},
		{0, 0, 4, 14, 10, 0, 2, 0, 0},
		{0, 0, 0, 0, 0, 2, 0, 1, 6},
		{8, 11, 0, 0, 0, 0, 1, 0, 7},
		{0, 0, 2, 0, 0, 0, 6, 7, 0}
	};

	Graph<9> graph(example_graph);

	SUBCASE("Test shortest path from node 0 to node 4") {
		auto [path, cost] = graph.Path(0, 4);
		CHECK(path == "4 3 2 1 0 ");
		CHECK(cost == 21);
	}

	SUBCASE("Test shortest path from node 1 to node 8") {
		auto [path, cost] = graph.Path(1, 8);
		CHECK(path == "8 2 1 ");
		CHECK(cost == 10);
	}

	SUBCASE("Test shortest path from node 6 to node 7") {
		auto [path, cost] = graph.Path(6, 7);
		CHECK(path == "7 6 ");
		CHECK(cost == 3);
	}

	SUBCASE("Test shortest path from node 5 to node 5") {
		auto [path, cost] = graph.Path(5, 5);
		CHECK(path == "5 ");
		CHECK(cost == 0);
	}
}

TEST_CASE("Path - basic test case") {
	size_t matrix[5][5] = { {0, 1, 3, 4, 0},
						   {0, 0, 2, 0, 0},
						   {0, 0, 0, 0, 1},
						   {0, 0, 0, 0, 3},
						   {0, 0, 0, 0, 0} };
	Graph<5> g(matrix);

	SUBCASE("start and end are the same") {
		auto path = g.Path(1, 1);
		CHECK(path.first == "1 ");
		CHECK(path.second == 0);
	}

	SUBCASE("there is no path") {
		auto path = g.Path(1, 5);
		CHECK(path.first == "");
		CHECK(path.second == -1);
	}

	SUBCASE("path exists") {
		auto path = g.Path(1, 5);
		CHECK(path.first == "1 2 3 5 ");
		CHECK(path.second == 6);
	}
}

TEST_CASE("Path - more complex test case") {
	size_t matrix[5][5] = { {0, 2, 5, 0, 0},
						   {2, 0, 7, 0, 1},
						   {5, 7, 0, 1, 0},
						   {0, 0, 1, 0, 4},
						   {0, 1, 0, 4, 0} };
	Graph<5> g(matrix);

	SUBCASE("start and end are the same") {
		auto path = g.Path(3, 3);
		CHECK(path.first == "3 ");
		CHECK(path.second == 0);
	}

	SUBCASE("there is no path") {
		auto path = g.Path(4, 1);
		CHECK(path.first == "");
		CHECK(path.second == -1);
	}

	SUBCASE("path exists") {
		auto path = g.Path(1, 5);
		CHECK(path.first == "1 2 5 ");
		CHECK(path.second == 3);
	}
}

//int main()
//{
//	size_t G[8][8] = {
//					{0,4,0,0,3,0,0,0},
//					{4,0,5,0,2,0,0,0},
//					{0,5,0,5,3,1,0,0},
//					{0,0,5,0,0,0,2,7},
//					{3,2,3,0,0,6,0,0},
//					{0,0,1,0,6,0,5,0},
//					{0,0,0,2,0,5,0,4},
//					{0,0,0,7,0,0,4,0}
//	};
//
//	Graph<8> g(G);
//
//	cout << g.Path(0, 7).second;
//}