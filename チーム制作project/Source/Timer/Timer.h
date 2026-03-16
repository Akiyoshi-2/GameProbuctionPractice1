#pragma once
#include "DxLib.h"

// タイマー
void InitTimer();
void StepTimer();
void UpdateTimer();
void DrawTimer();
void ResetTimer();

void SetTimerStage(int stage);

int GetLimitTime();
int GetRemainTime();