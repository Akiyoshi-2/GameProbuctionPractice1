#pragma once
#include "DxLib.h"

enum KillTutorialType
{
	VS_NORMAL,
	VS_HELMET,
	VS_SHIELD,
	VS_YELLOW,
	PLAYER_CHANGE_R,
	PLAYER_CHANGE_Q,
	PLAYER_MOVE,
	VS_MAX
};

struct KillTutorialTypeData
{
	int handle;
	VECTOR pos;
};

void InitTutorialScene();
void LoadTutorialScene(int stage);
void StartTutorialScene(int stage);
void StepTutorialScene(int stage);
void UpdateTutorialScene();
void DrawTutorialScene();
void FinTutorialScene();