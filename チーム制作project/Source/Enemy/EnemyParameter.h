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

// NORMAL_ENEMY関連
#define NORMAL_ENEMY_MAX	(255)
#define NORMAL_ENEMY_WIDTH	(38.0f)
#define NORMAL_ENEMY_HEIGHT	(38.0f)
#define NORMAL_ENEMY_RADIUD	(19.0f)

// アニメーションタイプ
enum NormalEnemyAnimationType
{
	NORMAL_ENEMY_RUN,
	NORMAL_ENEMY_CRUSH,
	NORMAL_ENEMY_STRIKE,
	NORMAL_ENEMY_ANIM_MAX,
	NORMAL_ENEMY_ANIM_NONE = -1
};

// EnemyDatat
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


// HELMET_ENEMY関連
#define HELMET_ENEMY_MAX	(255)
#define HELMET_ENEMY_WIDTH	(38.0f)
#define HELMET_ENEMY_HEIGHT	(39.0f)
#define HELMET_ENEMY_RADIUS	(19.5f)

// アニメーションタイプ
enum HelmetAnimationType
{
	HELMET_ENEMY_ANIM_RUN,
	HELMET_ENEMY_ANIM_DIE,
	HELMET_ENEMY_ANIM_MAX,
	HELMET_ENEMY_ANIM_NONE = -1
};

// EnemyData
struct HelmetEnemyData
{
	bool isTurn;
	bool isAir;
	bool active;
	VECTOR pos;
	VECTOR move;
	AnimationData animation[HELMET_ENEMY_ANIM_MAX];
	HelmetAnimationType playAnim;
	BoxCollision boxCollision;
	const EnemyParameter* param;
	HelmetEnemyData* data;
};

// ShieldEnemy関連
#define SHIELD_ENEMY_MAX	(255)
#define SHIELD_ENEMY_WIDTH	(38.0f)
#define SHIELD_ENEMY_HEIGHT	(38.0f)
#define SHIELD_ENEMY_RADIUD	(19.0f)

//アニメーションタイプ
enum ShieldEnemyAnimationType
{
	SHIELD_ENEMY_RUN,
	SHIELD_ENEMY_DIE,
	SHIELD_ENEMY_ANIM_MAX,
	SHIELD_ENEMY_ANIM_NONE = -1

};


// EnemyData
struct ShieldEnemyData
{
	bool isTurn;
	bool isAir;
	bool active;
	VECTOR pos;
	VECTOR move;
	AnimationData animation[SHIELD_ENEMY_ANIM_MAX];
	ShieldEnemyAnimationType playAnim;
	BoxCollision boxCollision;
	const EnemyParameter* param;
	ShieldEnemyData* data;
};

//////////////////////////////////

// スポーンデータ
struct EnemySpawnData
{
	EnemyType type;
	int spawnTime;
	float posX;
	float posY;
	const EnemyParameter* param;
};

