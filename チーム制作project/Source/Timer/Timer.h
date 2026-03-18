#pragma once

void InitTimer();
void StepTimer();
void UpdateTimer();
void DrawTimer();
void ResetTimer();

void SetTimerStage(int stage);

void SaveClearTime(int stage);

int GetClearTime();
int GetStageClearTime(int stage);

int GetRemainTime();
int GetLimitTime();

int GetCurrentStage();