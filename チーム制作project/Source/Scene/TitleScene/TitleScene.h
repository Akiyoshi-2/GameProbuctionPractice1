#pragma once
#include "DxLib.h"

enum Title
{
    TITLE_KEYUI,
    MENU_SELECT,
    MENU_TUTORIAL,
    MENU_ARROW,
    TITLE_MUX
};

struct TitleUIData
{
    int handle;
    VECTOR pos;
};

// 関数のプロトタイプ宣言
void InitTitleScene();
void LoadTitleScene();
void StartTitleScene();
void StepTitleScene();
void UpdateTitleScene();
void DrawTitleScene();
void FinTitleScene();
