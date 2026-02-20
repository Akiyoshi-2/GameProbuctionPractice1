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
	float width;
	float height;
	float hitFlag;

};

PlayerData GetPlayer();

void PlayerHitFullarmor_Enemy();
void PlayerHitHelmet_Enemy();
void PlayerHitNormal_Enemy();
void PlayerHitShield_Enemy();
void PlayerHitYellow_Enemy();