#pragma once
#include "../EnemyParameter.h"
#include "../../Map/MapParameter.h"

void InitShieldEnemy();
void LoadShieldEnemy();
void StepShieldEnemy();
void UpdateShieldEnemy();
void DrawShieldEnemy();
void FinShieldEnemy();

void CreateShieldEnemy(float posX, float posY, const EnemyParameter* param);

ShieldEnemyData* GetShieldEnemy();

void ShieldEnemyHitBlockX(MapChipData mapChipData, int index);
void ShieldEnemyHitBlockY(MapChipData mapChipData, int index);

void PlayerKillShieldEnemy(int index);
void PlayerKillShieldEnemyYellow(int index);
bool UpdateShieldCrush(int enemyIndex);