#pragma once
#include "../Map/MapParameter.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"

#define PLAYER_WIDTH	(50.0f)
#define PLAYER_HEIGHT	(50.0f)
#define PLAYER_RADIUS	(36.0f)

enum PlayerAnimationType
{
	RED_PLAYER_ANIM_ATTACK,
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

enum PlayerType
{
	TYPE_RED,
	TYPE_BLUE,
	TYPE_YELLOW
};

struct PlayerData
{
	int handle;
	VECTOR pos;
	VECTOR move;
	float hitFlag;
	int canJump;			//連続ジャンプ防止用
	int changeTypeCoolTime;	//切替クールタイム
	int yellowRemainTime;     // 黄色の残り時間（フレーム

	bool active;
	bool isTurn;
	bool isAir;

	PlayerType type;
	PlayerType prevType;      // 黄色に入る前のタイプ

	AnimationData animation[PLAYER_ANIM_MAX];	// アニメーション
	PlayerAnimationType playerAnim;				// 再生中のアニメーション

	BoxCollision boxCollision;					// 矩形当たり判定

};

void InitPlayer();
void LoadPlayer();
void StartPlayer(int stage);
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();
void UpdatePlayerAnimation();


PlayerData* GetPlayer();

void PlayerHitNormalBlockX(MapChipData mapChipData);
void PlayerHitNormalBlockY(MapChipData mapChipData);