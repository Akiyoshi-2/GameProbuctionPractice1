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
#include "../../Player/LifeUI/Life.h"

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
#include "../../Player/YellowSelect/YellowSelect.h"
#include "../../SaveData/SaveData.h"


int g_TestHandle = -1;
KillTutorialTypeData g_KillTutorialData[VS_MAX] = {};

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
	InitYellowSelect();
}

void LoadTutorialScene(int stage)
{

	g_TestHandle = LoadGraph("Data/Title/Select/PlaySceneBG.png");
	g_KillTutorialData[VS_NORMAL].handle = LoadGraph("Data/EnemyKillTutorial/NormalEnemy_Tutorial(透過).png");
	g_KillTutorialData[VS_HELMET].handle = LoadGraph("Data/EnemyKillTutorial/HelmetEnemy_Tutorial(透過).png");
	g_KillTutorialData[VS_SHIELD].handle = LoadGraph("Data/EnemyKillTutorial/ShieldEnemy_Tutorial(透過).png");
	g_KillTutorialData[VS_YELLOW].handle = LoadGraph("Data/EnemyKillTutorial/YellowEnemy_Tutorial(透過).png");
	g_KillTutorialData[PLAYER_CHANGE_R].handle = LoadGraph("Data/PlayerTutorial/PlayerChange.png");
	g_KillTutorialData[PLAYER_CHANGE_Q].handle = LoadGraph("Data/PlayerTutorial/PlayerChangeYellow.png");
	g_KillTutorialData[PLAYER_MOVE].handle = LoadGraph("Data/PlayerTutorial/操作説明.png");

	LoadPlayer();
	LoadEnemy();
	LoadMap(stage);
	LoadLife();

	LoadEffect();
}

void StartTutorialScene(int stage)
{
	g_KillTutorialData[VS_NORMAL].pos = VGet(2400, 400, 0);
	g_KillTutorialData[VS_HELMET].pos = VGet(2925, 400, 0);
	g_KillTutorialData[VS_SHIELD].pos = VGet(3425, 400, 0);
	g_KillTutorialData[VS_YELLOW].pos = VGet(3925, 400, 0);

	g_KillTutorialData[PLAYER_CHANGE_R].pos = VGet(1450, 400, 0);
	g_KillTutorialData[PLAYER_CHANGE_Q].pos = VGet(4600, 400, 0);
	g_KillTutorialData[PLAYER_MOVE].pos = VGet(300, 500, 0);

	g_DecidedStage = 0;

	SetCameraStage(stage);
	StartPlayer(stage);
	StartMap(stage);

	PlayBGM(BGM_TUTORIAL);

	PlayerData* player = GetPlayer();
	if (player == nullptr) return;

	if (g_IsTutorialMode)
	{
		player->life = 999;
	}
	else
	{
		int life;
		int score;
		int yellow;

		LoadGameData(life, score, yellow);

		player->life = life;
		SetScore(score);
	}
}

void StepTutorialScene(int stage)
{
	// Pでタイトルに戻る（デバッグ用）
	if (IsTriggerKey(KEY_P))
	{
		PlayerData* player = GetPlayer();
		if (player != nullptr)
		{
			player->life = 3;  // 通常ライフに戻す
		}

		g_ReturnFromGame = true;
		g_IsTutorialMode = false;

		ChangeScene(SCENE_TITLE);
	}

	StepPlayer();               // プレイヤー更新
	StepYellowSelect();
	StepEnemy();                // 敵更新
	StepEnemySpawnSystem(stage); // 敵スポーンシステム更新
	StepTimer();

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
			0,
			0,
			g_TestHandle,
			TRUE
		);
	}

	DrawMap();        // 背景（ブロック）

	for (int i = 0; i < VS_MAX; i++)
	{
		DrawGraph(
			(int)(g_KillTutorialData[i].pos.x - cam.posX),
			(int)(g_KillTutorialData[i].pos.y - cam.posY),
			g_KillTutorialData[i].handle,
			TRUE
		);
	}

	DrawPlayer();     // プレイヤー
	DrawEnemy();      // 敵
	DrawTimer();      // タイマー
	DrawScore();
	DrawLife();

	DrawEffect();
	DrawYellowSelect();

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
	for (int i = 0; i < VS_MAX; i++)
	{
		if (g_KillTutorialData[i].handle != -1)
		{
			DeleteGraph(g_KillTutorialData[i].handle);
			g_KillTutorialData[i].handle = -1;
		}

	}

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
