#include "Dxlib.h"
#include "Maze.h"
#include "RouteSearch.h"
#include <math.h>
#include <cmath>
#include <vector>
#include <iostream>

// �P�ƃS�[�X�g�̈ʒu�i�[�ϐ�
int HimeX;
int HimeY;
int GhostX;
int GhostY;

// �X�R�A
int Score;

// �t���[���J�E���g�̕ϐ�
int FrameCount = 0;

// �S�[�X�g�̑��x
int GhostSpeed = 40;

// �Q�[���̏�Ԃ��Ǘ�����ϐ�
// 0 : �^�C�g�����
// 1 : �Q�[���J�n
// 2 : �Q�[���I�[�o�[
// 3 : �Q�[���I��
int GameState = 0;

// �Q�[���������֐�
void InitializeGame() {
	HimeX = 1 * CELL_SIZE + CELL_SIZE / 2;
	HimeY = 1 * CELL_SIZE + CELL_SIZE / 2;
	GhostX = 18 * CELL_SIZE + CELL_SIZE / 2;
	GhostY = 18 * CELL_SIZE + CELL_SIZE / 2;
	Score = 0;

}

// �S�[�X�g�̈ړ�
void MoveGhost() {
	int StartX = GhostX / CELL_SIZE;
	int StartY = GhostY / CELL_SIZE;
	int GoalX = (HimeX - CELL_SIZE / 2) / CELL_SIZE;
	int GoalY = (HimeY - CELL_SIZE / 2) / CELL_SIZE;

	GenerateInfluenceMap(GoalX, GoalY); // �e���}�b�v�𐶐�
	std::vector<std::pair<int, int>> Path = AStar(StartX, StartY, GoalX, GoalY);
	
	if (Path.size() > 1) {
		GhostX = Path[1].first * CELL_SIZE + CELL_SIZE / 2;
		GhostY = Path[1].second * CELL_SIZE + CELL_SIZE / 2;
	}
}

// �Փ˔��菈��
bool CheckCollision(int HimeX, int HimeY, int GhostX, int GhostY, int Radius)
{
	// �P�ƃS�[�X�g�̋�����0�ł���ΏՓ˂Ƃ���
	int Distancex = HimeX - GhostX;
	int DistanceY = HimeY - GhostY;
	return (Distancex * Distancex + DistanceY * DistanceY) <= (Radius * Radius);
}

// �^�C�g����ʕ\��
void ShowTitleScreen(int HandlePrinces, int HandleGhost) {

	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		// �^�C�g����ʂ̕`��
		// �Q�[���^�C�g��
		int HandleTitle;
		HandleTitle = LoadGraph("Resource/Logo/Title_logo.png");
		DrawRotaGraph(320, 120, 1, 0, HandleTitle, TRUE);

		// ������@����
		SetFontSize(16);
		DrawString(185, 260, "�P�𑀍삵�Č��̋ʂ��W�߂悤�I", GetColor(255, 255, 255));
		DrawString(225, 300, "���������L�[�ňړ��I", GetColor(255, 255, 255));
		DrawString(170, 340, "���΂��ɕ߂܂�ƃQ�[���I�[�o�[���I", GetColor(255, 255, 255));

		// �X�^�[�g���S
		int HandleStart;
		HandleStart = LoadGraph("Resource/Logo/START_rogo.png");
		DrawRotaGraph(315, 480, 0.3, 0, HandleStart, TRUE);

		// �P�`��
		DrawRotaGraph(90, 330, 0.18, 5.93412, HandlePrinces, TRUE);

		// �S�[�X�g�`��
		DrawRotaGraph(530, 330, 0.25, 0.3491, HandleGhost, TRUE);

		// ���̓`�F�b�N
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			GameState = 1; // �Q�[���J�n
			InitializeGame(); // ������
			break; // SPACE�L�[�������ꂽ��Q�[�����J�n
		}

		// ��ʍX�V
		ScreenFlip();
	}
}

