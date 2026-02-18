#include "PlayScene.h"
#include "../../Input/Input.h"
#include "../SceneManager.h"
#include "../../Player/Player.h"



void InitPlayScene()
{
	InitPlayer();
}

void LoadPlayScene()
{
	LoadPlayer();
}

// スタートは始まる時に起こる処理
void StartPlayScene()
{
	StartPlayer();
}

void StepPlayScene()
{
	StepPlayer();
}

void UpdatePlayScene()
{
	UpdatePlayer();
}

void DrawPlayScene()
{
	DrawPlayer();
}

void FinPlayScene()
{
	FinPlayer();
}
