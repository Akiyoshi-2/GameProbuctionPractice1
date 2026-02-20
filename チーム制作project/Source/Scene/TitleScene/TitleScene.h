#pragma once
#include "DxLib.h"

enum Title
{
    TITLE_KEYUI,    //AnyKey Push
    MENU_SELECT,    //StageSelect
    MENU_TUTORIAL,  //Tutorial
    MENU_ARROW,
    SELECT_STAGE1,  
    SELECT_STAGE2,
    SELECT_STAGE3,
    SELECT_ARROW,
    SELECT_BACK,
    TITLE_MUX
};

struct TitleUIData
{
    int handle;
    VECTOR pos;

    int stage;
};

// 関数のプロトタイプ宣言
void InitTitleScene();
void LoadTitleScene();
void StartTitleScene();
void StepTitleScene();
void UpdateTitleScene();
void DrawTitleScene();
void FinTitleScene();
