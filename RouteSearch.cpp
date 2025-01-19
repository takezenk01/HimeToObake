#include "RouteSearch.h"
#include "Maze.h"

// �q���[���X�e�B�b�N�������v�Z
int Heuristic(int x1, int y1, int x2, int y2)
{
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// �אڂ���m�[�h�Z�o
std::vector<std::pair<int, int>> GetNeighbors(int x, int y)
{
	std::vector<std::pair<int, int>> Neighbors;

	// �㉺���E�̈ړ���\�������̃x�N�g��
	std::vector<std::pair<int, int>> Directions = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };

	for (auto& dir : Directions) {
		int nx = x + dir.first;
		int ny = y + dir.second;

		// ���W�����H�͈͓̔��ɂ���A����Q���łȂ��ꍇ�ɂ̂ݒǉ�
		if (nx >= 0 && nx < MAZE_WIDTH && ny >= 0 && ny < MAZE_HEIGHT && Maze[ny][nx] != 1) {
			Neighbors.push_back({ nx, ny });
		}
	}
	return Neighbors;
}

// �ړ��ʒu�o�́iA*�A���S���Y���j
std::vector<std::pair<int, int>> AStar(int StartX, int StartY, int GoalX, int GoalY) 
{
	std::priority_queue<Node> OpenList;

	// ���ׂẴm�[�h���Ǘ�����}�b�v
	std::unordered_map<int, Node*> AllNodes;

	// �J�n�m�[�h�̏�����
	Node* StartNode = new Node(StartX, StartY, 0, Heuristic(StartX, StartY, GoalX, GoalY));
	OpenList.push(*StartNode);
	AllNodes[StartY * MAZE_WIDTH + StartX] = StartNode;

	// �ł��i�񂾃m�[�h���i�[
	Node* BestNode = StartNode;

	while (!OpenList.empty()) {
		Node CurrentNode = OpenList.top();
		OpenList.pop();

		// �P���������ꍇ�A�o�H���\�z���ĕԂ�
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

		// �אڃm�[�h��T��
		for (auto& Neighbor : GetNeighbors(CurrentNode.X, CurrentNode.Y)) {
			int NewX = Neighbor.first;
			int NewY = Neighbor.second;
			int NewG = CurrentNode.G + 1;

			// �q���[���X�e�B�b�N�R�X�g�̌v�Z
			int NewH = Heuristic(NewX, NewY, GoalX, GoalY);

			// �e���}�b�v���l��
			int NewF = NewG + NewH - influenceMap[NewY][NewX];

			// �V�����m�[�h�����T���A�܂��͂��ǂ��o�H�����������ꍇ
			if (AllNodes.find(NewY * MAZE_WIDTH + NewX) == AllNodes.end() || NewF < AllNodes[NewY * MAZE_WIDTH + NewX]->F) {
				Node* NeighborNode = new Node(NewX, NewY, NewG, NewH, new Node(CurrentNode));
				OpenList.push(*NeighborNode);
				AllNodes[NewY * MAZE_WIDTH + NewX] = NeighborNode;

				// �ł��i�񂾃m�[�h���X�V
				if (NewH < BestNode->H) {
					BestNode = NeighborNode;
				}
			}
		}
	}

	// �o�H��������Ȃ������ꍇ�A�ł��i�񂾃m�[�h�̌o�H��Ԃ�
	std::vector<std::pair<int, int>> Path;
	Node* Node = BestNode;
	while (Node) {
		Path.push_back({ Node->X, Node->Y });
		Node = Node->Parent;
	}
	std::reverse(Path.begin(), Path.end());
	return Path;
}