#include "Dxlib.h"
#include "Maze.h"
#include "RouteSearch.h"
#include <math.h>
#include <cmath>
#include <vector>
#include <iostream>

// 姫とゴーストの位置格納変数
int HimeX;
int HimeY;
int GhostX;
int GhostY;

// スコア
int Score;

// フレームカウントの変数
int FrameCount = 0;

// ゴーストの速度
int GhostSpeed = 40;

// ゲームの状態を管理する変数
// 0 : タイトル画面
// 1 : ゲーム開始
// 2 : ゲームオーバー
// 3 : ゲーム終了
int GameState = 0;

// ゲーム初期化関数
void InitializeGame() {
	HimeX = 1 * CELL_SIZE + CELL_SIZE / 2;
	HimeY = 1 * CELL_SIZE + CELL_SIZE / 2;
	GhostX = 18 * CELL_SIZE + CELL_SIZE / 2;
	GhostY = 18 * CELL_SIZE + CELL_SIZE / 2;
	Score = 0;

}

// ゴーストの移動
void MoveGhost() {
	int StartX = GhostX / CELL_SIZE;
	int StartY = GhostY / CELL_SIZE;
	int GoalX = (HimeX - CELL_SIZE / 2) / CELL_SIZE;
	int GoalY = (HimeY - CELL_SIZE / 2) / CELL_SIZE;

	GenerateInfluenceMap(GoalX, GoalY); // 影響マップを生成
	std::vector<std::pair<int, int>> Path = AStar(StartX, StartY, GoalX, GoalY);
	
	if (Path.size() > 1) {
		GhostX = Path[1].first * CELL_SIZE + CELL_SIZE / 2;
		GhostY = Path[1].second * CELL_SIZE + CELL_SIZE / 2;
	}
}

// 衝突判定処理
bool CheckCollision(int HimeX, int HimeY, int GhostX, int GhostY, int Radius)
{
	// 姫とゴーストの距離が0であれば衝突とする
	int Distancex = HimeX - GhostX;
	int DistanceY = HimeY - GhostY;
	return (Distancex * Distancex + DistanceY * DistanceY) <= (Radius * Radius);
}

// タイトル画面表示
void ShowTitleScreen(int HandlePrinces, int HandleGhost) {

	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		// タイトル画面の描画
		// ゲームタイトル
		int HandleTitle;
		HandleTitle = LoadGraph("Resource/Logo/Title_logo.png");
		DrawRotaGraph(320, 120, 1, 0, HandleTitle, TRUE);

		// 操作方法説明
		SetFontSize(16);
		DrawString(185, 260, "姫を操作して光の玉を集めよう！", GetColor(255, 255, 255));
		DrawString(225, 300, "←↑→↓キーで移動！", GetColor(255, 255, 255));
		DrawString(170, 340, "おばけに捕まるとゲームオーバーだ！", GetColor(255, 255, 255));

		// スタートロゴ
		int HandleStart;
		HandleStart = LoadGraph("Resource/Logo/START_rogo.png");
		DrawRotaGraph(315, 480, 0.3, 0, HandleStart, TRUE);

		// 姫描画
		DrawRotaGraph(90, 330, 0.18, 5.93412, HandlePrinces, TRUE);

		// ゴースト描画
		DrawRotaGraph(530, 330, 0.25, 0.3491, HandleGhost, TRUE);

		// 入力チェック
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			GameState = 1; // ゲーム開始
			InitializeGame(); // 初期化
			break; // SPACEキーが押されたらゲームを開始
		}

		// 画面更新
		ScreenFlip();
	}
}

// ゲームオーバー画面表示関数
void ShowGameOverScreen() {
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		// ゲームオーバー画面の描画
		SetFontSize(48);
		DrawString(140, 200, "ゲームオーバー!", GetColor(255, 255, 0));

		SetFontSize(24);
		DrawFormatString(250, 290, GetColor(255, 255, 255), "スコア: %d", Score);

		SetFontSize(16);
		DrawString(200, 360, "Enterキーでタイトルに戻るよ！", GetColor(255, 255, 255));
		DrawString(210, 400, "スペースキーでリトライ！", GetColor(255, 255, 255));

		// 入力チェック
		if (CheckHitKey(KEY_INPUT_RETURN)) {
			GameState = 0; // タイトル画面に戻る
			break;
		}
		else if (CheckHitKey(KEY_INPUT_SPACE))
		{
			InitializeGame(); // 初期化
			GameState = 1; // リトライ
			break;
		}

		// 画面更新
		ScreenFlip();
	}
}

// ゲームクリア画面
void ShowGameClearScreen() 
{
	while (ProcessMessage() == 0) {
		ClearDrawScreen();

		// ゲームクリア画面の描画
		SetFontSize(40);
		DrawString(70, 200, "ゲームクリア!おめでとう！", GetColor(255, 255, 0));

		SetFontSize(24);
		DrawFormatString(250, 290, GetColor(255, 255, 255), "スコア: %d", Score);

		SetFontSize(16);
		DrawString(200, 360, "Enterキーでタイトルに戻るよ！", GetColor(255, 255, 255));
		DrawString(210, 400, "スペースキーでゲーム終了！", GetColor(255, 255, 255));

		// 入力チェック
		if (CheckHitKey(KEY_INPUT_RETURN)) {
			GameState = 0; // タイトル画面に戻る
			break;
		}
		else if (CheckHitKey(KEY_INPUT_SPACE))
		{
			GameState = 3; // ゲーム終了
			break;
		}

		// 画面更新
		ScreenFlip();
	}
}


