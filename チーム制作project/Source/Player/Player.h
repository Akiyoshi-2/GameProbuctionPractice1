#pragma once
#include "../Collision/Collision.h"
#include "../Map/MapParameter.h"
#include "../Animation/Animation.h"

#define PLAYER_WIDTH	(72.0f)
#define PLAYER_HEIGHT	(72.0f)
#define PLAYER_RADIUS	(36.0f)

enum PlayerAnimationType
{
	RRED_PLAYER_ANIM_ATTACK,
	RED_PLAYER_ANIM_IDLE,
	RED_PLAYER_ANIM_RUN,
	RED_PLAYER_ANIM_JUMP,
	RED_PLAYER_ANIM_FALL,
	RED_PLAYER_ANIM_DIE,
	BLUE_PLAYER_ANIM_IDLE,
	BLUE_PLAYER_ANIM_RUN,
	BLUE_PLAYER_ANIM_JUMP,
	BLUE_PLAYER_ANIM_FALL,
	BLUE_PLAYER_ANIM_DIE,
	YELLOW_PLAYER_ANIM_IDLE,
	YELLOW_PLAYER_ANIM_RUN,
	YELLOW_PLAYER_ANIM_JUMP,
	YELLOW_PLAYER_ANIM_FALL,
	PLAYER_ANIM_MAX,
	PLAYER_ANIM_NONE = -1
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
	int handle;
	float posX;
	float posY;
	float moveX;
	float moveY;
	float hitFlag;
	bool active;
	bool isTurn;
	bool isAir;
	BoxCollision boxCollision;
	AnimationData animation[PLAYER_ANIM_MAX];
	PlayerAnimationType playerAnim;
};

PlayerData* GetPlayer();

//void PlayerHitFullarmor_Enemy();
//void PlayerHitHelmet_Enemy();
//void PlayerHitNormal_Enemy();
//void PlayerHitShield_Enemy();
//void PlayerHitYellow_Enemy();
//
//void PlayerHitNormalBlockX(MapChipData mapChipData);
//void PlayerHitNormalBlockY(MapChipData mapChipData);