// �Q�[���I�[�o�[��ʕ\���֐�
void ShowGameOverScreen() {
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		// �Q�[���I�[�o�[��ʂ̕`��
		SetFontSize(48);
		DrawString(140, 200, "�Q�[���I�[�o�[!", GetColor(255, 255, 0));

		SetFontSize(24);
		DrawFormatString(250, 290, GetColor(255, 255, 255), "�X�R�A: %d", Score);

		SetFontSize(16);
		DrawString(200, 360, "Enter�L�[�Ń^�C�g���ɖ߂��I", GetColor(255, 255, 255));
		DrawString(210, 400, "�X�y�[�X�L�[�Ń��g���C�I", GetColor(255, 255, 255));

		// ���̓`�F�b�N
		if (CheckHitKey(KEY_INPUT_RETURN)) {
			GameState = 0; // �^�C�g����ʂɖ߂�
			break;
		}
		else if (CheckHitKey(KEY_INPUT_SPACE))
		{
			InitializeGame(); // ������
			GameState = 1; // ���g���C
			break;
		}

		// ��ʍX�V
		ScreenFlip();
	}
}

// �Q�[���N���A���
void ShowGameClearScreen() 
{
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		// �Q�[���N���A��ʂ̕`��
		SetFontSize(40);
		DrawString(70, 200, "�Q�[���N���A!���߂łƂ��I", GetColor(255, 255, 0));

		SetFontSize(24);
		DrawFormatString(250, 290, GetColor(255, 255, 255), "�X�R�A: %d", Score);

		SetFontSize(16);
		DrawString(200, 360, "Enter�L�[�Ń^�C�g���ɖ߂��I", GetColor(255, 255, 255));
		DrawString(210, 400, "�X�y�[�X�L�[�ŃQ�[���I���I", GetColor(255, 255, 255));

		// ���̓`�F�b�N
		if (CheckHitKey(KEY_INPUT_RETURN)) {
			GameState = 0; // �^�C�g����ʂɖ߂�
			break;
		}
		else if (CheckHitKey(KEY_INPUT_SPACE))
		{
			GameState = 3; // �Q�[���I��
			break;
		}

		// ��ʍX�V
		ScreenFlip();
	}
}


