#pragma once
#include "../EnemyParameter.h"
#include "../../Map/MapParameter.h"

void InitYellowEnemy();
void LoadYellowEnemy();
void StepYellowEnemy();
void UpdateYellowEnemy();
void DrawYellowEnemy();
void FinYellowEnemy();

void CreateYellowEnemy(float posX, float posY, const EnemyParameter* param);

YellowEnemyData* GetYellowEnemy();

// ƒ}ƒbƒv‚Æ‚Ì“–‚½‚è”»’è
void YellowEnemyHitBlockX(MapChipData mapChipData, int index);
void YellowEnemyHitBlockY(MapChipData mapChipData, int index);

// ‚â‚ç‚ê”»’è
void PlayerKillYellowEnemy(int index);