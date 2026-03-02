#pragma once
#include "../EnemyParameter.h"
#include "../../Map/MapParameter.h"

void InitFullArmorEnemy();
void LoadFullArmorEnemy();
void StepFullArmorEnemy();
void UpdateFullArmorEnemy();
void DrawFullArmorEnemy();
void FinFullArmorEnemy();

void CreateFullArmorEnemy(float posX, float posY, const EnemyParameter* param);

FullArmEnemyData* GetFullArmorEnemy();

// ƒ}ƒbƒv‚Æ‚Ì“–‚½‚è”»’è
void FullArmorEnemyHitBlockX(MapChipData mapChipData, int index);
void FullArmorEnemyHitBlockY(MapChipData mapChipData, int index);

// ‚â‚ç‚ê”»’è
void PlayerKillFullArmorEnemy(int index);