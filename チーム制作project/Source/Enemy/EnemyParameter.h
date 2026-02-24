#pragma once
#include "DxLib.h"
#include "../Animation/Animation.h"
#include "../Collision/Collision.h"

enum EnemyType
{
	NORMAL_ENEMY,
	HELMET_ENEMY,
	SHIELD_ENEMY,
	YELLOW_ENEMY,
	FULLARMOR_ENEMY,
};

struct EnemyParameter
{
	float EnemyDiePosX;
};

const EnemyParameter ENEMY_PARAMETER[] =
{
	4300.0f,
};

// Normal_Enemy関連
#define NORMAL_ENEMY_MAX	(255)
#define NORMAL_ENEMY_WIDTH	(38.0f)
#define NORMAL_ENEMY_HEIGHT	(38.0f)
#define NORMAL_ENEMY_RADIUD	(19.0f)

enum NormalEnemyAnimationType
{
	NORMAL_ENEMY_RUN,
	NORMAL_ENEMY_CRUSH,
	NORMAL_ENEMY_STRIKE,
	NORMAL_ENEMY_ANIM_MAX,
	NORMAL_ENEMY_ANIM_NONE = -1
};

struct NormalEnemyData
{
	bool isTurn;
	bool isAir;
	bool active;
	bool crush;
	bool strike;
	VECTOR pos;
	VECTOR move;
	AnimationData animation[NORMAL_ENEMY_MAX];
	NormalEnemyAnimationType playAnim;
	BoxCollision boxCollision;
	const EnemyParameter* param;
	NormalEnemyData* data;
};



// スポーンデータ
struct EnemySpawnData
{
	EnemyType type;
	int spawnTime;
	float posX;
	float posY;
	const EnemyParameter* param;
};