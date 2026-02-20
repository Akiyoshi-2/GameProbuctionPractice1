#include "PlayScene.h"
#include "../../Input/Input.h"
#include "../SceneManager.h"
#include "../../Player/Player.h"
#include "../../Map/MapManager.h"



void InitPlayScene()
{
	InitPlayer();
	
	InitMap();
}

void LoadPlayScene()
{
	LoadPlayer();

	LoadMap();
}

// スタートは始まる時に起こる処理
void StartPlayScene()
{
	StartPlayer();

	StartMap();
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
	DrawMap();

	DrawPlayer();
}

void FinPlayScene()
{
	FinMap();

	FinPlayer();
}
