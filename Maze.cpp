#include "Maze.h"
#include <queue>
#include <vector>

// �e���}�b�v�̐���
void GenerateInfluenceMap(int px, int py) 
{
	// �S�ẴZ���̉e���͂�������
	for (int y = 0; y < MAZE_HEIGHT; ++y) {
		for (int x = 0; x < MAZE_WIDTH; ++x) {
			influenceMap[y][x] = 0;
		}
	}

	// �P�̈ʒu����e���͂�ݒ�
	std::queue<std::pair<int, int>> queue;
	queue.push({ px, py });
	influenceMap[py][px] = 100; // �P�̈ʒu�̉e���͂��ő��

	while (!queue.empty()) {
		std::pair<int, int> current = queue.front();
		queue.pop();
		int cx = current.first;
		int cy = current.second;
		int currentInfluence = influenceMap[cy][cx];

		// 4�����ɉe���͂�`�d
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

// ���H�ɂ���h�b�g�̐��𐔂���
int CountDot(int nowMaze[MAZE_HEIGHT][MAZE_WIDTH])
{
	int count = 0;

	// ��d���[�v�Ŗ��H�z��𑖍�
	for (int i = 0; i < MAZE_HEIGHT; i++) {
		for (int j = 0; j < MAZE_WIDTH; j++) {
			// �v�f��0�ł���ꍇ�ɃJ�E���g�𑝂₷
			if (nowMaze[i][j] == 0) {
				count++;
			}
		}
	}

	return count;
}