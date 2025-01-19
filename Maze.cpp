#include "Maze.h"
#include <queue>
#include <vector>

// 影響マップの生成
void GenerateInfluenceMap(int px, int py) 
{
	// 全てのセルの影響力を初期化
	for (int y = 0; y < MAZE_HEIGHT; ++y) {
		for (int x = 0; x < MAZE_WIDTH; ++x) {
			influenceMap[y][x] = 0;
		}
	}

	// 姫の位置から影響力を設定
	std::queue<std::pair<int, int>> queue;
	queue.push({ px, py });
	influenceMap[py][px] = 100; // 姫の位置の影響力を最大に

	while (!queue.empty()) {
		std::pair<int, int> current = queue.front();
		queue.pop();
		int cx = current.first;
		int cy = current.second;
		int currentInfluence = influenceMap[cy][cx];

		// 4方向に影響力を伝播
		std::vector<std::pair<int, int>> directions = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };
		for (auto& dir : directions) {
			int nx = cx + dir.first;
			int ny = cy + dir.second;
			if (nx >= 0 && nx < MAZE_WIDTH && ny >= 0 && ny < MAZE_HEIGHT && Maze[ny][nx] != 1) {
				int newInfluence = currentInfluence - 1;
				if (newInfluence > influenceMap[ny][nx]) {
					influenceMap[ny][nx] = newInfluence;
					queue.push({ nx, ny });
				}
			}
		}
	}
}

// 迷路にあるドットの数を数える
int CountDot(int nowMaze[MAZE_HEIGHT][MAZE_WIDTH])
{
	int count = 0;

	// 二重ループで迷路配列を走査
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_WIDTH; j++) {
			// 要素が0である場合にカウントを増やす
			if (nowMaze[i][j] == 0) {
				count++;
			}
		}
	}

	return count;
}