// ���C���v���O����
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	//�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�
	ChangeWindowMode(TRUE);
	SetGraphMode(640, 640, 32);;
	if (DxLib_Init() == -1) return -1;

	SetDrawScreen(DX_SCREEN_BACK);

	// �P�ǂݍ���
	int HandlePrinces;
	HandlePrinces = LoadGraph("Resource/Character/character_hime_01_white_gold.png");

	// �S�[�X�g�ǂݍ���
	int HandleGhost;
	HandleGhost = LoadGraph("Resource/Character/character_monster_shinigami_02.png");

	// 1�}�X���P�𓮂������߂̕ϐ�
	// �i���ꂪ�Ȃ���Έړ��L�[���������ςȂ��ɂ����ꍇ�A��C�ɒ[�̃}�X�܂ňړ����Ă��܂��j
	bool KeyUpPressed = false;
    bool KeyDownPressed = false;
    bool KeyLeftPressedressed = false;
    bool KeyRightPressed = false;

	// ���H������
	int MazeMap[MAZE_HEIGHT][MAZE_WIDTH];
	std::memcpy(MazeMap, Maze, sizeof(Maze));

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// �Q�[����Ԃŕ���
		// �^�C�g�����
		if (GameState == 0)
		{
			ShowTitleScreen(HandlePrinces, HandleGhost);
		}

		// �Q�[���J�n
		else if (GameState == 1)
		{
			// �S��ʏ���
			ClearDrawScreen();

			// �L�[���͂̎擾
			int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			// �P�̈ړ�
			// ��
			if ((Key & PAD_INPUT_UP) && !KeyUpPressed) {
				KeyUpPressed = true;
				if (MazeMap[(HimeY - CELL_SIZE / 2 - 5) / CELL_SIZE][HimeX / CELL_SIZE] != 1) {
					HimeY -= CELL_SIZE;
				}
			}
			else if (!(Key & PAD_INPUT_UP)) {
				KeyUpPressed = false;
			}

			// ��
			if ((Key & PAD_INPUT_DOWN) && !KeyDownPressed) {
				KeyDownPressed = true;
				if (MazeMap[(HimeY + CELL_SIZE / 2 + 5) / CELL_SIZE][HimeX / CELL_SIZE] != 1) {
					HimeY += CELL_SIZE;
				}
			}
			else if (!(Key & PAD_INPUT_DOWN)) {
				KeyDownPressed = false;
			}

			// ��
			if ((Key & PAD_INPUT_LEFT) && !KeyLeftPressedressed) {
				KeyLeftPressedressed = true;
				if (MazeMap[HimeY / CELL_SIZE][(HimeX - CELL_SIZE / 2 - 5) / CELL_SIZE] != 1) {
					HimeX -= CELL_SIZE;
				}
			}
			else if (!(Key & PAD_INPUT_LEFT)) {
				KeyLeftPressedressed = false;
			}

			// ��
			if ((Key & PAD_INPUT_RIGHT) && !KeyRightPressed) {
				KeyRightPressed = true;
				if (MazeMap[HimeY / CELL_SIZE][(HimeX + CELL_SIZE / 2 + 5) / CELL_SIZE] != 1) {
					HimeX += CELL_SIZE;
				}
			}
			else if (!(Key & PAD_INPUT_RIGHT)) {
				KeyRightPressed = false;
			}
			

			// �S�[�X�g�̈ړ�
			if (++FrameCount % GhostSpeed == 0) {
				MoveGhost();
				FrameCount = 0;
			}

			// �P���h�b�g���擾���鏈��
			int MazeX = HimeX / CELL_SIZE;
			int MazeY = HimeY / CELL_SIZE;
			if (MazeMap[MazeY][MazeX] == 0) {
				MazeMap[MazeY][MazeX] = 2; // �h�b�g������
				Score += 10; // �X�R�A�𑝂₷
			}

			// �Փ˔���
			if (CheckCollision(HimeX, HimeY, GhostX, GhostY, 20))
			{
				GameState = 2; // �Q�[���I�[�o�[��ʂ�
				std::memcpy(MazeMap, Maze, sizeof(Maze));

			}


			// ���H�̕`��
			// 1�͕ǁA2�͒ʘH�A0�̓h�b�g
			for (int y = 0; y < MAZE_HEIGHT; y++) {
				for (int x = 0; x < MAZE_WIDTH; x++) {
					if (MazeMap[y][x] == 1) {
						DrawBox(x * CELL_SIZE, y * CELL_SIZE, x * CELL_SIZE + CELL_SIZE, y * CELL_SIZE + CELL_SIZE, GetColor(128, 128, 128), TRUE);
					}
					else if (MazeMap[y][x] == 2)
					{
						DrawCircle(x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 4, GetColor(0, 0, 0), TRUE);
					}
					else {
						DrawCircle(x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE + CELL_SIZE / 2, 4, GetColor(255, 255, 0), TRUE);
					}
				}
			}

			// �h�b�g�����H�ɂȂ���΃Q�[���N���A
			if (CountDot(MazeMap) == 0)
			{
				ShowGameClearScreen();
			}

			// �P�̕`��
			DrawRotaGraph(HimeX, HimeY, 0.07, 0.0, HandlePrinces, TRUE);

			// �S�[�X�g�̕`��
			DrawRotaGraph(GhostX, GhostY, 0.1, 0.0, HandleGhost, TRUE);


			// �X�R�A�̕\��
			DrawFormatString(10, 10, GetColor(255, 255, 255), "Score: %d", Score);

			// ��ʂ̍X�V
			ScreenFlip();
		}
		else if (GameState == 2)
		{
			// �Q�[���I�[�o�[���
			ShowGameOverScreen();
		}
		else if (GameState == 3)
		{
			break;
		}

	}

	DxLib_End(); // DX���C�u�����I������
	return 0;
}