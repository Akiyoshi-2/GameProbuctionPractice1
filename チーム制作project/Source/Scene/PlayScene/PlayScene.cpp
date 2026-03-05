#include "PlayScene.h"
#include "../../Input/Input.h"
#include "../SceneManager.h"
#include "../../Player/Player.h"
#include "../../Map/MapManager.h"
#include "../../Camera/Camera.h"
#include "../TitleScene/TitleScene.h"
#include "../../Collision/Collision.h"
#include "../ClearScene/ClearScene.h"
#include "../../Timer/Timer.h"

int g_Stage1Handle = -1;

void InitPlayScene()
{
	g_Stage1Handle = LoadGraph("Data/Title/Select/StageSelect.png");

	LoadPlayer();
	InitMap();
	InitTimer();
}

void LoadPlayScene(int stage)
{
	g_Stage1Handle = LoadGraph("Data/Title/Select/StageSelect.png");

	LoadPlayer();
	LoadMap(stage);
}

void StartPlayScene(int stage)
{

	ResetCamera();
	SetCameraStage(stage);

	StartPlayer(stage);
	StartMap(stage);
}

void StepPlayScene()
{
		
	StepPlayer();
	if (IsTriggerKey(KEY_C))
	{
		ChangeScene(SCENE_CLEAR);
	}
	

	if (IsTriggerKey(KEY_P))
	{
		g_ReturnFromGame = true;
		ChangeScene(SCENE_TITLE);
	}
		
}

void UpdatePlayScene()
{
	UpdatePlayer();
	UpdateCamera();
	CheckCollision();
	UpdatePlayerAnimation();
	UpdateTimer();
}

void DrawPlayScene()
{
	CameraData cam = GetCamera();

	if (g_Stage1Handle != -1)
	{
		DrawGraph(
			(int)-cam.posX,
			(int)-cam.posY,
			g_Stage1Handle,
			TRUE
		);
	}

	DrawMap();
	DrawPlayer();
	DrawCamera(); // デバッグ
	DrawTimer();
}


void FinPlayScene()
{
	FinMap();

	FinPlayer();
}
