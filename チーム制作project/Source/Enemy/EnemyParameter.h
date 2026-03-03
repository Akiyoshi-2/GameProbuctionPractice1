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
	float EnemyDiePosY;
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

// YELLOW_ENEMY関連
#define YELLOW_ENEMY_MAX	(255)
#define YELLOW_ENEMY_WIDTH	(38.0f)
#define YELLOW_ENEMY_HEIGHT	(38.0f)
#define YELLOW_ENEMY_RADIUS	(19.0f)

// アニメーションタイプ
enum YellowEnemyAnimationType
{
	YELLOW_ENEMY_RUN_1,
	YELLOW_ENEMY_RUN_2,
	YELLOW_ENEMY_STUN,
	YELLOW_ENEMY_CRUSH,
	YELLOW_ENEMY_STRIKE,
	YELLOW_ENEMY_ANIM_MAX,
	YELLOW_ENEMY_ANIM_NONE = -1
};

// EnemyData
struct YellowEnemyData
{
	bool isTurn;
	bool isAir;
	bool active;
	bool stun;
	bool crush;
	bool strike;
	int enemyHP;
	VECTOR pos;
	VECTOR move;
	AnimationData animation[YELLOW_ENEMY_ANIM_MAX];
	YellowEnemyAnimationType playAnim;
	BoxCollision boxCollision;
	const EnemyParameter* param;
	YellowEnemyData* date;
};

// FULLARMOR_ENEMY関連
#define FULLARMOR_ENEMY_MAX	(255)
#define FULLARMOR_ENEMY_WIDTH	(40.0f)
#define FULLARMOR_ENEMY_HEIGHT	(43.0f)
#define FULLARMOR_ENEMY_RADIUD	(21.0f)

enum FullArmEnemyAnimationType
{
	FULLARMOR_ENEMY_ANIM_RUN,
	FULLARMOR_ENEMY_ANIM_DIE,
	FULLARMOR_ENEMY_ANIM_MAX,
	FULLARMOR_ENEMY_ANIM_NONE = -1
};

struct FullArmEnemyData
{
	bool isTurn;
	bool isAir;
	bool active;
	VECTOR pos;
	VECTOR move;
	AnimationData animation[FULLARMOR_ENEMY_ANIM_MAX];
	FullArmEnemyAnimationType playAnim;
	BoxCollision boxCollision;
	const EnemyParameter* param;
};

//////////////////////////////////

// スポーンデータ
struct EnemySpawnData
{
	EnemyType type;
	float posX;
	float posY;
	const EnemyParameter* param;
};

// 配列
const EnemySpawnData ENEMY_SPAWN_DATA_0[] =
{
	NORMAL_ENEMY, 4400.0f, 350.0f,& ENEMY_PARAMETER[0],
	HELMET_ENEMY, 4400.0f, 650.0f,& ENEMY_PARAMETER[0],
	SHIELD_ENEMY, 4500.0f, 950.0f,& ENEMY_PARAMETER[0],


};

const EnemySpawnData ENEMY_SPAWN_DATA_1[] =
{
	NORMAL_ENEMY, 4400.0f, 350.0f,& ENEMY_PARAMETER[0],
	HELMET_ENEMY, 4400.0f, 650.0f,& ENEMY_PARAMETER[0],
	SHIELD_ENEMY, 4500.0f, 950.0f,& ENEMY_PARAMETER[0],
};

const EnemySpawnData ENEMY_SPAWN_DATA_2[] =
{
	NORMAL_ENEMY, 4400.0f, 350.0f,&ENEMY_PARAMETER[0],
	HELMET_ENEMY, 4400.0f, 650.0f,&ENEMY_PARAMETER[0],
	SHIELD_ENEMY, 4500.0f, 950.0f,&ENEMY_PARAMETER[0],
};

const EnemySpawnData ENEMY_SPAWN_DATA_3[] =
{
	NORMAL_ENEMY, 4400.0f, 350.0f,&ENEMY_PARAMETER[0],
	HELMET_ENEMY, 4400.0f, 650.0f,&ENEMY_PARAMETER[0],
	SHIELD_ENEMY, 4500.0f, 950.0f,&ENEMY_PARAMETER[0],
};