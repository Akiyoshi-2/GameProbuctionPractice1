#include "TutorialScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"
#include "../../Player/Player.h"
#include "../../Map/MapManager.h"
#include "../../Camera/Camera.h"
#include "../../Animation/Animation.h"
#include "../TitleScene/TitleScene.h"
#include "../../Collision/Collision.h"
#include "../../Enemy/EnemyManager.h"
#include "../../Timer/Timer.h"
#include "../../Player/Attack/Attack.h"

int g_TestHandle = -1;

void InitTutorialScene()
{
	g_TestHandle = -1;

	InitPlayer();
	InitEnemy();
	InitMap();
	InitTimer();
}

void LoadTutorialScene(int stage)
{
	g_TestHandle = LoadGraph("Data/Title/Select/StageSelect.png");

	LoadPlayer();
	LoadEnemy();
	LoadMap(stage);
}

void StartTutorialScene(int stage)
{
	SetCameraStage(stage);
	StartPlayer(stage);
	StartMap(stage);
}

void StepTutorialScene(int stage)
{
	// Pでタイトルに戻る（デバッグ用）
	if (IsTriggerKey(KEY_P))
	{
		g_ReturnFromGame = true;
		ChangeScene(SCENE_TITLE);

	}

	StepPlayer();

	StepEnemy();

	StepEnemySpawnSystem(stage);


}

void UpdateTutorialScene()
{

	UpdatePlayer();
	UpdateEnemy();
	UpdateCamera();
	CheckCollision();
	UpdatePlayerAnimation();
	UpdateTimer();
	UpdateAttack();
}

void DrawTutorialScene()
{
	CameraData cam = GetCamera();   //追加

	if (g_TestHandle != -1)
	{
		DrawGraph(
			(int)-cam.posX,         // カメラを引く
			(int)-cam.posY,
			g_TestHandle,
			TRUE
		);
	}

	DrawMap();      // 背景（ブロック）
	DrawPlayer();   // プレイヤー
	DrawEnemy();
	DrawTimer();
	//デバック
	DrawCamera();
	DrawAttack();
	DrawString(0, 40, "A:ジャンプ", GetColor(255, 255, 255));
	DrawString(0, 60, "B:攻撃", GetColor(255, 255, 255));
	DrawString(0, 80, "X:カラーチェンジ", GetColor(255, 255, 255));
	DrawString(0, 100, "RB:メニューに戻る", GetColor(255, 255, 255));
}

void FinTutorialScene()
{
	if (g_TestHandle != -1)
	{
		DeleteGraph(g_TestHandle);
		g_TestHandle = -1;
	}

	FinPlayer();
	FinEnemy();
	FinMap();
}
