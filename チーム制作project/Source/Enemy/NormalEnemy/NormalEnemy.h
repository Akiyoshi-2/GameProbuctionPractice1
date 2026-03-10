#pragma once
#include "../EnemyParameter.h"
#include "../../Map/MapParameter.h"


void InitNormalEnemy();
void LoadNormalEnemy();
void StepNormalEnemy();
void UpdateNormalEnemy();
void DrawNormalEnemy();
void FinNormalEnemy();

void CreateNomalEnemy(float posX, float posY, const EnemyParameter* param);

NormalEnemyData* GetNormalEnemy();

// ƒ}ƒbƒv‚Æ‚Ì“–‚½‚è”»’è
void NormalEnemyHitBlockX(MapChipData mapChipData, int index);
void NormalEnemyHitBlockY(MapChipData mapChipData, int index);

void NormalEnemyHitStopBlockX(MapChipData mapChipData, int index);

// ‚â‚ç‚ê”»’è
void PlayerKillNormalEnemy(int index);