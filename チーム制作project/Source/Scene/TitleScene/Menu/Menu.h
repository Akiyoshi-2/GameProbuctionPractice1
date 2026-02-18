#pragma once
#include "DxLib.h"

enum MenuUIType
{
	MENU_SELECT,
	MENU_TUTORIAL,
	MENU_ARROW,
	MENU_MAX
};

struct MenuUIData
{
	int handle;
	VECTOR pos;
};

void InitMenuScene();
void LoadMenuScene();
void StartMenuScene();
void StepMenuScene();
void UpdateMenuScene();
void DrawMenuScene();
void FinMenuScene();