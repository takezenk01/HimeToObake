#pragma once
#include "Dxlib.h"
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <memory>

// A*アルゴリズム
// A*アルゴリズムのヘルパー関数
struct Node {
	int X, Y;
	int G, H, F;
	Node* Parent;

	Node(int x, int y, int g, int h, Node* parent = nullptr)
		: X(x), Y(y), G(g), H(h), F(g + h), Parent(parent) {}

	bool operator<(const Node& other) const {
		return F > other.F;
	}
};

// ヒューリスティック距離計算関数
extern int Heuristic(int x1, int y1, int x2, int y2);

//  隣接するノード算出関数
extern std::vector<std::pair<int, int>> GetNeighbors(int x, int y);

// 移動位置出力関数（A*アルゴリズム）
extern std::vector<std::pair<int, int>> AStar(int startX, int startY, int goalX, int goalY);