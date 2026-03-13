#include "TutorialScene.h"

// --- DxLib / 基本入力 ---
#include "../../Input/Input.h"
#include "DxLib.h"

// --- シーン関連 ---
#include "../../Scene/SceneManager.h"
#include "../TitleScene/TitleScene.h"

// --- プレイヤー関連 ---
#include "../../Player/Player.h"
#include "../../Player/Attack/Attack.h"
#include "../../Player/Attack/Crush.h"
#include "../../LifeUI/Life.h"

// --- マップ・カメラ・アニメーション ---
#include "../../Map/MapManager.h"
#include "../../Camera/Camera.h"
#include "../../Animation/Animation.h"

// --- 敵・衝突 ---
#include "../../Collision/Collision.h"
#include "../../Enemy/EnemyManager.h"

// --- その他 ---
#include "../../Timer/Timer.h"
#include "../../Score/Score.h"

#include "../../Effect/Effect.h"
#include "../../Sound/SoundManager.h"

int g_TestHandle = -1;

void InitTutorialScene()
{
	g_TestHandle = -1;

	InitPlayer();      // プレイヤー初期化
	InitEnemy();       // 敵初期化
	InitMap();         // マップ初期化
	InitTimer();       // タイマー初期化
	InitScore();	   // スコア初期化
	InitLife();

	InitEffect();
}

void LoadTutorialScene(int stage)
{
	g_TestHandle = LoadGraph("Data/Title/Select/PlaySceneBG.png");

	LoadPlayer();
	LoadEnemy();
	LoadMap(stage);
	LoadLife();
	LoadEffect();
}

void StartTutorialScene(int stage)
{
	SetCameraStage(stage);
	StartPlayer(stage);
	StartMap(stage);

	PlayBGM(BGM_TUTORIAL);
}

void StepTutorialScene(int stage)
{
	// Pでタイトルに戻る（デバッグ用）
	if (IsTriggerKey(KEY_P))
	{
		g_ReturnFromGame = true;
		ChangeScene(SCENE_TITLE);
	}

	StepPlayer();               // プレイヤー更新
	StepEnemy();                // 敵更新
	StepEnemySpawnSystem(stage); // 敵スポーンシステム更新

	StepEffect();
}

void UpdateTutorialScene()
{
	UpdatePlayer();        // プレイヤー更新
	UpdateAttack();        // 攻撃更新
	UpdateEnemy();         // 敵更新
	UpdateCamera();        // カメラ更新
	CheckCollision();      // 衝突判定
	UpdatePlayerAnimation(); // プレイヤーアニメーション更新
	UpdateTimer();         // タイマー更新
	UpdateScore();

	UpdataEffect();
}

void DrawTutorialScene()
{
	CameraData cam = GetCamera();   // カメラ取得

	if (g_TestHandle != -1)
	{
		DrawGraph(
			(int)-cam.posX,       // カメラに合わせて描画
			(int)-cam.posY,
			g_TestHandle,
			TRUE
		);
	}

	DrawMap();        // 背景（ブロック）
	DrawPlayer();     // プレイヤー
	DrawEnemy();      // 敵
	DrawTimer();      // タイマー
	DrawScore();
	DrawLife();

	DrawEffect();

	// デバッグ表示
	/*DrawCamera();
	DrawAttack();
	DrawString(0, 40, "A:ジャンプ", GetColor(255, 255, 255));
	DrawString(0, 60, "B:攻撃", GetColor(255, 255, 255));
	DrawString(0, 80, "X:カラーチェンジ", GetColor(255, 255, 255));
	DrawString(0, 100, "RB:メニューに戻る", GetColor(255, 255, 255));*/
}

void FinTutorialScene()
{
	if (g_TestHandle != -1)
	{
		DeleteGraph(g_TestHandle);
		g_TestHandle = -1;
	}

	FinPlayer();   // プレイヤー終了処理
	FinEnemy();    // 敵終了処理
	FinMap();      // マップ終了処理
	FinLife();

	FinEffect();

	StopBGM(BGM_TUTORIAL);
}
