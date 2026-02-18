#pragma once

void InitPlayer();
void LoadPlayer();
void StartPlayer();
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();

struct PlayerData
{
	int g_PlayerHandle;
	float posX;
	float posY;
	float moveX;
	float moveY;

};

PlayerData GetPlayer();