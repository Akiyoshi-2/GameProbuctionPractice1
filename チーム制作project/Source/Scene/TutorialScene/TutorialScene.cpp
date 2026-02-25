#include "TutorialScene.h"
#include "../../Input/Input.h"
#include "../../Scene/SceneManager.h"
#include "../../Player/Player.h"
#include "../../Map/MapManager.h"
#include "../../Camera/Camera.h"
#include "../../Animation/Animation.h"
#include "../TitleScene/TitleScene.h"
#include "../../Collision/Collision.h"

int g_TestHandle = -1;

void InitTutorialScene()
{
	g_TestHandle = -1;

	InitPlayer();
	InitMap();
}

void LoadTutorialScene()
{
	g_TestHandle = LoadGraph("Data/Title/Select/StageSelect.png");

	LoadPlayer();
	LoadMap();
}

void StartTutorialScene()
{
	StartPlayer();
	StartMap();
}

void StepTutorialScene()
{
	// Pでタイトルに戻る（デバッグ用）
	if (IsTriggerKey(KEY_P))
	{
		g_ReturnFromGame = true;
		ChangeScene(SCENE_TITLE);
	}

	StepPlayer();
}

void UpdateTutorialScene()
{
	UpdatePlayer();
	UpdateCamera();
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
	DrawCamera();   // デバッグ
}

void FinTutorialScene()
{
	if (g_TestHandle != -1)
	{
		DeleteGraph(g_TestHandle);
		g_TestHandle = -1;
	}

	FinPlayer();
	FinMap();
}
