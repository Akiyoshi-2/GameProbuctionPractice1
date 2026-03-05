#pragma once

void InitMap();
void LoadMap(int stage);
void StartMap(int stage);
void DrawMap();
void FinMap();

void CheckMapPlayerCpllision();
void CheckMapNormalEnemyCollision();
void CheckMapHelmetEnemyCollision();
void CheckMapShieldEnemyCollision();
void CheckMapYellowEnemyCollision();
void CheckMapFullarmorEnemyCollision();
