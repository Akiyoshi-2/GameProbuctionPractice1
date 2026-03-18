#pragma once
#include "DxLib.h"

// タイマー（制限時間）
void InitTimer();
void StepTimer();
void UpdateTimer();
void DrawTimer();
void ResetTimer();

void SetTimerStage(int stage);

int GetLimitTime();
int GetRemainTime();

// クリアタイム保存（ゴール時に呼ぶ）
void SaveClearTime();

// クリアタイム取得（ClearSceneで使う）
int GetClearTime();