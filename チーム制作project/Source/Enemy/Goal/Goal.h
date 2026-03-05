#pragma once
#include "../EnemyParameter.h"


void InitGoal();
void LoadGoal();
void StepGoal();
void UpdateGoal();
void DrawGoal();
void FinGoal();

void CreateGoal(float posX, float posY, const EnemyParameter* param);

GoalData* GetGoal();

void PlayerHitGoal();