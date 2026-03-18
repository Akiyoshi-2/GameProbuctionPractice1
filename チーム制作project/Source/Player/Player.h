#pragma once
#include "DxLib.h"
#include "../Map/MapParameter.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"
#include "../Enemy/EnemyParameter.h"

#define PLAYER_WIDTH	(50.0f)
#define PLAYER_HEIGHT	(50.0f)
#define PLAYER_RADIUS	(25.0f)

// 死亡後のリスポーン待機時間
#define PLAYER_DIE_TIME (60)

//最大残機
#define PLAYER_MAX_LIFE (10)

// 黄色状態の持続時間（60 = 1秒）
#define PLAYER_YELLOW_TIME (600)

// 黄色状態エフェクトインターバル
#define YELLOW_EFFECT_INTERVAL	(8)

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
	float hitFlag;				//ヒットフラグ
	int canJump;				//連続ジャンプ防止用
	int changeTypeCoolTime;		//切替クールタイム
	int yellowRemainTime;	    //黄色の残り時間（フレーム)
	int life;					//残機

	bool active;				//生存フラグ
	bool isTurn;				//向き
	bool isAir;					//空中にいるかどうか
	bool isAttacking;			//攻撃中かどうか
	int attackTimer;			//攻撃継続時間

	bool isDead;				//死亡中かどうか

	int  deadTimer;				//死亡時間

	PlayerType type;			//タイプ管理
	PlayerType prevType;		// 黄色に入る前のタイプ

	AnimationData animation[PLAYER_ANIM_MAX];	// アニメーション
	PlayerAnimationType playerAnim;				// 再生中のアニメーション

	BoxCollision boxCollision;					// 矩形当たり判定

	bool selectingYellow;	// 選択画面中か

	bool g_BlockAttackInput = false;
};

void InitPlayer();
void LoadPlayer();
void StartPlayer(int stage);
void StepPlayer();
void UpdatePlayer();
void DrawPlayer();
void FinPlayer();
void StartPlayerAnimation(PlayerAnimationType anim);	// アニメーション再生
void UpdatePlayerAnimation();

PlayerData* GetPlayer();

void PlayerHitNormalBlockX(MapChipData mapChipData);
void PlayerHitNormalBlockY(MapChipData mapChipData);
void PlayerHitThornBlockX(MapChipData mapChipData);
void PlayerHitThornBlockY(MapChipData mapChipData);

void PlayerHitEnemy();
void PlayerDie();

int GetPlayerDeathCount();
void ResetPlayerDeathCount();