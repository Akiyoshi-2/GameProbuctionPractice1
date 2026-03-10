#include "PlayScene.h"
#include "../../Input/Input.h"
#include "../SceneManager.h"
#include "../../Player/Player.h"
#include "../../Map/MapManager.h"
#include "../../Camera/Camera.h"
#include "../TitleScene/TitleScene.h"
#include "../../Collision/Collision.h"
#include "../ClearScene/ClearScene.h"
#include "../GameOverScene/GameOverScene.h"
#include "../../Timer/Timer.h"
#include "../../Player/Attack/Attack.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Score/Score.h"

int g_Stage1Handle = -1;

void InitPlayScene()
{
	g_Stage1Handle = LoadGraph("Data/Title/Select/StageSelect.png");

	InitPlayer();
	InitEnemy();
	InitMap();
	InitTimer();
	InitScore();
}

void LoadPlayScene(int stage)
{
	g_Stage1Handle = LoadGraph("Data/Title/Select/StageSelect.png");

	LoadPlayer();
	LoadEnemy();
	LoadMap(stage);
}

void StartPlayScene(int stage)
{

	ResetCamera();
	SetCameraStage(stage);

	StartPlayer(stage);
	StartMap(stage);
}

void StepPlayScene(int stage)
{

	StepPlayer();

	if (IsTriggerKey(KEY_C)) //デバッグ用
	{
		ChangeScene(SCENE_STAGE_2);
	}



	if (IsTriggerKey(KEY_P))
	{
		g_ReturnFromGame = true;
		ChangeScene(SCENE_TITLE);
	}

	StepEnemy();
	StepEnemySpawnSystem(stage);
}

void UpdatePlayScene()
{
	UpdatePlayer();        // プレイヤー更新
	UpdateAttack();        // 攻撃更新
	UpdateEnemy();         // 敵更新
	UpdateCamera();        // カメラ更新
	CheckCollision();      // 衝突判定
	UpdatePlayerAnimation(); // プレイヤーアニメーション更新
	UpdateTimer();         // タイマー更新
	UpdateScore();
}

void DrawPlayScene()
{
	CameraData cam = GetCamera();   // カメラ取得

	if (g_Stage1Handle != -1)
	{
		DrawGraph(
			(int)-cam.posX,       // カメラに合わせて描画
			(int)-cam.posY,
			g_Stage1Handle,
			TRUE
		);
	}

	DrawMap();        // 背景（ブロック）
	DrawPlayer();     // プレイヤー
	DrawEnemy();      // 敵
	DrawTimer();      // タイマー
	DrawScore();

	// デバッグ表示
	DrawCamera();
	DrawAttack();
	DrawString(0, 40, "A:ジャンプ", GetColor(255, 255, 255));
	DrawString(0, 60, "B:攻撃", GetColor(255, 255, 255));
	DrawString(0, 80, "X:カラーチェンジ", GetColor(255, 255, 255));
	DrawString(0, 100, "RB:メニューに戻る", GetColor(255, 255, 255));
}


void FinPlayScene()
{
	if (g_Stage1Handle != -1)
	{
		DeleteGraph(g_Stage1Handle);
		g_Stage1Handle = -1;
	}

	FinPlayer();   // プレイヤー終了処理
	FinEnemy();    // 敵終了処理
	FinMap();      // マップ終了処理
}
