

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <stack>


#include "priorityqueue.h"


#include <math.h>




struct pos {
	int x, y;
	int step;
};


const int kAroundX[3] = {
	-1, 0, 1,
};

const int kAroundY[3] = {
	-1, 0, 1,
};


int heuristic(const struct pos& from, const struct pos& target)
{
	int width = abs(from.x - target.x);
	int height = abs(from.y - target.y);
	return width > height ? width : height;
	//return (width * width + height * height);
}

int heuristicWithEpsilon(const struct pos& from, const struct pos& target, const int epsilon)
{
	int width = abs(from.x - target.x);
	int height = abs(from.y - target.y);
	return (width > height ? width : height) * epsilon;
	//return (width * width + height * height) * epsilon;
}

int g(const struct pos& p)
{
	return p.step;
}

void printWalkMap(char walkmap[20][20], int width, int height)
{
	std::ofstream f;
	f.open("data.txt", std::ios::out | std::ios::app | std::ios::binary);

	if (f.is_open()) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				f << std::setfill('0') << std::setw(2) << (int)walkmap[i][j] << ", ";
			}
			f << std::endl;
		}

		f << std::endl;
		f << std::endl;
		f.close();
	}
}

void printWalkMapWithEpsilon(char walkmap[20][20], int width, int height)
{
	std::ofstream f;
	f.open("datawithepsilon.txt", std::ios::out | std::ios::app | std::ios::binary);

	if (f.is_open()) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				f << std::setfill('0') << std::setw(2) << (int)walkmap[i][j] << ", ";
			}
			f << std::endl;
		}

		f << std::endl;
		f << std::endl;
		f.close();
	}
}


void bfs(priorityqueue::MinPriorityQueue<int, struct pos> &q, const char map[20][20], char walkmap[20][20], char walkedmap[20][20], int width, int height, const struct pos &t)
{
	if (q.size() == 0)
		return;
	struct pos p = q.top();
	q.pop();

	walkmap[p.y][p.x] = p.step + 1;
	printWalkMap(walkmap, width, height);
	if (t.x == p.x && t.y == p.y) {
		return;
	}

	struct pos node;

	node.step = p.step + 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			node.x = p.x + kAroundX[j];
			node.y = p.y + kAroundY[i];

			if (node.x < 0 || node.x >= width || node.y < 0 || node.y >= height)
				continue;
			if (walkedmap[node.y][node.x] > 0 || map[node.y][node.x] == 0)
				continue;

			walkedmap[node.y][node.x] = node.step + 1;
			q.push(heuristic(node, t) + g(node), node);
		}
	}
	
	bfs(q, map, walkmap, walkedmap, width, height, t);
}


void bfsWithEpsilon(priorityqueue::MinPriorityQueue<int, struct pos> &q, const char map[20][20], char walkmap[20][20], char walkedmap[20][20], int width, int height, const struct pos &t, int epsilon)
{
	if (q.size() == 0)
		return;
	struct pos p = q.top();
	q.pop();

	walkmap[p.y][p.x] = p.step + 1;
	printWalkMapWithEpsilon(walkmap, width, height);
	if (t.x == p.x && t.y == p.y) {
		return;
	}

	struct pos node;

	node.step = p.step + 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			node.x = p.x + kAroundX[j];
			node.y = p.y + kAroundY[i];

			if (node.x < 0 || node.x >= width || node.y < 0 || node.y >= height)
				continue;
			if (walkedmap[node.y][node.x] > 0 || map[node.y][node.x] == 0)
				continue;

			walkedmap[node.y][node.x] = node.step + 1;
			q.push(heuristicWithEpsilon(node, t, epsilon) + g(node), node);
		}
	}

	bfsWithEpsilon(q, map, walkmap, walkedmap, width, height, t, epsilon);
}

void getPath(const char walkmap[20][20], int width, int height, const struct pos &from, const struct pos &to)
{
	struct pos p;
	p.x = from.x;
	p.y = from.y;
	p.step = walkmap[p.y][p.x];

	std::stack<struct pos> path;

	path.push(p);

	struct pos next;
	struct pos node;

	int maxint = 0x7fffffff;
	while (p.x != to.x || p.y != to.y) {

		next.step = p.step;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				node.x = p.x + kAroundX[j];
				node.y = p.y + kAroundY[i];
				if (node.x < 0 || node.x >= width || node.y < 0 || node.y >= height)
					continue;
				if (walkmap[node.y][node.x] == 0)
					continue;
				if (walkmap[node.y][node.x] < next.step) {
					next.step = walkmap[node.y][node.x];
					next.x = node.x;
					next.y = node.y;
				}
			}
		}

		if (next.step == p.step) {
			break;
		}
		path.push(next);
		p.x = next.x;
		p.y = next.y;
		p.step = next.step;
	}


	while (path.size() > 0) {
		node = path.top();
		path.pop();

		std::cout << std::setfill('0') << std::setw(2) << node.x;
		std::cout << ", ";
		std::cout << std::setfill('0') << std::setw(2) << node.y;
		std::cout << ", ";
		std::cout << std::setfill('0') << std::setw(2) << node.step;
		std::cout << std::endl;
	}
}

int main()
{
	const char map[20][20] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	int width = 20;
	int height = 20;

	struct pos begin;
	struct pos tail;
	
	begin.x = 1;
	begin.y = 18;

	tail.x = 17;
	tail.y = 2;


	char walkedmap[20][20];
	char walkmap[20][20];
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			walkmap[i][j] = 0;
			walkedmap[i][j] = 0;
		}
	}


	struct pos start;
	start.step = 0;
	start.x = begin.x;
	start.y = begin.y;

	priorityqueue::MinPriorityQueue<int, struct pos> q;

	q.push(heuristic(start, tail) + g(start), start);
	walkedmap[start.y][start.x] = start.step + 1;

	//bfs(q, map, walkmap, walkedmap, width, height, tail);
	bfsWithEpsilon(q, map, walkmap, walkedmap, width, height, tail, 5);

	getPath(walkmap, width, height, tail, start);
}