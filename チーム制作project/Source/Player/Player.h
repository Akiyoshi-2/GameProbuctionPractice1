#pragma once
#include "../Collision/Collision.h"
#include "../Map/MapParameter.h"
#include "../Animation/Animation.h"

#define PLAYER_WIDTH	(72.0f)
#define PLAYER_HEIGHT	(72.0f)
#define PLAYER_RADIUS	(36.0f)


enum PlayerType
{
	TYPE_RED,
	TYPE_BLUE,
	TYPE_YELLOW
};

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
	bool active;
	bool isTurn;
	bool isAir;
	BoxCollision boxCollision;

};

PlayerData GetPlayer();

void PlayerHitFullarmor_Enemy();
void PlayerHitHelmet_Enemy();
void PlayerHitNormal_Enemy();
void PlayerHitShield_Enemy();
void PlayerHitYellow_Enemy();

void PlayerHitNormalBlockX(MapChipData mapChipData);
void PlayerHitNormalBlockY(MapChipData mapChipData);