// メインプログラム
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpCmdLine, int nCmdShow)
{
	//ウィンドウモード変更と初期化と裏画面設定
	ChangeWindowMode(TRUE);
	SetGraphMode(640, 640, 32);;
	if (DxLib_Init() == -1) return -1;

	SetDrawScreen(DX_SCREEN_BACK);

	// 姫読み込み
	int HandlePrinces;
	HandlePrinces = LoadGraph("Resource/Character/character_hime_01_white_gold.png");

	// ゴースト読み込み
	int HandleGhost;
	HandleGhost = LoadGraph("Resource/Character/character_monster_shinigami_02.png");

	// 1マスずつ姫を動かすための変数
	// （これがなければ移動キーを押しっぱなしにした場合、一気に端のマスまで移動してしまう）
	bool KeyUpPressed = false;
    bool KeyDownPressed = false;
    bool KeyLeftPressedressed = false;
    bool KeyRightPressed = false;

	// 迷路初期化
	int MazeMap[MAZE_HEIGHT][MAZE_WIDTH];
	std::memcpy(MazeMap, Maze, sizeof(Maze));

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ゲーム状態で分岐
		// タイトル画面
		if (GameState == 0)
		{
			ShowTitleScreen(HandlePrinces, HandleGhost);
		}

		// ゲーム開始
		else if (GameState == 1)
		{
			// 全画面消去
			ClearDrawScreen();

			// キー入力の取得
			int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			// 姫の移動
			// ↑
			if ((Key & PAD_INPUT_UP) && !KeyUpPressed) {
				KeyUpPressed = true;
				if (MazeMap[(HimeY - CELL_SIZE / 2 - 5) / CELL_SIZE][HimeX / CELL_SIZE] != 1) {
					HimeY -= CELL_SIZE;
				}
			}
			else if (!(Key & PAD_INPUT_UP)) {
				KeyUpPressed = false;
			}

			// ↓
			if ((Key & PAD_INPUT_DOWN) && !KeyDownPressed) {
				KeyDownPressed = true;
				if (MazeMap[(HimeY + CELL_SIZE / 2 + 5) / CELL_SIZE][HimeX / CELL_SIZE] != 1) {
					HimeY += CELL_SIZE;
				}
			}
			else if (!(Key & PAD_INPUT_DOWN)) {
				KeyDownPressed = false;
			}

			// ←
			if ((Key & PAD_INPUT_LEFT) && !KeyLeftPressedressed) {
				KeyLeftPressedressed = true;
				if (MazeMap[HimeY / CELL_SIZE][(HimeX - CELL_SIZE / 2 - 5) / CELL_SIZE] != 1) {
					HimeX -= CELL_SIZE;
				}
			}
			else if (!(Key & PAD_INPUT_LEFT)) {
				KeyLeftPressedressed = false;
			}

			// →
			if ((Key & PAD_INPUT_RIGHT) && !KeyRightPressed) {
				KeyRightPressed = true;
				if (MazeMap[HimeY / CELL_SIZE][(HimeX + CELL_SIZE / 2 + 5) / CELL_SIZE] != 1) {
					HimeX += CELL_SIZE;
				}
			}
			else if (!(Key & PAD_INPUT_RIGHT)) {
				KeyRightPressed = false;
			}
			

			// ゴーストの移動
			if (++FrameCount % GhostSpeed == 0) {
				MoveGhost();
				FrameCount = 0;
			}

			// 姫がドットを取得する処理
			int MazeX = HimeX / CELL_SIZE;
			int MazeY = HimeY / CELL_SIZE;
			if (MazeMap[MazeY][MazeX] == 0) {
				MazeMap[MazeY][MazeX] = 2; // ドットを消す
				Score += 10; // スコアを増やす
			}

			// 衝突判定
			if (CheckCollision(HimeX, HimeY, GhostX, GhostY, 20))
			{
				GameState = 2; // ゲームオーバー画面へ
				std::memcpy(MazeMap, Maze, sizeof(Maze));

			}


			// 迷路の描画
			// 1は壁、2は通路、0はドット
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

			// ドットが迷路になければゲームクリア
			if (CountDot(MazeMap) == 0)
			{
				ShowGameClearScreen();
			}

			// 姫の描画
			DrawRotaGraph(HimeX, HimeY, 0.07, 0.0, HandlePrinces, TRUE);

			// ゴーストの描画
			DrawRotaGraph(GhostX, GhostY, 0.1, 0.0, HandleGhost, TRUE);


			// スコアの表示
			DrawFormatString(10, 10, GetColor(255, 255, 255), "Score: %d", Score);

			// 画面の更新
			ScreenFlip();
		}
		else if (GameState == 2)
		{
			// ゲームオーバー画面
			ShowGameOverScreen();
		}
		else if (GameState == 3)
		{
			break;
		}

	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}