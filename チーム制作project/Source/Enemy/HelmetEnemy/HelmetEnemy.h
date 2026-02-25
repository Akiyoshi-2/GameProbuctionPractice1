#pragma once
#include "../EnemyParameter.h"
#include "../../Map/MapParameter.h"

void InitHelmetEnemy();
void LoadHelmetEnemy();
void StepHelmetEnemy();
void UpdateHelmetEnemy();
void DrawHelmetEnemy();
void FinHelmetEnemy();

void CreateHelmetEnemy(float posX, float posY, const EnemyParameter* param);

HelmetEnemyData* GetHelmetEnemy();

// ƒ}ƒbƒv‚Æ‚Ì“–‚½‚è”»’è
void HelmetEnemyHitBlockX(MapChipData mapChipData, int index);
void HelmetEnemyHitBlockY(MapChipData mapChipData, int index);

// ‚â‚ç‚ê”»’è
void PlayerKillHelmetEnemy(int index);
