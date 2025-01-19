#include "RouteSearch.h"
#include "Maze.h"

// ヒューリスティック距離を計算
int Heuristic(int x1, int y1, int x2, int y2)
{
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// 隣接するノード算出
std::vector<std::pair<int, int>> GetNeighbors(int x, int y)
{
	std::vector<std::pair<int, int>> Neighbors;

	// 上下左右の移動を表す方向のベクトル
	std::vector<std::pair<int, int>> Directions = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };

	for (auto& dir : Directions) {
		int nx = x + dir.first;
		int ny = y + dir.second;

		// 座標が迷路の範囲内にあり、かつ障害物でない場合にのみ追加
		if (nx >= 0 && nx < MAZE_WIDTH && ny >= 0 && ny < MAZE_HEIGHT && Maze[ny][nx] != 1) {
			Neighbors.push_back({ nx, ny });
		}
	}
	return Neighbors;
}

// 移動位置出力（A*アルゴリズム）
std::vector<std::pair<int, int>> AStar(int StartX, int StartY, int GoalX, int GoalY) 
{
	std::priority_queue<Node> OpenList;

	// すべてのノードを管理するマップ
	std::unordered_map<int, Node*> AllNodes;

	// 開始ノードの初期化
	Node* StartNode = new Node(StartX, StartY, 0, Heuristic(StartX, StartY, GoalX, GoalY));
	OpenList.push(*StartNode);
	AllNodes[StartY * MAZE_WIDTH + StartX] = StartNode;

	// 最も進んだノードを格納
	Node* BestNode = StartNode;

	while (!OpenList.empty()) {
		Node CurrentNode = OpenList.top();
		OpenList.pop();

		// 姫を見つけた場合、経路を構築して返す
		if (CurrentNode.X == GoalX && CurrentNode.Y == GoalY) {
			std::vector<std::pair<int, int>> Path;
			Node* Node = &CurrentNode;
			while (Node) {
				Path.push_back({ Node->X, Node->Y });
				Node = Node->Parent;
			}
			std::reverse(Path.begin(), Path.end());
			return Path;
		}

		// 隣接ノードを探索
		for (auto& Neighbor : GetNeighbors(CurrentNode.X, CurrentNode.Y)) {
			int NewX = Neighbor.first;
			int NewY = Neighbor.second;
			int NewG = CurrentNode.G + 1;

			// ヒューリスティックコストの計算
			int NewH = Heuristic(NewX, NewY, GoalX, GoalY);

			// 影響マップを考慮
			int NewF = NewG + NewH - influenceMap[NewY][NewX];

			// 新しいノードが未探索、またはより良い経路が見つかった場合
			if (AllNodes.find(NewY * MAZE_WIDTH + NewX) == AllNodes.end() || NewF < AllNodes[NewY * MAZE_WIDTH + NewX]->F) {
				Node* NeighborNode = new Node(NewX, NewY, NewG, NewH, new Node(CurrentNode));
				OpenList.push(*NeighborNode);
				AllNodes[NewY * MAZE_WIDTH + NewX] = NeighborNode;

				// 最も進んだノードを更新
				if (NewH < BestNode->H) {
					BestNode = NeighborNode;
				}
			}
		}
	}

	// 経路が見つからなかった場合、最も進んだノードの経路を返す
	std::vector<std::pair<int, int>> Path;
	Node* Node = BestNode;
	while (Node) {
		Path.push_back({ Node->X, Node->Y });
		Node = Node->Parent;
	}
	std::reverse(Path.begin(), Path.end());
	return Path;
}