#pragma once
#include "../EnemyParameter.h"
#include "../../Map/MapParameter.h"

void InitShieldEnemy();
void LoadShieldEnemy();
void StepShieldEnemy();
void UpdateShieldEnemy();
void DrawShieldEnemy();
void FinShieldEnemy();

void CreatShieldEnemy(float posX, float posY, const EnemyParameter* param);

ShieldEnemyData* GetShieldEnemy();

void ShieldEnemyHitBlockX(MapChipData mapChipData, int index);
void ShieldEnemyHitBlockY(MapChipData mapChipData, int index);

void PlayerKillShieldEnemy(int index);